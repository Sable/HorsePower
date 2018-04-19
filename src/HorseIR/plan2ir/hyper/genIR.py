import json
import sys
import time
import copy

from codegen import *
from codelib import *
from collections import OrderedDict

# Class definitions
# class Node(object):
#     def __init__(self, d):
#         self.left  = None
#         self.right = None
#         self.value = d
#         self.value2= None

# def build():
#     r = Node()
#     r.data  = 'cool'
#     r.left  = Node()
#     r.left.data = 'left'
#     r.right = Node()
#     r.right.data = 'right'
#     return r

table_list    = []
column_list   = []
value_list    = []
current_table = ''
string_map    = {} # mapping
prev_mask_t   = '' # mask returned from predicates
final_order   = ''
current_resz  = '' # current residues
left_join_mask= ''

col_to_table  = {}
table_to_cols = {}
join_list     = [] # find join pairs in condition
id_to_env     = {} # map operatorId to env

"""
string_map
- insertMap: allow key updates
- getMap: raise an error if key not found
"""
def insertMap(key, string):
    string_map[key] = string
    return key

def getMap(key):
    if key in string_map:
        return string_map[key]
    else:
        warning("%s not found in map" % key)

def linkCol2Table(c, t):
    if c not in col_to_table:
        col_to_table[c] = t
    else:
        warning('column %c points to table %s and %s at the same time' % (c,t,col_to_table[c]))

def linkTable2Cols(t, c):
    if t not in table_to_cols:
        table_to_cols[t] = []
    table_to_cols[t].append(c)

def getTableFromCol(c):
    if c in col_to_table:
        return col_to_table[c]
    else:
        warning('No col %s found' % c)

def getColsFromTable(t):
    if t in table_to_cols:
        return table_to_cols[t]
    else:
        warning('No table %s found' % t)


# fetch k in d if exists
def fetch(k, d):
    if k not in d:
        print d
        warning(('%s not found.' % k))
    return d[k]

def scanValuesRef(v, env):
    if isEnv2(env):
        _,alias = findAliasByName2(v['iu'], env)
    else:
        alias = findAliasByName(v['iu'], env)
    return genAssignment(alias)

def scanValuesConst(v):
    typ = v['value']['type']
    val = v['value']['value']
    return stringValue(typ, val)

def stringValue(typ, value):
    typ =  strType(typ)
    if typ == 'char':
        return '"%s"' % value
    else:
        return '%s:%s'%(value, typ)

def scanValuesExpr(v, expr, env):
    global join_list
    if 'left' in v and 'iu' in v['left']:
        if 'right' in v and 'iu' in v['right']:
            if expr == 'eq' and isEnv2(env):
                indx0,alias0 = findAliasByName2(v['left' ]['iu'], env)
                indx1,alias1 = findAliasByName2(v['right']['iu'], env)
                if indx0 != indx1:  # from different sides (left/right)
                    join_list.append([[v['left' ]['iu'],indx0], [v['right' ]['iu'],indx1]])
                    return None
    if 'right' in v:
        # rightValue = v['right']['iu']
        rightValue = scanValuesV(v['right'], env)
        if 'left' in v:
            # leftValue = v['left']['iu']
            leftValue = scanValuesV(v['left'], env)
            return genDyadic(expr, leftValue, rightValue)
            # return genAssignment('@%s(%s, %s) // %s(%s, %s)' % (expr,\
            #     getMap(leftValue), getMap(rightValue), expr,leftValue, rightValue))
        else:
            return genMonadic(expr, rightValue)
            # return genAssignment('@%s(%s) // %s' % (expr,\
            #     getMap(rightValue), rightValue))
    else:
        return genNiladic(expr)

def scanValuesCompare(v, m, env):
    op = m2p(m)
    if op:  # empty string -> False
        return scanValuesExpr(v, op, env)
    else:
        debug(m)
        unexpected('%s: len > 1 for having left and right children ??' % m)

def scanValuesCase(v, env):
    trueBlock = v['cases']
    elseBlock = scanValuesV(v['else'], env)
    if len(trueBlock) == 1:
        trueB = trueBlock[0]
        trueBCase  = scanValuesV(trueB['case'] , env)
        trueBValue = scanValuesV(trueB['value'], env)
        t0 = genMul(trueBCase        , trueBValue)
        t1 = genMul(genNot(trueBCase), elseBlock)
        return genPlus(t0, t1)
    else:
        unexpected('len != 1 (%d)' % len(trueBlock))

def scanValuesLike(v, env):
    args = fetch('arguments', v)
    t0 = scanValuesV(args[0], env)
    t1 = scanValuesV(args[1], env)
    return genLike(t0, t1)

def scanValuesBetween(v, env):
    args = fetch('arguments', v)
    t0 = scanValuesV(args[0], env)
    t1 = scanValuesV(args[1], env)
    t2 = scanValuesV(args[2], env)
    return genBetween(t0, t1, t2)

def scanLogicExpr(v, expr, env):
    args = fetch('arguments', v)
    temp = []
    for a in args:
        temp.append(scanValuesV(a, env))
    if len(temp) > 0:
        t0 = temp[0]
        for t in temp[1:]:
            t0 = genAssignment('@%s(%s, %s)' % (expr, t0, t))
        return t0
    else:
        unexpected('Empty logic expr found')

def scanValuesUnaryOp(v, expr, env):
    args = fetch('input', v)
    t0 = scanValuesV(args, env)
    if expr == 'not':
        return genNot(t0)
    else:
        unexpected('%s not impl. yet' % expr)

def scanValuesSet(v):
    if len(v) > 0:
        s0 = v[0]
        if s0['expression'] == 'const':
            typ = strType(s0['value']['type'])
        else:
            unexpected('Not supported')
        sets = [ scanValuesConst(s0) ]
        for s in v[1:]:
            if strType(s['value']['type']) == typ:
                sets.append(scanValuesConst(s))
            else:
                unexpected('mixed typ in sets: %s and %s' % (typ, s['expression']))
        return genAssignment('(%s):%s' % (stringList(sets), typ)) #gen literal
    else:
        unexpected('size 0 set found')

# member
def scanValuesQuantor(v, env):
    mode = fetch('mode', v)
    if mode == '=some':
        setValue = scanValuesSet(fetch('set', v))
        varValue = scanValuesV(fetch('value', v), env)
        return genMember(setValue, varValue)
    else:
        unexpected('New mode %s for quantor' % mode)

def scanValuesLookup(v, env):
    collates = v['collates']
    inputs   = v['input']
    values   = v['values']
    if len(collates) == len(inputs):
        if len(inputs) == 1:
            or_list = []
            for x in range(len(values)):
                left = findAliasByName(inputs[0]['iu'], env)  # var
                right= stringValue(values[x]['type'], values[x]['value']) #const
                or_list.append(genEq(left, right))
            if len(or_list) > 1:
                or_cell = genOr(or_list[0], or_list[1])
                for o in or_list[2:]:
                    or_cell = genOr(or_cell, o)
            elif len(or_list) == 1:
                or_cell = or_list[0]
            else:
                unexpected('or_list empty found')
            return or_cell
        elif len(inputs) > 1:
            pending('working on value look up')
        else: unexpected('len(inputs) == 0 found')

        # or_list = []
        # for x in range(0, len(values), len(inputs)):
        #     and_list = []
        #     for k in range(len(inputs)):
        #         left = findAliasByName(inputs[k]['iu'], env)  # var
        #         right= stringValue(values[x+k]['type'], values[x+k]['value']) #const
        #         and_list.append(genEq(left, right))
        #     if len(and_list) > 1:
        #         and_cell = genAnd(and_list[0], and_list[1])
        #         for a in and_list[2:]:
        #             and_cell = genAnd(and_cell, a)
        #     else:
        #         and_cell = and_list[0]
        #     or_list.append(and_cell)
        # if len(or_list) > 1:
        #     or_cell = genOr(or_list[0], or_list[1])
        #     for o in or_list[2:]:
        #         or_cell = genOr(or_cell, o)
        # else:
        #     or_cell = or_list[0]
        # return or_cell
    else:
        unexpected('size not match %d vs. %d' % (len(collates), len(inputs)))

def scanValuesSubstring(v, env):
    args = fetch('arguments', v)
    t0 = scanValuesV(args[0], env) # value
    t1 = scanValuesV(args[1], env) # start
    t2 = scanValuesV(args[2], env) # length
    # 1st generate range
    t3 = genRange(t2)
    t4 = genPlus(t3, t1)
    t5 = genSubString(t0, t4)
    return t5

def scanValuesExtractYear(v, env):
    args = fetch('input', v)
    t0   = scanValuesV(args, env)
    t1   = genDateYear(t0)
    return t1

# return anything input
def scanValuesIsnotnull(v, env):
    args = fetch('input', v)
    t0   = scanValuesV(args, env)
    return t0

def scanValuesV(v, env):
    expr = fetch('expression', v)
    if expr == 'iuref':
        return scanValuesRef(v, env)
    elif expr == 'const':
        return scanValuesConst(v)
    elif expr == 'comparison':
        return scanValuesCompare(v, v['mode'], env)
    elif isArithOps(expr):
        return scanValuesExpr(v, expr, env)
    elif isLogicOps(expr):
        return scanLogicExpr(v, expr, env)
    elif expr == 'case':
        return scanValuesCase(v, env)
    elif expr == 'like':
        return scanValuesLike(v, env)
    elif expr == 'between':
        return scanValuesBetween(v, env)
    elif expr == 'quantor':
        return scanValuesQuantor(v, env)
    elif expr == 'lookup':
        return scanValuesLookup(v, env)
    elif isUnaryOps(expr):
        return scanValuesUnaryOp(v, expr, env)
    elif expr == 'substring':
        return scanValuesSubstring(v, env)
    elif expr == 'extractyear':
        return scanValuesExtractYear(v, env)
    elif expr == 'isnotnull':
        return scanValuesIsnotnull(v, env)
    else:
        warning('%s is not supported yet' % expr)
    
def loadTables(d, t):
    cols  = []; alias = []; types = []
    for v in d:
        nam = v['iu'][0]
        typ = strType(v['iu'][1])
        vid = genLoadTableWithCols(nam, typ, t)
        cols.append(nam); alias.append(vid); types.append(typ)
    return cols,alias,types

def scanValuesI(v):
    nam = v['iu'][0]
    typ = v['iu'][1]
    vid = genLoadTableWithCols(nam,strType(typ),current_table) #value id
    insertMap(nam, vid)  # for compress
    linkCol2Table(nam, current_table)
    linkTable2Cols(current_table, nam)
    return nam

def scanValues(d, env):
    values = []
    for v in d:
        values.append(scanValuesV(v['value'], env))
    return values

# def scanValues(d):
#     for v in d:
#         if 'value' in v:
#             value_list.append(scanValuesV(v['value']))
#         elif 'iu' in v:
#             column_list.append(scanValuesI(v))
#         else:
#             warning("invalid value")

def scanAggr(d, env):
    debug('aggregates')
    keep_list = []
    env_names = [] ; env_types = []; env_alias = []
    for a in d:
        if a['operation'] == 'keep':
            keep_list.append(a)
    if len(keep_list) == 0: # implicit aggr
        for a in d:
            source = int(fetch('source', a)) # decide which value to sort
            op     = fetch('operation' , a)
            nam    = fetch('iu'        , a)[0]
            typ    = strType(fetch('iu'        , a)[1])
            env_names.append(nam)
            env_types.append(typ)
            if op == 'sum'    : env_alias.append(genSum(findAliasByIndex(source, env)))
            elif op == 'avg'  : env_alias.append(genAvg(findAliasByIndex(source, env)))
            elif op == 'max'  : env_alias.append(genMax(findAliasByIndex(source, env)))
            elif op == 'min'  : env_alias.append(genMin(findAliasByIndex(source, env)))
            elif op == 'count': env_alias.append(genCount(findAliasByIndex(0, env)))
            else: unexpected("Not impl. %s"%op)
    else: # explicit aggr
        t_list = []
        for k in keep_list: t_list.append(findAliasByIndex(int(k['source']), env))
        temp = '@list(' + stringList(t_list) + ')'
        t0 = genAssignment(temp)
        t1 = genGroup (t0)
        t2 = genKeys  (t1)
        t3 = genValues(t2)
        # t4 = genAssignment('@where(%s)' %prev_mask_t)
        # t5 = genAssignment('@each_right(@index,%s,%s)'%(t4,t3))
        debug(d)
        for a in d:
            source = int(fetch('source', a))
            op     = fetch('operation' , a)
            nam    = fetch('iu'        , a)[0]
            typ    = strType(fetch('iu'        , a)[1])
            env_names.append(nam)
            env_types.append(typ)
            if op == 'sum':
                a0 = genEachRight('@index', findAliasByIndex(source, env), t3)
                a1 = genEach     ('@sum'  , a0)
                a2 = genRaze     (a1)
                env_alias.append(a2)
            elif op == 'avg':
                a0 = genEachRight('@index', findAliasByIndex(source, env), t3)
                a1 = genEach     ('@avg'  , a0)
                a2 = genRaze     (a1)
                env_alias.append(a2)
            elif op == 'count':
                a0 = genEachRight('@index', findAliasByIndex(0, env), t3)
                a1 = genEach     ('@len'  , a0)
                a2 = genRaze     (a1)
                env_alias.append(a2)
            else:
                env_alias.append(t1) # keep
        # raise ValueError("stop")
    # print env_names
    # print env_types
    # print env_alias
    return encodeEnv('lambda', env_names, env_alias, env_types)

def scanHeader(d, env):
    new_env = scanMain(d['plan'], env)
    print new_env
    debug('result')
    head = d['header']
    size = len(head)/2 
    temp = ''
    # sort?
    if 'order' in new_env:
        final_order = new_env['order']
        for x in range(size):
            insertMap(head[x*2+1], genIndex(findAliasByName(head[x*2+1], new_env),final_order))
    # build col names
    for x in range(size):
        temp = temp + '`%s' % head[x*2]
    a0 = genAssignment(temp)
    # build col values
    if size == 1:
        temp = '@enlist(%s)' % findAliasByName(head[1], new_env)
    else:
        temp = '@list('
        for x in range(size):
            if x > 0:
                temp = temp + ','
            temp = temp + findAliasByName(head[x*2+1], new_env)
        temp = temp + ')'
    a1 = genAssignment(temp)
    a2 = genTable(a0, a1)
    return None

def scanRestrictionCell(d, cols):
    attr = fetch('attribute', d)
    mode = fetch('mode'     , d)
    if 'value' in d:
        col_name = cols[int(attr)]
        if 'value2' in d:
            a0 = genAssignment('@%s(%s,%s)'%(m2p(mode[0]),col_name,scanValuesConst(d['value'])))
            a1 = genAssignment('@%s(%s,%s)'%(m2p(mode[1]),col_name,scanValuesConst(d['value2'])))
            return genAnd(a0, a1)
        else:
            return genAssignment('@%s(%s,%s)'%(m2p(mode[0]),col_name,scanValuesConst(d['value'])))
    else:
        unexpected("Not impl.")

def scanRestrictions(d, cols):
    debug('restrictions')
    cells = []
    for c in d:
        cells.append(scanRestrictionCell(c, cols))
    if len(cells) == 1:
        return cells[0]
    elif len(cells) == 2:
        return genAnd(cells[0], cells[1])
    elif len(cells) > 2:
        c0 = cells[0]
        for c in cells[1:]:
            c0 = genAnd(c0, c)
        return c0
    else:
        return None #no restrictions

def actionCompress(vid, cols):
    alias = []
    for c in cols:
        a = genCompress(vid, c)
        alias.append(a)
    return alias

def updateEnvWithMask(mask, env, func_name='lambda'):
    alias = actionCompress(mask, env['cols_a'])
    return encodeEnv(func_name, env['cols_n'], alias, env['cols_t'], mask) #update alias

# def actionCompress(vid, cols):
#     for c in cols:
#         a = genCompress(vid, getMap(c))
#         insertMap(c, a)

def scanByFunc(d, name, foo):
    if name in d:
        return foo(d[name])
    else:
        unexpected('%s not found' % name)

def scanGroupby(d, env):
    debug('groupby')
    scanValues(d['values'], env)
    return scanAggr(d['aggregates'], env)

def encodeEnv(table_name, cols_names, cols_alias, cols_types, mask=''):
    return {
        "table"    : table_name,
        "cols_n"   : cols_names,
        "cols_a"   : cols_alias,
        "cols_t"   : cols_types,
        "mask"     : mask
    }

def scanTablescan(d, env):
    from_table = d['from']
    debug('tablescan for %s' % from_table)
    current_table  = genLoadTable(from_table)
    columns, alias, types = loadTables(d['values'], current_table)
    residuals      = scanResiduals(\
                        d['residuals'],\
                        encodeEnv(from_table, columns, alias, types)) \
                        if 'residuals' in d else None
    mask           = scanRestrictions(d['restrictions'], alias) if 'restrictions' in d else None
    if residuals is not None:
        if mask is not None:
            mask = genAnd(mask, residuals)
        else:
            mask = residuals
    alias          = actionCompress(mask, alias) if mask is not None else alias  #??
    return encodeEnv(from_table, columns, alias, types, mask)

# def scanTablescan(d):
#     global table_list, column_list, current_table, \
#            prev_mask_t, current_resz, left_join_mask
#     column_list = []
#     from_table = fetch('from', d)
#     debug('tablescan for %s' % from_table)
#     current_table = genLoadTable(from_table)
#     scanByFunc(d, 'values', scanValues)
#     prev_mask_t = scanByFunc(d, 'restrictions', scanRestriction)
#     if left_join_mask != '':
#         if prev_mask_t == '': prev_mask_t = left_join_mask
#         else: prev_mask_t = genAssignment('@and(%s,%s); // left_join_mask'%(prev_mask_t,left_join_mask))
#     if prev_mask_t != '':
#         actionCompress(prev_mask_t, column_list)
#     table_list.append({current_table: column_list})
#     # q4: leftsemijoin
#     if 'residuals' in d:
#         current_resz = scanResiduals(d['residuals'])

# Copied from scanRestrictions
def scanResiduals(d, env):
    debug('residuals')
    cells = []
    for c in d:
        cells.append(scanValuesV(c, env))
        # cells.append(scanResidualsCell(c))
    if len(cells) == 1:
        return cells[0]
    elif len(cells) == 2:
        return genAnd(cells[0], cells[1])
    elif len(cells) > 2:
        c0 = cells[0]
        for c in cells[1:]:
            c0 = genAnd(c0, c)
        return c0
    else:
        return None #no restrictions

def scanResidualsCell(d):
    expr = fetch('expression', d)
    mode = fetch('mode', d)
    if expr == 'comparison':
        leftValue  = scanValuesV(d['left'])
        rightValue = scanValuesV(d['right'])
        return genAssignment('@%s(%s,%s)'%(m2p(mode),leftValue,rightValue))
    else:
        unexpected("Not impl. %s" % expr)


def scanSort(d, env):
    debug('sort')
    criterion = fetch('criterion', d)
    names = []
    bools = []
    for c in criterion:
        v = c['value']
        if v['expression'] == 'iuref':
            names.append(findAliasByName(v['iu'][0], env))
            bools.append(1 if c['descending']==False else 0)
    list_order  = genAssignment('@list(%s)'%stringList(names))
    if len(bools) == 1:
        final_order = genAssignment('@sort(%s,%s:bool)'%(list_order,stringList(bools)))
    else:
        final_order = genAssignment('@sort(%s,(%s):bool)'%(list_order,stringList(bools)))
    return addOrderToEnv(final_order, env)

"""
compact the code later
"""
def scanSemijoin(d, env2, side):
    debug('%s semijoin <----' % side)
    left_env = env2[0];  left_mask  = left_env ['mask']
    right_env= env2[1];  right_mask = right_env['mask']
    left_col, right_col = scanCondition(d['condition'], 'hash', env2)
    if side == 'left':
        # left side
        # y0 = findAliasByName(left_col, left_env)
        # if left_mask: y0 = genCompress(left_mask, left_col)
        y0 = genCompress(left_mask, left_col) if left_mask else left_col
    else:
        # right side
        # y0 = findAliasByName(right_col, right_env)
        # if right_mask: y0 = genCompress(right_mask, y0)
        y0 = genCompress(right_mask, right_col) if right_mask else right_col
    y1 = genValues (y0)
    y2 = genKeys   (y0)
    y3 = genLength (y2)
    y4 = genVector (y3, '0:bool') # return y4 for the left side
    y5 = genIndexA (y4, y1, '1:bool')
    if side == 'left':
        w0 = genAnd(right_mask, y4) if right_mask else y4
        return updateEnvWithMask(w0, left_env)
    else:
        # merge the results from both sides
        w0 = genAnd(left_mask, y4) if left_mask else y4  #new mask for left table
        return updateEnvWithMask(w0, right_env)

# def scanSemijoin(d, env2, side):
#     debug('%s semijoin <----' % side)
#     left_env = env2[0]
#     right_env= env2[1]
#     # join_cond= scanCondition(d['condition'], 'semijoin', appendEnv2(left_env, right_env) )
#     cond,_ = scanCondition(d['condition'], 'hash', env2)
#     if len(cond) == 1:
#         cols = [cond[0][0][0], cond[0][1][0]]
#     else:
#         unexpected('cond size %d not impl.' % (len(cond)))
#     from_indx0 = cond[0][0][1]
#     from_indx1 = cond[0][1][1]
#     # TODO: need to check which tables these cols belong to
#     if from_indx0 == 0:
#         left_col = cols[0]
#         right_col= cols[1]
#     else:
#         left_col = cols[1] #l_orderkey
#         right_col= cols[0] #o_orderkey
#     left_mask = left_env['mask']
#     right_mask= right_env['mask']
#     if side == 'left':
#         # handle left
#         y0 = findAliasByName(left_col, left_env)
#         if left_mask:
#             y0 = genCompress(left_mask, y0)
#     else:
#         # handle right
#         y0 = findAliasByName(right_col, right_env)
#         if right_mask:
#             y0 = genCompress(right_mask, y0)
#     y1 = genValues (y0)
#     y2 = genKeys   (y0)
#     y3 = genLength (y2)
#     y4 = genVector (y3, '0:bool') # return y4 for the left side
#     y5 = genIndexA (y4, y1, '1:bool')
#     if side == 'left':
#         w0 = genAnd(right_mask, y4) if right_mask else y4
#         return updateEnvWithMask(w0, left_env)
#     else:
#         # merge the results from both sides
#         w0 = genAnd(left_mask, y4) if left_mask else y4  #new mask for left table
#         return updateEnvWithMask(w0, right_env)

def scanLeftsemijoin(d, env2):
    return scanSemijoin(d, env2, 'left')

def scanRightsemijoin(d, env2):
    return scanSemijoin(d, env2, 'right')

def fetchID(d):
    if d['expression'] == 'iuref':
        return d['iu']
    else:
        unexpected('not handled yet.')

def getConstList(values):
    value_list = []
    for v in values:
        value_list.append(stringValue(v['type'], v['value']))
    return value_list

def getAliasList(inputs, consts, env2):
    left = []; right = []
    for c,n in enumerate(inputs):
        indx, alias = findAliasByName2(fetchID(n), env2)
        if indx == 0: left.append([alias, consts[c]])
        else: right.append([alias, consts[c]])
    def getAndList(v):
        if len(v) == 1: t0 = genEq(v[0][0], v[0][1])
        elif len(v) > 1:
            t0 = genEq(v[0][0], v[0][1])
            for x in v[1:]:
                t1 = genEq(x[0], x[1])
                t0 = genAnd(t1, t0)
        else: unexpected('wrong in len(v) = %d' % len(v))
        return t0
    x0 = getAndList(left)  # left
    y0 = getAndList(right) # right
    # starting join from both sides
    w0 = genWhere(x0)    ; w1 = genWhere(y0)
    w2 = genDuplicate(w0); w3 = genReplicate(w1)
    return [w2, w3]

keycol_list = [
  'n_nationkey' , 's_suppkey'  , 'p_partkey'   , 'c_custkey', 'l_orderkey', 'o_orderkey',
  'n_nationkey2', 's_suppkey2'
]

def joinColumnsGeneral(left_alias, left_name, right_alias, right_name):
    todo('general join not impl.')
    return ['<none>', '<none>']

def joinColumns(left_alias, left_name, right_alias, right_name, isList=False):
    global keycol_list
    if isList:
        todo('list join (multi-column join) not impl.')
        # joinColumnsGeneral
        return ['<none>', '<none>']
    else:
        # left side is a keyed column
        if left_name in keycol_list:
            e0 = genEnum(left_alias, right_alias)
            t0 = genValues(e0) # left_index
            t1 = genKeys(e0)
            t2 = genLt(t0, genLength(t1))
            t3 = genWhere(t2)  # right_index
            return [t0, t3]
        else:
            # right side is a keyed column
            if right_name in keycol_list:
                e0 = genEnum(right_alias, left_alias)
                t0 = genValues(e0) # right_index
                t1 = genKeys(e0)
                t2 = genLt(t0, genLength(t1))
                t3 = genWhere(t2)  # left_index
                return [t3, t0]
            else:
                # q15
                # joinColumnsGeneral
                # unexpected('Both sides are non-key (%s = %s)' % (left_name, right_name))
                return joinColumnsGeneral(left_alias, left_name, right_alias, right_name)

def findExprFromSide(d, env2, isCollect=False):
    expr = d['expression']
    if expr == 'comparison':
        mode = d['mode']; left = [] ; right = []
        name0 = fetchID(d['left' ])
        name1 = fetchID(d['right'])
        side0, alias0 = findAliasByName2(name0, env2)
        side1, alias1 = findAliasByName2(name1, env2)
        if side0 == side1:
            unexpected('pred. vars from both sides')
        # q2: 'is'
        if mode == '=' or mode == 'is':
            # print 'side0 = %d, side1 = %d' % (side0, side1)
            # print 'alias0 = %s, alias1 = %s' % (alias0, alias1)
            if isCollect:
                return [alias0, alias1] if side0 == 0 else [alias1, alias0]
            if side0 == 0:
                return joinColumns(alias0, name0, alias1, name1)
            else:
                return joinColumns(alias1, name1, alias0, name0)
            # key_list = ['o_orderkey', 'p_partkey']
            # special check with pre-built key/fkey
            x0 = genMember(alias0, alias1) # pending...
            x1 = genWhere(x0)  # for alias1

            y0 = genIndexOf(alias1, alias0)
            y1 = genLength(alias1)
            y2 = genLt(y0, y1)
            y3 = genWhere(y2) # for alias0
            # if any x_ or y_ is key, one can be ignored
            unexpected('Need to fix later')
        elif mode == '>' or mode == '<' or mode == '<>':
            if side0 == 0:
                return joinColumnsGeneral(alias0, name0, alias1, name1)
            else:
                return joinColumnsGeneral(alias1, name1, alias0, name0)
        else: unexpected('cond (%s) not handled' % mode)
    elif expr == 'and':  #q7, multiple column join
        left_list = [] ; right_list = []
        for d0 in d['arguments']:
            new_left, new_right = findExprFromSide(d0, env2, True)
            left_list.append(new_left)
            right_list.append(new_right)
        t0 = genList(stringList(left_list))
        t1 = genList(stringList(right_list))
        return joinColumns(t0, 'left_list', t1, 'right_list', True)
    elif expr == 'or': #q19
        warning('think about q19: and & or in condition')
    elif expr == 'lookup':
        const_list = getConstList(d['values'])
        size = 0; indx0 = ''; indx1 = ''
        while size < len(const_list):
            t0, t1 = getAliasList(d['input'], const_list[size:], env2)
            indx0 = genCons(indx0, t0) if indx0 else t0
            indx1 = genCons(indx1, t1) if indx0 else t1
            size = size + len(d['input'])
        # unique [indx0, indx1]
        indx = genUnique(genList('%s,%s' % (indx0, indx1)))
        left  = genIndex(indx0, indx)
        right = genIndex(indx1, indx)
        return left, right
    else:
        unexpected('not handled (%s) when looking for side info.' % expr)

def scanCondition(d, tag, env):
    if tag == 'hash':
        # env -> 2 env
        return findExprFromSide(d, env)
    elif tag == 'index':
        # index join == hash join ??
        return findExprFromSide(d, env)
    elif tag == 'select':
        # q18
        return scanValuesV(d, env)
    else:
        unexpected("unknown tag %s found in condition" % tag)

# join condition
# def scanCondition(d, cond, env):
#     debug('condition for %s' % cond)
#     if cond == 'index' or cond == 'semijoin':
#         if fetch('expression', d) == 'comparison' and fetch('mode', d) == '=':
#             if d['left']['expression'] == 'iuref' and d['right']['expression'] == 'iuref':
#                 return [d['left']['iu'], d['right']['iu'] ]
#             else:
#                 unexpected('No...')
#         else:
#             unexpected('...')
#     elif cond == 'hash':
#         global join_list
#         join_list = []
#         t0 = scanValuesV(d, env)
#         return join_list, t0
#     elif cond == 'select':
#         return scanValuesV(d, env)
#     else:
#         unexpected('cond %s not handled' % cond)

# table x,y
# def updateTablesByEnum(x, y, e0):
#     cols_x = getColsFromTable(x)
#     cols_y = getColsFromTable(y)
#     t0 = genValues(e0)
#     for c in cols_x:
#         val = getMap(c)

# def updateTableWithIndex(x, ind):
#     debug('Updating table: %s, with index %s' % (x,ind))
#     cols_x = getColsFromTable(x)
#     for c in cols_x:
#         insertMap(c, genIndex(getMap(c), ind))

def updateEnvWithCols(a, env):
    new_env = copy.deepcopy(env)
    new_env['cols_n'] = a
    return new_env

def updateEnvWithAlias(a, env):
    new_env = copy.deepcopy(env)
    new_env['cols_a'] = a
    return new_env

def updateEnvWithIndex(ind, env):
    alias = env['cols_a']
    new_a = []
    for a in alias:
        new_a.append(genIndex(a, ind))
    return updateEnvWithAlias(new_a, env)

# TODO: check duplicated column names
def combineEnv2(env1, env2, is_new_name=True):
    return encodeEnv(\
             getEnvTable(env1)+'+'+getEnvTable(env2) if is_new_name else getEnvTable(env1), \
             getEnvName (env1)  +  getEnvName (env2), \
             getEnvAlias(env1)  +  getEnvAlias(env2), \
             getEnvType(env1)   +  getEnvType(env2)   \
           )

# def appendEnv2(env1, env2):
#     return [env1, env2]

def updateTableWithEnum(e0, key_env, value_env):
    t0 = genKeys(e0)  #take all keys? assuming now
    t1 = genValues(e0)
    return combineEnv2(updateEnvWithIndex(t1, key_env), value_env)

def updateTableWithAntiEnum(e0, env):
    t0 = genValues(e0)
    t1 = genLength(e0)
    t2 = genGe(t0, t1)
    return updateEnvWithMask(t2, env)

def scanJoinIndex(d, env2):
    debug('join index <----')
    left_col, right_col = scanCondition(fetch('condition', d), 'index', env2)
    e0 = genEnum(left_col, right_col)
    return updateTableWithEnum(e0, env2[0], env2[1])

# def scanJoinIndex(d, env2):
#     debug('join index <----')
#     cond = scanCondition(fetch('condition', d), 'index', env2)
#     if len(cond) == 2:
#         key_list = ['o_orderkey', 'p_partkey', ]
#         if cond[0] in key_list  : key_index = 0
#         elif cond[1] in key_list: key_index = 1
#         else:
#             # warning('No key found in join index')
#             k0,x0 = findAliasByName2(cond[0], env2)
#             k1,x1 = findAliasByName2(cond[1], env2)
#             e0 = genEnum(x0, x1)
#             return updateTableWithEnum(e0, env2[k0], env2[k1])
#         _, key_value   = findAliasByName2(cond[key_index], env2)
#         value_index    = 1 - key_index
#         _, value_value = findAliasByName2(cond[value_index], env2)
#         e0 = genEnum(key_value, value_value)
#         t0 = genValues(e0)
#         # from_table = findTableByColumn2(cond[1],left_env,right_env)
#         return updateTableWithEnum(t0, env2[0], env2[1])
#         # updateTablesByEnum(getTableFromCol(cond[0]), getTableFromCol(cond[1]), e0)
#     else:
#         unexpected('No len != 2')

"""
Q1: hash join means key/fkey relation?
Q2: key (left) and fkey(right)?
"""
def scanJoinHash(d, env2, msg='hash join'):
    debug('%s <----' % msg)
    left_col,right_col = scanCondition(d['condition'], 'hash', env2)
    e0 = genEnum(left_col, right_col)
    return updateTableWithEnum(e0, env2[0], env2[1])

# def scanJoinHash(d, env2, msg='hash join'):
#     debug('%s <----' % msg)
#     cond,mask = scanCondition(d['condition'], 'hash', env2)
#     debug('query = ' + str(cond))
#     if len(cond) == 0:
#         # not an actual join: 1) check cardinality (1:n or n:1)
#         left_child_size  = int(d['left' ]['cardinality'])
#         right_child_size = int(d['right']['cardinality'])
#         if left_child_size == 1:
#             return updateEnvWithMask(mask, env2[1]) #update with right env
#         else:
#             unexpected('check join hash with len(cond) == 0')
#     elif len(cond) == 1:
#         debug(cond)
#         from_name0 = cond[0][0][0]; from_indx0 = cond[0][0][1]
#         from_name1 = cond[0][1][0]; from_indx1 = cond[0][1][1]
#         if from_indx0 == 0 and from_indx1 == 1:
#             e0 = findAliasByName(from_name1, env2[1]) # from right hand side (enum)
#             t0 = genValues(e0)
#             return updateTableWithEnum(t0, env2[0], env2[1])
#         else:
#             e0 = findAliasByName(from_name0, env2[1])
#             t0 = genValues(e0)
#             return updateTableWithEnum(t0, env2[1], env2[0]) # this order 'env2[1]' 'env2[0]' ?
#             # unexpected('TODO: order reversed %d %d' % (from_indx0, from_indx1))
#     elif len(cond) > 1: #q7
#         # debug(str(cond))
#         print 'query'
#         left_names = []; right_names = []  # left key, right value?
#         for x in range(len(cond)):
#             from_name0 = cond[x][0][0]; from_name1 = cond[x][1][0]
#             from_indx0 = cond[x][0][1]; from_indx1 = cond[x][1][1]
#             if from_indx0 == 0 and from_indx1 == 1:
#                 left_names.append(findAliasByName(from_name0, env2[0]))
#                 right_names.append(findAliasByName(from_name1, env2[1]))
#             else:
#                 left_names.append(findAliasByName(from_name1, env2[0]))
#                 right_names.append(findAliasByName(from_name0, env2[1]))
#         left_names  = genAssignment(stringList(left_names))
#         right_names = genAssignment(stringList(right_names))
#         e0 = genEnum(left_names, right_names)
#         return updateTableWithEnum(e0, env2[0], env2[1])  # env2 0/1 order matters?
#     else: unexpected('TODO: len(cond) = %d'%len(cond))
#     pending('working on join hash')

def scanJoin(d, env2):
    method = fetch('method', d)
    if method == 'hash':
        return scanJoinHash (d, env2)
    elif method == 'indexnl':
        return scanJoinIndex(d, env2)
    elif method == 'bnl':
        return scanJoinHash (d, env2, 'bnl join')  # same as hash join?
    else:
        unexpected('Unknown method %s' % method)

"""
return: new + old env
"""
def scanMap(d, env):
    debug('map')
    vals = fetch('values', d)
    env_names = []; env_alias = []; env_types = []
    for v in vals:
        env_names.append(v['iu'][0])
        env_types.append(strType(v['iu'][1]))
        env_alias.append(scanValuesV(v['value'], env))
    return combineEnv2(encodeEnv('lambda_map', env_names, env_alias, env_types), env)

def findKeepList(d):
    r = []
    for x in d:
        if x['operation'] == 'keep': r.append(x['iu'][0])
    return r

def findKeepList2(d1, d2):
    r = []
    r.append(d1)
    r.append(d2)
    return r

def addEnvValues(names, alias, types, d, env, indx):
    for a in d:
        source = int(a['source'])
        if source == 4294967295:  # special 'count' #q13
            continue
        nam = a['iu'][0]
        typ = strType(a['iu'][1])
        if indx == None:
            names.append(nam)
            alias.append(findAliasByIndex(source, env))
            types.append(typ)
        else:
            names.append(nam)
            alias.append(genRaze(genIndex(nam, indx)))
            types.append(typ)

def scanGroupjoin(d, env2):
    debug('group join')
    left_keys  = d['leftKey']
    right_keys = d['rightKey']
    if len(left_keys) == 1:
        # it seems left_col and right_col are useless
        # since 'source' has indicated the name of keys from both sides 
        left_col  = left_keys [0]['iu']
        right_col = right_keys[0]['iu']
        t0 = findAliasByName(left_col , env2[0])
        t1 = findAliasByName(right_col, env2[1])
        e0 = genEnum(t0, t1) # if t0: key; t1 is fkey
        w0 = genValues(e0)
        w1 = genGroup(w0)  # dict<key, value>
        w2 = genKeys(w1)   # key on the left should be kept
        w3 = genValues(w1) # value
        env_names =[] ; env_alias = []; env_types = []
        addEnvValues(env_names, env_alias, env_types, d['leftAggregates' ],env2[0], None)
        addEnvValues(env_names, env_alias, env_types, d['rightAggregates'],env2[1], w3)
        return encodeEnv('lambda_groupjoin',env_names, env_alias, env_types)
    else:
        unexpected('keys > 1 found in group join')

def addTarget2Env(d, env):
    env_names = [] ; env_alias = []; env_types = []
    for x in d:
        source_name = x['source'][0] if isList(x['source']) else x['source']
        env_names.append(x['target'][0])
        env_alias.append(genAssignment(findAliasByName(source_name, env)))
        env_types.append(strType(x['target'][1]))
    return combineEnv2(env, encodeEnv('handleTarget', env_names, env_alias, env_types), False)

def scanTempscan(d, env):
    debug('temp scan')
    global id_to_env
    source = d['source']
    if isDict(source):
        if source['operator'] == 'temp':
            temp_env  = scanMain(source['input'], env)
            temp_env2 = addTarget2Env(d['output'], temp_env) # <-- d
            # id_to_env = { source['operatorId'] : temp_env }
            id_to_env[source['operatorId']] = temp_env
            return temp_env2
        else:
            unexpected('operator %s not impl. in temp scan' % source['operator'])
    else:
        temp_env  = id_to_env[source]
        temp_env2 = addTarget2Env(d['output'], temp_env)
        return temp_env2

def scanAntijoin(d, env2, side):
    debug('%s anti join <----' % side)
    left_col, right_col = scanCondition(d['condition'], 'hash', env2)
    if side == 'right':
        e0 = genEnum(right_col, left_col)
    else:
        e0 = genEnum(left_col, right_col)
    return updateTableWithEnum(e0, env2[0], env2[1])


# def scanAntijoin(d, env2, side):
#     debug('%s anti join <----' % side)
#     cond,mask = scanCondition(d['condition'], 'hash', env2)
#     debug(str(cond))
#     if len(cond) == 1:
#         method = d['method']
#         if method == 'hash':
#             debug(cond)
#             from_name0 = cond[0][0][0]; from_indx0 = cond[0][0][1]
#             from_name1 = cond[0][1][0]; from_indx1 = cond[0][1][1]
#             if from_indx0 == 0 and from_indx1 == 1:
#                 t0 = findAliasByName(from_name0, env2[0])
#                 t1 = findAliasByName(from_name1, env2[1])
#                 pending('unchecked')
#             else:
#                 t0 = findAliasByName(from_name0, env2[1]) # fkey for 'right'
#                 t1 = findAliasByName(from_name1, env2[0]) # key  for 'right'
#                 # checked with q16
#             if side == 'right':
#                 e0 = genEnum(t1, t0)
#                 return updateTableWithAntiEnum(e0, env2[1])
#             else: # 'left'
#                 e0 = genEnum(t0, t1)
#                 return updateTableWithAntiEnum(e0, env2[0])
#         else:
#             unexpected('special method %s found in right anti join' % method)
#     else:
#         unexpected('cond has %d keys for join' % len(cond))

def scanLeftantijoin(d, env2):
    return scanAntijoin(d, env2, 'left')

def scanRightantijoin(d, env2):
    return scanAntijoin(d, env2, 'right')

def scanSelect(d, env):
    debug('select')
    mask = scanCondition(d['condition'], 'select', env)
    return updateEnvWithMask(mask, env)

# do nothing ...
def scanEearlyprobe(d, env):
    debug('early probe')
    return scanMain(d['input'], env)

def scanGroupbyscan(d, env):
    debug('group by scan')
    source = d['source']
    output = d['output']
    source_env = id_to_env[source]
    # printEnv(source_env)
    old_cols = getEnvName(source_env)
    for item in output:
        old_key = item['source'][0]
        new_key = item['target'][0]
        print old_key
        for c,v in enumerate(old_cols):
            if v == old_key:
                old_cols[c] = new_key
                break
    return updateEnvWithCols(old_cols, source_env)

# read source from the result of join (left-side)
def scanMagic(d, env):
    debug('magic')
    global id_to_env
    op = d['operator']
    if op == 'groupby':
        id_to_env[d['operatorId']] = scanGroupby(d, env)
    else:
        unexpected('(%s) not impl. in magic' % op)

def scanMain(d, env):
    if 'plan' in d:
        return scanHeader(d, env)
    elif 'operator' in d:
        op = d['operator']
        env1 = {}; env2 = []
        if 'input' in d:
            env1 = scanMain(d['input'], env)
        elif 'left' in d and 'right' in d:
            env2.append(scanMain(d['left'], env))
            if 'magic' in d:
                if op == 'join': scanMagic(d['magic'], env2[0])  # q17
                else: unexpected('Unhandled magic in (%s)' % op)
            env2.append(scanMain(d['right'], env))
        if   op == 'tablescan'    : return scanTablescan    (d, env )  # 0 child
        elif op == 'tempscan'     : return scanTempscan     (d, env)   # 0 child
        elif op == 'groupbyscan'  : return scanGroupbyscan  (d, env)   # 0 children
        elif op == 'groupby'      : return scanGroupby      (d, env1)  # 1 child
        elif op == 'sort'         : return scanSort         (d, env1)  # 1 child
        elif op == 'map'          : return scanMap          (d, env1)  # 1 child
        elif op == 'select'       : return scanSelect       (d, env1)  # 1 child
        elif op == 'earlyprobe'   : return scanEearlyprobe  (d, env1)  # 1 child
        elif op == 'join'         : return scanJoin         (d, env2)  # 2 children
        elif op == 'groupjoin'    : return scanGroupjoin    (d, env2)  # 2 children
        elif op == 'leftantijoin' : return scanLeftantijoin (d, env2)  # 2 children
        elif op == 'rightantijoin': return scanRightantijoin(d, env2)  # 2 children
        elif op == 'leftsemijoin' : return scanLeftsemijoin (d, env2)  # 2 children
        elif op == 'rightsemijoin': return scanRightsemijoin(d, env2)  # 2 children
        else:
            unexpected("Not impl. %s" % op)
    else:
        unexpected("Check scanMain")


def main():
    if len(sys.argv) != 2:
        print "Usage: python buildAST.py opt/<file>"
        sys.exit(1)
    name = sys.argv[1]
    plan = json.loads(readLines(name, ''))
    genModuleBegin('default')
    scanMain(plan, {})
    genModuleEnd()

if __name__ == '__main__':
    start = time.time()
    main()
    sys.stderr.write('Elapsed time: %g ms\n' % (1000*(time.time() - start)))

