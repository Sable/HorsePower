import json, sys, time, copy

from codegen import *
from codelib import *
from collections import OrderedDict
from datetime import date, timedelta

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
standard_date = date(2000,1,1) # standard date: 2000.01.01

primary_key   = {}
foreign_key   = {}
mapping_key   = {}
table_types   = {} # char to sym
isOpSubstring = False
vec_side_or   = {}

"""
Entry/Main
"""
def main():
    if len(sys.argv) != 2:
        print "Usage: python genIR.py opt/<file>"
        sys.exit(1)
    name = sys.argv[1]
    plan = json.loads(readLines(name, ''))
    initRelations()
    scanMain(plan, {})
    # debug= True
    debug= False
    if debug:
        # printVarNum()
        # selected_columns = traverseUse()
        # print selected_columns
        # printAllCode([], debug=True)
        pass
    else:
        selected_columns = traverseUse()
        printAllCode(selected_columns)

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
                else: unexpected('Unhandled magic in the operator (%s)' % op)
            env2.append(scanMain(d['right'], env))
        if   op == 'tablescan'    : rtn = scanTablescan    (d, env )  # 0 child
        elif op == 'tempscan'     : rtn = scanTempscan     (d, env )  # 0 child
        elif op == 'groupbyscan'  : rtn = scanGroupbyscan  (d, env )  # 0 child
        elif op == 'groupby'      : rtn = scanGroupby      (d, env1)  # 1 child
        elif op == 'sort'         : rtn = scanSort         (d, env1)  # 1 child
        elif op == 'map'          : rtn = scanMap          (d, env1)  # 1 child
        elif op == 'select'       : rtn = scanSelect       (d, env1)  # 1 child
        elif op == 'earlyprobe'   : rtn = scanEearlyprobe  (d, env1)  # 1 child
        elif op == 'join'         : rtn = scanJoin         (d, env2)  # 2 children
        elif op == 'groupjoin'    : rtn = scanGroupjoin    (d, env2)  # 2 children
        elif op == 'leftantijoin' : rtn = scanLeftantijoin (d, env2)  # 2 children
        elif op == 'rightantijoin': rtn = scanRightantijoin(d, env2)  # 2 children
        elif op == 'leftsemijoin' : rtn = scanLeftsemijoin (d, env2)  # 2 children
        elif op == 'rightsemijoin': rtn = scanRightsemijoin(d, env2)  # 2 children
        # leftouterjoin
        # leftmarkjoin
        else:
            unexpected("Not impl. %s" % op)
        return setProperty(d, rtn) # set cardinality
    else:
        unexpected("Check scanMain")

"""
Scan functions
"""
def scanValuesRef(v, env):
    if isEnv2(env):
        _,alias = findAliasByName2(v['iu'], env)
    else:
        alias = findAliasByName(v['iu'], env)
    return alias
    # return genCopy(alias)

def scanValuesConst(v, withType=True):
    typ = v['value']['type']
    val = v['value']['value']
    return stringValue(typ, val, withType)

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
            return genDyadic(genArithOps(expr), leftValue, rightValue)
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
            # t0 = genAssignment('@%s(%s, %s)' % (expr, t0, t))
            t0 = genDyadic(expr, t0, t)
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
        sets = [ scanValuesConst(s0, False) ]
        for s in v[1:]:
            if strType(s['value']['type']) == typ:
                sets.append(scanValuesConst(s, False))
            else:
                unexpected('mixed typ in sets: %s and %s' % (typ, s['expression']))
        if typ == 'sym':
            if isOpSubstring:
                return genLiteral('(%s):str' % stringList(sets))
            else:
                return genLiteral('%s:sym' % stringList(sets,''))
        else:
            return genLiteral('(%s):%s' % (stringList(sets), typ)) #gen literal
        # return genAssignment('(%s):%s' % (stringList(sets), typ)) #gen literal
    else:
        unexpected('size 0 set found')

# member
def scanValuesQuantor(v, env):
    global isOpSubstring
    mode = fetch('mode', v)
    if mode == '=some':
        isOpSubstring = fetch('expression', fetch('value', v)) == 'substring' #chf
        setValue = scanValuesSet(fetch('set', v))
        isOpSubstring = False #chf
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
    else:
        unexpected('size not match %d vs. %d' % (len(collates), len(inputs)))

def scanValuesSubstring(v, env):
    args = fetch('arguments', v)
    t0 = scanValuesV(args[0], env) # value
    t1 = scanValuesV(args[1], env) # start
    t2 = scanValuesV(args[2], env) # length
    # 1st generate range
    #t3 = genRange(t2)
    #t4 = genPlus(t3, t1)
    start  = t1.split(':')[0]; start_type  = t1.split(':')[1]
    length = t2.split(':')[0]; length_type = t2.split(':')[1]
    if start_type == length_type and start_type == 'i64':
        t5 = genSubString(t0, '(%s,%s):i64'%(start,length))
    else:
        print t1, t2
        print 'type: %s, %s' % (start_type, length_type)
        pending('Need to check substring')
    return t5

def scanValuesExtractYear(v, env):
    args = fetch('input', v)
    t0   = scanValuesV(args, env)
    t1   = genDateYear(t0)
    return t1

# return anything input
def scanValuesIsnotnull(v, env):
    # args = fetch('input', v)
    # t0   = scanValuesV(args, env)
    # return t0
    return None

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

def scanAggr(d, values):
    debug('aggregates')
    keep_list = []
    env_names = [] ; env_types = []; env_alias = []
    for a in d:
        if a['operation'] == 'keep' or a['operation'] == 'any':
            keep_list.append(a)
    if len(keep_list) == 0: # implicit aggr
        for a in d:
            source = int(fetch('source', a)) # decide which value to sort
            op     = fetch('operation' , a)
            nam    = fetch('iu'        , a)[0]
            typ    = strType(fetch('iu'        , a)[1])
            env_names.append(nam)
            env_types.append(typ)
            if op == 'sum'    : env_alias.append(genSum(values[source]))
            elif op == 'avg'  : env_alias.append(genAvg(values[source]))
            elif op == 'max'  : env_alias.append(genMax(values[source]))
            elif op == 'min'  : env_alias.append(genMin(values[source]))
            elif op == 'count': env_alias.append(genCount(values[0]))
            else: unexpected("Not impl. %s"%op)
    else: # explicit aggr
        t_list = []
        # for k in keep_list: t_list.append(findAliasByIndex(int(k['source']), env))
        for k in keep_list: t_list.append(values[int(k['source'])])
        t0 = genList  (t_list)
        t1 = genGroup (t0)
        t2 = genKeys  (t1)
        t3 = genValues(t1)
        # t4 = genAssignment('@where(%s)' %prev_mask_t)
        # t5 = genAssignment('@each_right(@index,%s,%s)'%(t4,t3))
        # debug(d)
        for a in d:
            source = int(fetch('source', a))
            op     = fetch('operation' , a)
            nam    = fetch('iu'        , a)[0]
            typ    = strType(fetch('iu'        , a)[1])
            env_names.append(nam)
            env_types.append(typ)
            if op == 'sum':
                a0 = genEachRight('@index', values[source], t3)
                a1 = genEach     ('@sum'  , a0)
                a2 = genRaze     (a1)
                env_alias.append(a2)
            elif op == 'avg':
                a0 = genEachRight('@index', values[source], t3)
                a1 = genEach     ('@avg'  , a0)
                a2 = genRaze     (a1)
                env_alias.append(a2)
            elif op == 'count':
                # a0 = genEachRight('@index', values[0], t3)
                a1 = genEach     ('@len'  , t3) # a0
                a2 = genRaze     (a1)
                env_alias.append(a2)
            elif op == 'countdistinct':
                a0 = genEachRight('@index', values[source], t3)
                a1 = genEach     ('@unique', a0)
                a2 = genEach     ('@len', a1)
                a3 = genRaze     (a2)
                env_alias.append(a3)
            elif op == 'min' or op == 'max':
                a0 = genEachRight('@index', values[source], t3)
                a1 = genEach     ('@'+op, a0)
                a2 = genRaze     (a1)
                env_alias.append (a2)
            elif op == 'keep' or op == 'any':
                a0 = genIndex(values[source], t2)
                env_alias.append(a0) # keep
            else:
                pending('op (%s) is not supported yet.' % op)
        # raise ValueError("stop")
        env_names.append('_aggr_value_')
        env_types.append('N/A')
        env_alias.append(t3)
    # values
    # print env_names
    # print env_types
    # print env_alias
    return encodeEnv('lambda_aggr', env_names, env_alias, env_types)

def scanHeader(d, env):
    new_env = scanMain(d['plan'], env)
    # debug(new_env)
    # debug('result')
    head = d['header']
    size = len(head)/2 
    temp = ''
    # sort?
    if 'order' in new_env:
        final_order = new_env['order']
        new_env_copy = copy.deepcopy(new_env)
        for x in range(size):
            updateEnvWithPartialAlias(head[x*2+1], genIndex(findAliasByName(head[x*2+1], new_env), final_order), new_env_copy)
        new_env = new_env_copy
        # insertMap(head[x*2+1], genIndex(findAliasByName(head[x*2+1], new_env),final_order))
    # build column names
    for x in range(size):
        temp = temp + '`%s' % head[x*2]
    a0 = genLiteral(temp + ':sym')
    # build column values
    # if size == 1:
    #     a1 = genEnlist(findAliasByName(head[1], new_env))
    # else:
    temp = []
    for x in range(size):
        temp.append(findAliasByName(head[x*2+1], new_env))
    a1 = genList(temp)
    a2 = genTable(a0, a1)
    genReturn(a2)  # return table
    return None

def scanRestrictionCell(d, cols):
    attr = fetch('attribute', d)
    mode = fetch('mode'     , d)
    if 'value' in d:
        col_name = cols[int(attr)]
        if 'value2' in d:
            # a0 = genAssignment('@%s(%s,%s)'%(m2p(mode[0]),col_name,scanValuesConst(d['value'])))
            # a1 = genAssignment('@%s(%s,%s)'%(m2p(mode[1]),col_name,scanValuesConst(d['value2'])))
            a0 = genDyadic(m2p(mode[0]), col_name, scanValuesConst(d['value']));
            a1 = genDyadic(m2p(mode[1]), col_name, scanValuesConst(d['value2']));
            return genAnd(a0, a1)
        else:
            # return genAssignment('@%s(%s,%s)'%(m2p(mode[0]),col_name,scanValuesConst(d['value'])))
            return genDyadic(m2p(mode), col_name, scanValuesConst(d['value']));
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

def scanValuesI(v):
    nam = v['iu'][0]
    typ = v['iu'][1]
    vid = genColumnValue(current_table, '`'+nam, strType(typ)) #value id
    # vid = genLoadTableWithCols(nam,strType(typ),current_table) 
    insertMap(nam, vid)  # for compress
    linkCol2Table(nam, current_table)
    linkTable2Cols(current_table, nam)
    return nam

def scanValues(d, env):
    values = []
    for v in d:
        k = scanValuesV(v['value'], env)
        t = getAliasType(k, env)
        values.append(getEnumValue(k, env))
    return values

def scanByFunc(d, name, foo):
    if name in d:
        return foo(d[name])
    else:
        unexpected('%s not found' % name)

def scanGroupby(d, env):
    debug('groupby')
    values = scanValues(d['values'], env)
    return scanAggr(d['aggregates'], values)

def scanTablescan(d, env):
    table_name = d['from']
    debug('tablescan for %s' % table_name)
    current_table  = genLoadTable(table_name)
    columns, alias, types = loadTables(d['values'], table_name, current_table)
    residuals      = scanResiduals(\
                        d['residuals'],\
                        encodeEnv(table_name, columns, alias, types)) \
                        if 'residuals' in d else None
    mask           = scanRestrictions(d['restrictions'], alias) if 'restrictions' in d else None
    if residuals is not None:
        if mask is not None:
            mask = genAnd(mask, residuals)
        else:  # q2, earlyprobe -> tablescan
            mask = residuals  # q13
    newAlias = actionCompress(mask, alias, env) if mask is not None else alias  #??
    return encodeEnv(table_name, columns, newAlias, types, mask, alias)

# Copied from scanRestrictions
def scanResiduals(d, env):
    debug('residuals')
    cells = []
    for c in d:
        value = scanValuesV(c, env)
        if value: cells.append(value) # skip isnotnull -> None
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
        # return genAssignment('@%s(%s,%s)'%(m2p(mode),leftValue,rightValue))
        return genDyadic(m2p(mode),leftValue,rightValue)
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
    list_order = genList(names)
    # list_order  = genAssignment('@list(%s)'%stringList(names))
    if len(bools) == 1:
        final_order = genSort(list_order, '%s:bool' % stringList(bools))
        # final_order = genAssignment('@sort(%s,%s:bool)'%(list_order,stringList(bools)))
    else:
        final_order = genSort(list_order, '(%s):bool' % stringList(bools))
        # final_order = genAssignment('@sort(%s,(%s):bool)'%(list_order,stringList(bools)))
    return addOrderToEnv(final_order, env)

#TODO: compact the code later
def scanSemijoin(d, env2, joinType):
    debug('join type: %s <----' % joinType)
    return handleJoinResult(scanCondition(d['condition'], 'hash', joinType, env2), joinType, env2)

def scanLeftsemijoin(d, env2):
    return scanSemijoin(d, env2, 'left_semijoin')

def scanRightsemijoin(d, env2):
    return scanSemijoin(d, env2, 'right_semijoin')

def scanCondition(d, tag, joinType, env):
    if tag == 'hash':
        # env -> 2 env
        return findExprFromSide2(d, joinType, env)
    elif tag == 'index':
        # index join == hash join ??
        return findExprFromSide2(d, joinType, env)
    elif tag == 'select':
        # q18
        return scanValuesV(d, env)
    else:
        unexpected("unknown tag %s found in condition" % tag)

def scanJoinIndex(d, env2):
    debug('join index <----')
    joinType = 'equi_join'
    return handleJoinResult2(scanCondition(fetch('condition', d), 'index', joinType, env2), joinType, env2)
    # e0 = genEnum(left_col, right_col)
    # return updateTableWithEnum(e0, env2[0], env2[1])

#Q1: hash join means key/fkey relation?
#Q2: key (left) and fkey(right)?
def scanJoinHash(d, env2, msg='hash join'):
    debug('%s <----' % msg)
    joinType = 'equi_join'
    # printEnv(env2[0])
    # printEnv(env2[1])
    # raw_input()
    return handleJoinResult2(scanCondition(d['condition'], 'hash', joinType, env2), joinType, env2)

def scanJoin(d, env2):
    method = fetch('method', d)  # unopt may don't have a method after join
    if method == 'hash':
        return scanJoinHash (d, env2)
    elif method == 'indexnl':
        return scanJoinIndex(d, env2)
    elif method == 'bnl':
        return scanJoinHash (d, env2, 'bnl join')  # same as hash join?
    else:
        unexpected('Unknown method %s' % method)

#scanMap: return new + old env
def scanMap(d, env):
    debug('map')
    vals = fetch('values', d)
    env_names = []; env_alias = []; env_types = []
    for v in vals:
        env_names.append(v['iu'][0])
        env_types.append(strType(v['iu'][1]))
        env_alias.append(scanValuesV(v['value'], env))
    return combineEnv2(encodeEnv('lambda_map', env_names, env_alias, env_types), env)

def scanGroupjoin(d, env2):
    def readExpressions(expr, env):
        rtn = []
        for x in expr:
            rtn.append(scanValuesV(x, env))
        return rtn
    def updateValueWithIndex(value, index):
        v_alias = []
        for v in value:
            v_alias.append(genIndex(v,index))
            # v_alias.append(genEachRight('@index',v,index))
        return v_alias
    def readAggrGrouped(aggr, value):
        ids = []
        for a in aggr:
            sId = int(a['source'])
            oId = a['operation']
            if oId == 'keep' or oId == 'any':
                ids.append(value[sId])
            else:
                pending('Grouped aggregation must have the operation "keep": %s' % oId)
        return ids
    def readAggrNotGrouped(aggr, value, length, env):
        ids = []
        hasSum = hasCount = False
        t_sum = t_count = -1
        for a in aggr:
            sId  = int(a['source'])
            oId  = a['operation']
            uId  = a['iu'][0]
            # print oId,uId
            # raw_input()
            if oId == 'sum':
                hasSum = True
                t0 = genEach('@sum', value[sId])
                t1 = genRaze(t0)
                t2 = genMul(length, t1)
                t_sum = t2
                ids.append(t_sum)
            elif oId == 'count' and sId != 4294967295:
                hasCount = True
                t0 = genEach('@len', value[sId])
                t1 = genRaze(t0)
                t2 = genMul(length, t1)
                t_count = t2
                ids.append(t_count)
            elif oId == 'avg':
                if hasSum and hasCount:
                    t0 = genDiv(t_sum, t_count)
                    ids.append(t0)
                else:
                    pending('not hasSum and hasCount: %d, %d' % (hasSum,hasCount))
            elif oId == 'keep':
                pass
        return ids
    def readAggrGroupedAfter(aggr, left_value, right_value):
        ids = []
        for a in aggr:
            sId = int(a['source'])
            oId = a['operation']
            if oId == 'count' and sId != 4294967295:
                t0 = genMul(left_value, right_value)
                ids.append(t0)
            else:
                pass
        return ids
    def isFirstSource(aggrs):
        for a in aggrs:
            if a['source'] == 0:
                return True
        return False
    def fetchSourceName(aggrs, value):
        for a in aggrs:
            if a['source'] == value:
                return a['iu'][0]
        return ''
    def fetchSpecialName(aggrs):
        names = []
        for a in aggrs:
            if a['operation'] == 'sum':
                names.append(a['iu'][0])
        return names
    def copyGroupJoinAggr(d, isLeft):
        exprs = d['leftExpressions' if isLeft else 'rightExpressions']
        aggrs = d['leftAggregates'  if isLeft else 'rightAggregates']
        keys  = d['leftKey' if isLeft else 'rightKey']
        values = []; key_list = []
        for a in keys:
            values.append({'value': a})
            key_list.append(fetchID(a))
        for a in exprs:
            values.append({'value': a})
        if not isFirstSource(aggrs):
            aggrs.append({ 'source': 0, 'operation': 'keep', 'iu': [fetchID(keys[0]), [ 'N/A' ]] })
        return {
            'aggregates': aggrs,
            'values'    : values,
            'special'   : fetchSpecialName(aggrs)
        }
    """ Start from here """
    debug('group join')
    left_env, right_env = env2
    left_keys  = d['leftKey']
    right_keys = d['rightKey']
    semantic   = d['semantic']
    # print left_keys, right_keys
    # printEnv(env2[0])
    # printEnv(env2[1])
    # raw_input()
    key_grouped = 'groupbyscan'
    isLeftGrouped = key_grouped in getEnvTable(left_env)
    isRightGrouped = key_grouped in getEnvTable(right_env)
    # print isLeftGrouped, isRightGrouped
    if len(left_keys) == 1:
        # it seems left_col and right_col are useless
        # since 'source' has indicated the name of keys from both sides
        left_name  = left_keys [0]['iu']
        right_name = right_keys[0]['iu']
        left_alias  = getEnumValue(findAliasByName(left_name , left_env ), left_env )
        right_alias = getEnumValue(findAliasByName(right_name, right_env), right_env)
        # print left_alias, right_alias
        left_value = [ left_alias ] + readExpressions(d['leftExpressions'], left_env)
        right_value = [ right_alias ] + readExpressions(d['rightExpressions'], right_env)
        # print isLeftGrouped, isRightGrouped
        # print left_value, right_value
        # raw_input()
        if isLeftGrouped:
            if isRightGrouped:
                EP('pending: %d, %d' % (isLeftGrouped,isRightGrouped))
            else:
                # 0. join index
                t0_0 = genJoinIndex('@eq', left_alias, right_alias)
                t0_1 = genIndex(t0_0, '0:i64')
                t0_2 = genIndex(t0_0, '1:i64')
                # 1. group by right side
                t1_0 = genGroup(t0_1)
                t1_1 = genKeys(t1_0)
                t1_2 = genValues(t1_0)
                t1_3 = genIndex(t0_1, t1_1)
                t1_4 = genEachRight('@index',t0_2,t1_2)
                t1_5 = genIndex(left_alias, t1_3) # left key
                # 2. return
                t2_0 = findAliasByName('_aggr_value_', left_env)
                t2_1 = genEach('len', t2_0)
                t2_2 = genRaze(t2_1)
                t2_3 = genIndex(t2_2, t1_3) # length
                left_aggr = readAggrGrouped(d[ 'leftAggregates'],  left_value)
                right_updated_value = updateValueWithIndex(right_value, t1_4)
                right_aggr = readAggrNotGrouped(d['rightAggregates'], right_updated_value, t2_3, right_env)
                # print left_value, right_value
                env_names =[] ; env_alias = []; env_types = []
                # print left_aggr
                # print right_aggr
                # raw_input()
                addEnvValuesFromVector(env_names, env_alias, env_types, d['leftAggregates' ],  left_aggr)
                addEnvValuesFromVector(env_names, env_alias, env_types, d['rightAggregates'], right_aggr)
                return encodeEnv('lambda_groupjoin', env_names, env_alias, env_types)
        else:
            if isRightGrouped:
                EP('pending: %d, %d' % (isLeftGrouped,isRightGrouped))
            else:
                if semantic == 'inner':
                    # left table
                    leftAggr  = copyGroupJoinAggr(d, True)
                    new_left_env  = scanGroupby( leftAggr, left_env )
                    t0_0 = findAliasByName('_aggr_value_', new_left_env)
                    t0_1 = genEach('@len', t0_0)
                    t0_2 = genRaze(t0_1)   # length of left side
                    # printEnv(new_left_env)
                    # right table
                    rightAggr = copyGroupJoinAggr(d, False)
                    new_right_env = scanGroupby(rightAggr, right_env)
                    # printEnv(new_right_env)
                    left_name = fetchSourceName(leftAggr['aggregates'], 0)
                    right_name = fetchSourceName(rightAggr['aggregates'], 0)
                    # print left_name, right_name
                    left_key = findAliasByName(left_name, new_left_env);
                    right_key = findAliasByName(right_name, new_right_env);
                    # print left_key, right_key
                    t1_0 = genJoinIndex('@eq', left_key, right_key)
                    t1_1 = genIndex(t1_0, '0:i64')
                    t1_2 = genIndex(t1_0, '1:i64')
                    t1_3 = genIndex(t0_2, t1_1)   # left side: length
                    return updateEnvWithSpecial(rightAggr['special'], t1_3, updateEnvWithIndex2(t1_1, t1_2, new_left_env, new_right_env))
                # group: left
                t0_0 = genGroup(left_alias)
                t0_1 = genKeys(t0_0)
                t0_2 = genValues(t0_0)
                t0_3 = genEach('@len', t0_2)
                t0_4 = genRaze(t0_3)
                t0_5 = genIndex(left_alias, t0_1)
                # group: right
                t1_0 = genGroup(right_alias)
                t1_1 = genKeys(t1_0)
                t1_2 = genValues(t1_0)
                t1_3 = genEach('@len', t1_2)
                t1_4 = genRaze(t1_3)
                t1_5 = genIndex(right_alias, t1_1)
                # join
                t2_0 = genJoinIndex('@eq', t0_5, t1_5)
                t2_1 = genIndex(t2_0, '0:i64')
                t2_2 = genIndex(t2_0, '1:i64')
                t2_3 = genIndex(t0_4, t2_1)
                t2_4 = genIndex(t1_4, t2_2)
                left_updated_value = updateValueWithIndex(left_value, t2_1) # t2_3
                right_updated_value = updateValueWithIndex(right_value, t2_2) # t2_4
                left_aggr  = readAggrGrouped(d['leftAggregates'], left_updated_value)
                right_aggr = []
                if semantic == 'outer':
                    right_after = readAggrGroupedAfter(d['rightAggregates'], t2_3, t2_4)
                    t3_0 = genLength(t0_1)
                    for val_aggr in right_after:
                        t3_1 = genVector(t3_0, '0:i64')
                        t3_2 = genIndexA(t3_1, t2_1, val_aggr)  # left side
                        right_aggr.append(t3_2)
                elif semantic == 'inner':
                    # print right_updated_value
                    # print d['rightAggregates']
                    # raw_input()
                    right_aggr = readAggrNotGrouped(d['rightAggregates'], right_updated_value, t2_3, right_env)
                    # right_aggr = right_updated_value
                    stop(right_aggr)
                else:
                    pending('GroupJoin: semantic not supported: %s' % semantic)
                env_names =[] ; env_alias = []; env_types = []
                addEnvValuesFromVector(env_names, env_alias, env_types, d['leftAggregates' ],  left_aggr)
                addEnvValuesFromVector(env_names, env_alias, env_types, d['rightAggregates'], right_aggr)
                return encodeEnv('lambda_groupjoin', env_names, env_alias, env_types)
        # raise
        # # if left outer join (q13)
        # env_names =[] ; env_alias = []; env_types = []
        # addEnvValues(env_names, env_alias, env_types, d['leftAggregates' ],  left_env, None)
        # addEnvValues(env_names, env_alias, env_types, d['rightAggregates'], right_env, None)
        # print d['leftAggregates']
        # printEnv(left_env)
        # print env_names
        # print env_alias
        # print env_types
        # print d['rightAggregates']
        # printEnv(right_env)
        # raw_input()
        # raise
        # # q3
        # # handle: leftExpressions
        # # handle: rightExpressions
        # env_names =[] ; env_alias = []; env_types = []
        # addEnvValues(env_names, env_alias, env_types, d['leftAggregates' ],  left_env, None)
        # addEnvValues(env_names, env_alias, env_types, d['rightAggregates'], right_env, w3  )
        # return encodeEnv('lambda_groupjoin',env_names, env_alias, env_types)
    else:
        unexpected('keys > 1 found in group join')

def addTarget2Env(d, env):
    env_names = [] ; env_alias = []; env_types = []
    for x in d:
        source_name = x['source'][0] if isList(x['source']) else x['source']
        env_names.append(x['target'][0])
        env_alias.append(findAliasByName(source_name, env))
        # env_alias.append(genAssignment(findAliasByName(source_name, env)))
        env_types.append(strType(x['target'][1]))
    return combineEnv2(env, encodeEnv('add_target', env_names, env_alias, env_types), False)

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

def scanAntijoin(d, env2, joinType):
    debug('join type: %s <----' % joinType)
    return handleJoinResult(scanCondition(d['condition'], 'hash', joinType, env2), joinType, env2)

def scanLeftantijoin(d, env2):
    return scanAntijoin(d, env2, 'left_antijoin')

def scanRightantijoin(d, env2):
    return scanAntijoin(d, env2, 'right_antijoin')

def scanSelect(d, env):
    debug('select')
    mask = scanCondition(d['condition'], 'select', 'Not a join (select)', env)
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
        # print old_key
        for c,v in enumerate(old_cols):
            if v == old_key:
                old_cols[c] = new_key
                break
    return updateEnvWithCols(old_cols, source_env, 'lambda_groupbyscan')

# read source from the result of join (left-side)
def scanMagic(d, env):
    debug('magic')
    global id_to_env
    op = d['operator']
    if op == 'groupby':
        # if len(d['values']) == 1:  #q17
        #     colName = d['values'][0]['value']['iu']
        #     print checkPrimaryKey(getEnvTable(env), colName) # False
        #     printEnv(env)
        #     raw_input()
        id_to_env[d['operatorId']] = scanGroupby(d, env)
    else:
        unexpected('(%s) not impl. in magic' % op)


"""
Entry for joins
  checkExpr
    0: none
    1: left/value
    2: right
    3: both
"""
def checkExpr(expr):
    expr_name = expr['expression']
    # print expr_name
    if expr_name == 'quantor':
        return [1,fetchID(expr['value'])]
    elif expr_name == 'between':
        return [1,fetchID(expr['arguments'][0])]
    elif expr['left']['expression'] == 'iuref':
        return [3,[fetchID(expr['left']),fetchID(expr['right'])]] if expr['right']['expression'] == 'iuref' else [1,fetchID(expr['left'])]
    elif expr['right']['expression'] == 'iuref':
        return [2,fetchID(expr['right'])]
    else:
        return [0,'<none>']

def scanConditionOr(d, env2):
    """ q19 """
    global vec_side_or
    # off load to scanValue
    vec_left_or = []
    vec_right_or = []
    # left_env, right_env = env2
    # printEnv(left_env)
    # printEnv(right_env)
    # raw_input()
    for d0 in d['arguments']:
        if d0['expression'] == 'and':
            vec_left_and = []
            vec_right_and = []
            for d1 in d0['arguments']:
                t0 = scanValuesV(d1, env2)
                typ,nam = checkExpr(d1)
                if typ == 1 or typ == 2:
                    side = whichTableByName(nam, env2)  # important: decide a name from which side
                    if side == 0:  # 0:left / 1:right
                        vec_left_and.append(t0)
                    else:
                        vec_right_and.append(t0)
                else:
                    unexpected('unknown type %d' % typ)
            vec_left_or.append(genVectorAnd(vec_left_and))
            vec_right_or.append(genVectorAnd(vec_right_and))
    vec_side_or = [vec_left_or, vec_right_or]
    return 0,0
    # return genVectorOr(vec_left_or),genVectorOr(vec_right_or)

def scanConditionOr2(d, env2):
    vec_left_or = []
    vec_right_or = []
    # print d
    for d0 in d['arguments']:
        if d0['expression'] == 'and':
            vec_left_and = []
            vec_right_and = []
            for d1 in d0['arguments']:
                t0 = scanValuesV(d1, env2)
                typ,nam = checkExpr(d1)
                if typ == 1 or typ == 2:
                    if 0 == whichTableByName(nam, env2):
                        vec_left_and.append(t0)
                    else:
                        vec_right_and.append(t0)
                else:
                    unexpected('unknown type %d' % typ)
            vec_left_or.append(genVectorAnd(vec_left_and))
            vec_right_or.append(genVectorAnd(vec_right_and))
    # print [vec_left_or, vec_right_or]
    # raw_input()
    return [vec_left_or, vec_right_or]

def fixSideInfo(name0, name1, env2):
    side0 = findSideByName2(name0, env2)
    side1 = findSideByName2(name1, env2)
    if side0 == 0 and side1 == 1:
        return [name0, name1, 0]
    elif side0 == 1 and side1 == 0:
        return [name1, name0, 1]
    else:
        unexpected('both columns (%s, %s) from the same table (%d,%d)' % (name0,name1,side0,side1))

"""
Conditions
  - comparison
  - and
  - or
  - lookup
"""

def scanJoinCondition(d, env2):
    """ Return join columns """
    expr = d['expression']
    if expr == 'comparison':
        typ,_ = checkExpr(d)
        if typ == 3:
            mode = d['mode']
            left_value, right_value, flag = fixSideInfo(fetchID(d['left']), fetchID(d['right']), env2)
            return {
                "expr"  : 'comparison',
                "value" : [left_value, right_value, fixMode(mode) if flag else mode]
            }
        else:
            unexpected('error in join condition')
    elif expr == 'and':
        cond_list = []
        for d0 in d['arguments']:
            t0 = scanJoinCondition(d0, env2)
            cond_list.append(t0)
        return {
            "expr"  : expr,
            "value" : cond_list
        }
    elif expr == 'or':
        result_or = scanConditionOr2(d, env2)
        return {
            "expr" : expr,
            "value": result_or
        }
        # unexpected('adding or, see q19')
    elif expr == 'lookup':  #q7
        const_list = getConstList(d['values'])
        size = 0; indx0 = ''; indx1 = ''
        while size < len(const_list):
            t0, t1 = getAliasList(d['input'], const_list[size:], env2)
            indx0 = genAppend(indx0, t0) if indx0 else t0
            indx1 = genAppend(indx1, t1) if indx1 else t1
            size = size + len(d['input'])
        return {
            "expr" : expr,
            "value": [indx0, indx1]
        }
    else:
        unexpected('join error: %s' % expr)

def isSingleColumnJoin(x):
    """ Single / Multiple """
    return x['expr'] == 'comparison'

def joinColumnsMultiple2(joinType, cond_list, env2):
    # print cond_list
    def isJoinAllEqual(x):
        for cell in x['value']:
            if cell['expr'] == 'comparison':
                _,_,mode = cell['value']
                if mode != '=' and mode != 'is': return False
            else: return False
        return True
    def strJoinOp(x):
        str_op = ''
        for cell in x['value']:
            _,_,mode = cell['value']
            str_op = str_op + '@' + m2p(mode)
        return str_op
    #if joinType != 'equi_join':
    #    pending('multiple columns not supported for join: %s' % joinType)
    left_alias = []; right_alias = []; left_env, right_env = env2
    join_op = '@eq' if isJoinAllEqual(cond_list) else strJoinOp(cond_list)
    # gen code
    for cond in cond_list['value']:
        left_name, right_name, _ = cond['value']
        left_alias.append(getEnumValue(findAliasByName(left_name, left_env), left_env))
        right_alias.append(getEnumValue(findAliasByName(right_name, right_env), right_env))
    t0 = genList(left_alias)
    t1 = genList(right_alias)
    t2 = genJoinIndex(join_op, t0, t1)
    if joinType == 'equi_join':
        p0 = genIndex(t2, '0:i64')
        p1 = genIndex(t2, '1:i64')
        return [p0, p1, 'indexing', 'plain']
    elif joinType == 'left_antijoin':
        p0 = genIndex(t2, '0:i64')
        p1 = genVector(genLength(p0), '1:bool')
        p2 = genIndexA(p1, p0, '0:bool')
        return [p2, None, 'masking', 'plain']
    elif joinType == 'left_semijoin':
        p0 = genIndex(t2, '0:i64')
        p1 = genUnique(p0) #?
        p2 = genIndex(p0, p1)
        return [p2, None, 'indexing', 'plain']
    else:
        pending('joinType not supported: %s' % joinType)
        

def joinColumnsHybrid(joinType, cond_list, env2):
    pos = 0 if cond_list['value'][0]['expr'] == 'or' else 1
    or_left, or_right = cond_list['value'][pos]['value']
    cmp_left, cmp_right, cmp_mode = cond_list['value'][1-pos]['value']
    cmp_left, cmp_right, side_info = fixSideInfo(cmp_left, cmp_right, env2)
    left_env, right_env = env2
    if cmp_mode == '=':
        left_alias  = findAliasByName(cmp_left, left_env)
        right_alias = findAliasByName(cmp_right, right_env)
        or_list = []
        for c,_ in enumerate(or_left):
            t0 = genCompress(or_left[c],  left_alias)
            t1 = genMember  (t0, right_alias)
            t2 = genAnd     (t1, or_right[c])
            or_list.append(t2)
        tt = genVectorOr(or_list)
        return [tt, tt, 'masking', 'plain']
    else:
        pending('column join hybrid case fail: join type = %s, mode = %s' % (joinType,cmp_mode))

def findExprFromSide2(d, joinType, env2, isCollect=False):
    def isJoinAllComparison(x):
        for cell in x['value']:
            if cell['expr'] == 'and':
                if not isJoinAllComparison(cell):
                    return False
            elif cell['expr'] != 'comparison':
                return False
        return True
    def isJoinAndOrCompare(x):
        """ and{or, =} """
        if x['expr'] == 'and' and len(x['value']) == 2:
            value = x['value']
            if value[0]['expr'] == 'or' or value[1]['expr'] == 'or':
                if value[0]['expr'] == 'comparison' or value[1]['expr'] == 'comparison':
                    return True
        return False
    def floatConditionExpr(x):
        value = []
        for cell in x['value']:
            if cell['expr'] == 'and':
                value = value + floatConditionExpr(cell)
            else:
                value.append(cell)
        return value
    def flatConditionList(x):
        return {
            "expr": "and",
            "value": floatConditionExpr(x)
        }
    expr = d['expression']
    # printEnv(env2[0])
    # printEnv(env2[1])
    # raw_input()
    cond_list = scanJoinCondition(d, env2)
    # printJSON(cond_list)
    if isSingleColumnJoin(cond_list):
        name0, name1, mode = cond_list['value']
        if mode == '=' or mode == 'is':
            return joinColumns(joinType, name0, env2[0], name1, env2[1])
        elif mode == '>' or mode == '<' or mode == '<>': # check cardinality == 1?
            return joinColumnsWithMode(mode, name0, env2[0], name1, env2[1])
        else:
            unexpected('cond (%s) not handled' % mode)
    elif expr == 'and':
        # print 'multiple column joins'
        if isJoinAllComparison(cond_list):
            return joinColumnsMultiple2(joinType, flatConditionList(cond_list), env2)
        elif isJoinAndOrCompare(cond_list):
            return joinColumnsHybrid(joinType, cond_list, env2)
        else:
            unexpected('not supported join type: ' + str(cond_list))
    elif expr == 'lookup':
        return cond_list['value'] + ['indexing', 'plain']
    else:
        unexpected('not supported join: %s' % expr)
    pass

def isMultipleColumnJoin(d, env2):
    """ Return if multiple columns (>1) """
    numJoin = 0
    # print scanJoinCondition(d) # for q9
    if d['expression'] == 'and':
        for d0 in d['arguments']:
            if d0['expression'] == 'comparison': #q17, d0['mode'] == '<' | 'is'
                typ,_ = checkExpr(d0)
                if typ != 3: return False
                else: numJoin = numJoin + 1
            else:
                return False
    print 'Number of joins: %d' % numJoin
    return True if numJoin > 1 else False

"""
return: [left,right,type]
type  : value / indexing
"""
def findExprFromSide(d, joinType, env2, isCollect=False):
    expr = d['expression']
    # findExprFromSide2(d, joinType, env2, isCollect) # debug
    if expr == 'comparison': # single join case
        mode = d['mode']; left = [] ; right = []
        name0 = fetchID(d['left' ])
        name1 = fetchID(d['right'])
        # side0, alias0 = findMaskAliasByName2(name0, env2)
        # side1, alias1 = findMaskAliasByName2(name1, env2) # findAliasByName2(name1, env2)
        side0 = findSideByName2(name0, env2)
        side1 = findSideByName2(name1, env2)
        if side0 == side1:
            unexpected('pred. vars from both sides')
        # q2/q17/q20: 'is'
        if mode == '=' or mode == 'is':
            # print 'side0 = %d, side1 = %d' % (side0, side1)
            # print 'alias0 = %s, alias1 = %s' % (alias0, alias1)
            if isCollect:
                #print name0, name1
                alias0 = findAliasByName(name0, env2[side0])
                alias1 = findAliasByName(name1, env2[side1])
                return ([alias0, alias1, 'value'] if side0 == 0 else [alias1, alias0, 'value']) + [name0, name1]
            if side0 != 0:
                name0,name1 = name1,name0
            return joinColumns(joinType, name0, env2[0], name1, env2[1])
        elif mode == '>' or mode == '<' or mode == '<>':
            # '>' (q22)
            # print side0
            # print name0,name1
            # printEnv(env2[0])
            # printEnv(env2[1])
            # raw_input()
            if side0 != 0:
                env2[0],env2[1] = env2[side0],env2[side1]
            return joinColumnsWithMode(mode, name0, env2[0], name1, env2[1])
            # if side0 == 0:
            #     return joinColumnsGeneral(alias0, name0, alias1, name1)
            # else:
            #     return joinColumnsGeneral(alias1, name1, alias0, name0)
        else: unexpected('cond (%s) not handled' % mode)
    elif expr == 'and':  #q7, multiple column join
        if isMultipleColumnJoin(d, env2):
            # pending('No support for multiple column joins')
            return joinColumnsMultiple(joinType, d, env2)
            # return joinColumnsSpecial(joinType, d, env2)
            # left_list = [] ; right_list = []
            # for d0 in d['arguments']:
            #     new_left, new_right, _ = findExprFromSide(d0, joinType, env2, True)
            #     left_list.append(new_left)
            #     right_list.append(new_right)
            # t0 = genList(left_list)
            # t1 = genList(right_list)
            # return joinColumns(joinType, t0, env2[0], t1, env2[1], True)
        else:
            # q19
            cnt_pred = cnt_new = 0
            left_list = []; right_list = []
            for d0 in d['arguments']:
                if d0['expression'] == 'or':
                    pred_left, pred_right = scanConditionOr(d0, env2)
                    cnt_pred = cnt_pred + 1
                elif d0['expression'] == 'and':
                    # print '-'*30
                    # print d0
                    # print joinType
                    # print findExprFromSide(d0, joinType, env2, True)
                    # print '-'*30
                    # raw_input()
                    new_left, new_right, _, _ = findExprFromSide(d0, joinType, env2, True)
                    left_list.append(new_left)
                    right_list.append(new_right)
                elif d0['expression'] == 'comparison' and d0['mode'] == '=':
                    new_left, new_right, _, name0, name1 = findExprFromSide(d0, joinType, env2, True)
                    left_list.append(new_left)
                    right_list.append(new_right)
                    cnt_new = cnt_new + 1
                else:
                    print d0
                    unexpected('unknown expression (%s) in join' % d0['expression'])
            # assume only 1 'or' and 1 'and' upon
            if cnt_pred == 1 and cnt_new == 1:
                # apply pred to new first before join two sides
                # left_env  = updateMaskInEnv(pred_left, env2[0])
                # right_env = updateMaskInEnv(pred_right, env2[1])
                # env2[0], env2[1] = left_env, right_env  # q19: need update env2
                left_env, right_env = env2
                # printEnv(left_env)
                # printEnv(right_env)
                # print vec_side_or
                # raw_input()
                return joinColumns(joinType, name0, left_env, name1, right_env) # l_partkey / p_partkey
            elif cnt_pred == 0 and len(left_list) > 1: #q9
                t0 = genList(left_list)
                t1 = genList(right_list)
                return joinColumns(joinType, t0, env2[0], t1, env2[1]) # left_list / right_list
            else:
                unexpected('cnt_pred = %d, cnt_new = %d' % (cnt_pred,cnt_new))
    elif expr == 'lookup': #q7
        const_list = getConstList(d['values'])
        size = 0; indx0 = ''; indx1 = ''
        while size < len(const_list):
            t0, t1 = getAliasList(d['input'], const_list[size:], env2)
            indx0 = genAppend(indx0, t0) if indx0 else t0
            indx1 = genAppend(indx1, t1) if indx1 else t1
            size = size + len(d['input'])
        # unique [indx0, indx1]
        # indx = genUnique(genList('%s,%s' % (indx0, indx1)))
        # indx = genUnique(genList([indx0, indx1]))
        # left  = genIndex(indx0, indx)
        # right = genIndex(indx1, indx)
        return [indx0, indx1, 'indexing', 'plain'] 
    else:
        unexpected('not handled (%s) when looking for side info.' % expr)

# Need to update the result of both sides
def handleJoinResult2(result, joinType, env2):
    left_col, right_col, result_type, result_kind = result
    left_env, right_env = env2
    if result_kind != 'relation' and (left_col == None or right_col == None):
        pending("left_col or right_col can't be None: " + left_col + ", " + right_col)
        side = 0 if right_col == None else 1
        return handleJoinResultWithMode(result, joinType, side, env2) # see joinColumnsWithMode (q22)
    if result_kind == 'relation':
        updateFunc = updateEnvWithMaskIndex2 if result_type == 'indexing' else updateEnvWithMaskMask2
    elif result_kind == 'plain':
        updateFunc = updateEnvWithIndex2 if result_type == 'indexing' else updateEnvWithMask2
    else:
        pending('unknown result_type: %s' % result_type)
    if joinType == 'equi_join':
        return updateFunc(left_col, right_col, left_env, right_env)
    else:
        pending('unknown join type: %s' % joinType)

def handleJoinResult(result, joinType, env2):
    # print result
    # raw_input()
    left_col, right_col, result_type, result_kind = result
    left_env, right_env = env2  # assign env2[0]/[1]
    if result_kind == 'relation':
        updateFunc = updateEnvWithMaskIndex if result_type == 'indexing' else updateEnvWithMaskMask
    elif result_kind == 'plain':
        updateFunc = updateEnvWithIndex if result_type == 'indexing' else updateEnvWithMask
    else:
        pending('unknown result_type: %s' % result_type)
    if joinType == 'left_semijoin' or joinType == 'left_antijoin':
        return updateFunc(left_col, left_env)
    elif joinType == 'right_semijoin' or joinType == 'right_antijoin':
        return updateFunc(right_col, right_env)
    else:
        pending('unknown join type: %s' % joinType)

def handleJoinResultWithMode(result, joinType, side, env2):
    left_col, right_col, result_type, result_kind = result
    # left_env, right_env = env2  # assign env2[0]/[1]
    env = env2[side]
    col = left_col if side == 0 else right_col
    if result_kind == 'plain' and result_type == 'masking':
        updateFunc = updateEnvWithMaskNoCopy
    else:
        pending('unknown result_type (%s) and result_kind' % (result_type, result_kind))
    if joinType == 'equi_join':
        return updateFunc(col, env, getEnvTable(env))
    else:
        pending('unknown join type: %s' % joinType)

#default: side='left'
def processJoinResult(res, env2, side='left'):
    # print res
    left_col, right_col, result_type = res
    if result_type == 'value':
        if side == 'right':
            e0 = genEnum(right_col, left_col)
        else:
            e0 = genEnum(left_col, right_col)
        return updateTableWithEnum(e0, env2[0], env2[1])
    elif result_type == 'indexing':
        return updateEnvWithIndex2(left_col, right_col, env2[0], env2[1])
    else:
        unexpected('Unsupported return type for join: (%s)' % result_type)

"""
Join functions
"""
keycol_list = [
  'n_nationkey' , 's_suppkey'  , 'p_partkey'   , 'c_custkey', 'l_orderkey', 'o_orderkey',
  'n_nationkey2', 's_suppkey2'
]


def joinColumnsGeneral(left_alias, left_name, right_alias, right_name, isList=False):
    todo('general join -- need check')
    t0 = genOuter('@eq' if not isList else '@same', left_alias, right_alias)
    t1 = genWhere(t0)
    t2 = genIndex(t1,'0:i64','i64')
    t3 = genIndex(t2,'1:i64','i64')
    return [t2, t3, 'indexing']
    # return ['<none>', '<none>']


def joinWithEnum(joinType, k_alias, k_env, f_alias, f_env):
    k_mask  = getEnvMask(k_env)
    f_mask  = getEnvMask(f_env)
    if joinType == 'left_semijoin':
        t0 = genValues   (f_alias)
        t1 = genKeys     (f_alias)
        if k_mask and f_mask:    
            t2 = genCompress (f_mask, t0)
            t3 = genLength   (t1)
            t4 = genVector   (t3, '0:bool')
            t5 = genIndexA   (t4, t2, '1:bool')
            p0 = genAnd      (k_mask, t5)
        elif k_mask:
            t3 = genLength   (t1)
            t4 = genVector   (t3, '0:bool')
            t5 = genIndexA   (t4,t0,'1:bool')
            p0 = genAnd      (k_mask, t5)
        elif f_mask:
            t2 = genCompress (f_mask, t0)
            t3 = genLength   (t1)
            t4 = genVector   (t3, '0:bool')
            t5 = genIndexA   (t4, t2, '1:bool')
            p0 = t5
        else:
            pending('join without masks found. 1')
        return [p0, None, 'masking']
    elif joinType == 'right_semijoin':
        if k_mask and f_mask:
            t0 = genValues   (f_alias)
            t1 = genCompress (f_mask, t0)
            t2 = genIndex    (k_mask, t1)
            t3 = genWhere    (f_mask)
            p1 = genCompress (t2, t3)
            kind = 'indexing'
        elif k_mask:
            t0 = genValues   (f_alias)
            p1 = genIndex    (k_mask, t0)
            kind = 'masking'
        elif f_mask:
            p1 = f_mask
            kind = 'masking'
        else:
            pending('join without masks found. 2')
        return [None, p1, kind]
    elif joinType == 'equi_join':
        # print k_mask, f_mask
        # print k_alias,f_alias
        # raw_input()
        if k_mask and f_mask:
            t0 = genValues   (f_alias)
            t1 = genCompress (f_mask, t0)
            t2 = genIndex    (k_mask, t1)
            p0 = genCompress (t2, t1)
            t3 = genWhere    (f_mask)
            p1 = genCompress (t2, t3)
        elif k_mask:
            t0 = genValues   (f_alias)
            t1 = genIndex    (k_mask, t0)
            p0 = genCompress (t1, t0)
            p1 = genWhere    (t1)
        elif f_mask:
            t0 = genValues   (f_alias)
            p0 = genCompress (f_mask, t0)
            p1 = genWhere    (f_mask)
        else: # no mask
            p0 = genValues   (f_alias)
            p1 = None
            # pending('join without masks found. 3')  #9
        return [p0, p1, 'indexing']
    elif joinType == 'left_antijoin':
        if k_mask and f_mask:
            pending('[joinWithEnum]: k_mask and f_mask')
        elif k_mask:
            t0 = genValues   (f_alias)
            p0 = genIndexA   (k_mask, t0, '0:bool')
        elif f_mask:
            pending('[joinWithEnum]: f_mask')
        else:
            pending('join without masks found. 4')
        return [p0, None, 'masking']
    elif joinType == 'groupjoin':
        if k_mask:
            pending('adding k_mask for %s' % joinType)
        elif f_mask:
            t0 = genValues   (f_alias)
            t1 = genCompress (f_mask, t0)
            t2 = genGroup    (t1)
            t3 = genKeys     (t2)
            t4 = genValues   (t2)
            return [None, t4, 'indexing']
        else:
            pending('adding neither k_mask nor f_mask for %s' % joinType)
    pending('[joinWithEnum] add more join types: join type (%s), mask (%s, %s)' % (joinType, k_mask, f_mask))

def joinWithKeys(joinType, k_alias, k_env, v_alias, v_env):
    global vec_side_or
    # print joinType
    # print k_alias
    # printEnv(k_env)
    # print v_alias
    # printEnv(v_env)
    # raw_input()
    k_alias = getEnumValue(k_alias, k_env)
    v_alias = getEnumValue(v_alias, v_env)
    if joinType == 'equi_join':
        # printEnv(k_env)
        # printEnv(v_env)
        # Fail in q14
        # t0 = genMember (k_alias, v_alias)
        # t1 = genVector(genLength(k_alias), '0:bool')
        # t2 = genIndexA(t1, genCompress(t0, v_alias), '1:bool')
        # return [t2, t0, 'masking']
        # Pass in q14
        if vec_side_or: # chf: has sth.
            # print k_alias, v_alias
            k_cond, v_cond = vec_side_or
            k_temp = []; v_temp = []; vector_or = []
            if not getEnvMask(k_env):
                for cond in k_cond:
                    k_temp.append(genCompress(cond, k_alias))
            else:
                unexpected('mask found')
            if getEnvMask(v_env):
                for x in k_temp:
                    v_temp.append(genMember(x, v_alias))
                for (i,v) in enumerate(v_temp):
                    vector_or.append(genAnd(v, v_cond[i]))
            else:
                unexpected('mask not found')
            t0 = genVectorOr(vector_or)
            vec_side_or = {} # reset
            return [t0, t0, 'masking']  # chf: t0, t0?
        else:
            # print k_alias, v_alias
            # printEnv(k_env)
            # printEnv(v_env)
            # raw_input()
            # TODO: add compress for both sides before join_index
            t0 = genJoinIndex('@eq', k_alias, v_alias)
            t1 = genIndex(t0, '0:i64')
            t2 = genIndex(t0, '1:i64')
            return [t1, t2, 'indexing']
            # e0 = genEnum  (k_alias, v_alias)
            # t0 = genValues(e0)  #p0
            # t1 = genKeys  (e0)
            # t2 = genLt    (t0, genLength(t1))
            # t3 = genWhere (t2)  #p1
            # return [t0, t3, 'indexing']
    elif joinType == 'right_antijoin':
        return joinWithoutRelation(joinType, k_alias, k_env, v_alias, v_env)
    elif joinType == 'left_antijoin':
        return joinWithoutRelation(joinType, k_alias, k_env, v_alias, v_env)
    elif joinType == 'right_semijoin':
        return joinWithoutRelation(joinType, k_alias, k_env, v_alias, v_env)
    elif joinType == 'left_semijoin':
        return joinWithoutRelation(joinType, k_alias, k_env, v_alias, v_env)
    else:
        pending('[joinWithKeys] add more join types: join type (%s)' % joinType)

def joinWithoutRelation(joinType, left_alias, left_env, right_alias, right_env, isList=False):
    debug('join without relation')
    # print joinType
    # printEnv(left_env)
    # printEnv(right_env)
    # raw_input()
    left_alias = getEnumValue( left_alias,  left_env)
    right_alias= getEnumValue(right_alias, right_env)
    if joinType == 'right_antijoin':
        t0 = genMember(left_alias, right_alias)
        p1 = genNot(t0)
        return [None, p1, 'masking']
    elif joinType == 'left_antijoin':
        t0 = genMember(right_alias, left_alias)
        p0 = genNot(t0)
        return [p0, None, 'masking']
    elif joinType == 'right_semijoin':
        p1 = genMember(left_alias, right_alias)
        return [None, p1, 'masking']
    elif joinType == 'left_semijoin':
        p0 = genMember(right_alias, left_alias)
        return [p0, None, 'masking']
    elif joinType == 'equi_join':
        t0 = genJoinIndex('@eq', left_alias, right_alias)
        p0 = genIndex(t0, '0:i64')
        p1 = genIndex(t0, '1:i64')
        return [p0, p1, 'indexing']
        #pending('general equi_join')
        #t0 = genOuter('@eq' if not isList else '@same', left_alias, right_alias)
        #t1 = genWhere(t0)
        #p0 = genIndex(t1,'0:i64','i64')
        #p1 = genIndex(t2,'1:i64','i64')
        #eturn [p0, p1, 'indexing']
    elif joinType == 'groupjoin':
        pending('general groupjoin')
        e0 = genEnum  (left_alias, right_alias) # if t0: key; t1 is fkey
        t0 = genValues(e0)
        t1 = genGroup (t0)   # dict<key, value>
        t2 = genKeys  (t1)   # key on the left should be kept
        t3 = genValues(t1)   # value
        return [None, t3, 'indexing']
    else:
        pending('[joinWithoutRelation] add more join types: join type (%s)' % joinType)

def joinWithScalar(joinType, left_alias, left_env, right_alias, right_env, isScalarLeft):
    if joinType == 'equi_join':
        t0 = genEq(left_alias, right_alias)
        t1 = genVector(genLength(t0), '1:64')
        return [t1, t0, 'masking'] if isScalarLeft else [t0, t1, 'masking']
    else:
        pending('[joinWithScalar] unknown join type: %s' % joinType)

"""
 return type: indexing / masking
"""
def joinColumns(joinType, left_name, left_env, right_name, right_env, isList=False):
    def reverseJoinType(t):
        return {
          'left_semijoin' : 'right_semijoin',
          'right_semijoin': 'left_semijoin'
        }.get(t,t)
    global keycol_list
    left_table  = getEnvTable(left_env)
    right_table = getEnvTable(right_env)
    # print left_name
    # printEnv(left_env)
    # print right_name
    # printEnv(right_env)
    # raw_input()
    left_alias  = findAliasByName(left_name, left_env)
    right_alias = findAliasByName(right_name, right_env)
    # debugging
    if checkRelation(left_table, left_name, right_table, right_name):
        debug('%s.%s (fkey) -> %s.%s(key), right -> left' % (right_table, right_name, left_table, left_name))
        left_alias  = findMaskAliasByName(left_name , left_env)
        right_alias = findMaskAliasByName(right_name, right_env)
        return joinWithEnum(joinType, left_alias, left_env, right_alias, right_env) + ['relation']
    elif checkRelation(right_table, right_name, left_table, left_name):
        debug('%s.%s (fkey) -> %s.%s(key), left -> right' % (left_table, left_name, right_table, right_name))
        left_alias  = findMaskAliasByName(left_name , left_env)
        right_alias = findMaskAliasByName(right_name, right_env)
        r = joinWithEnum(reverseJoinType(joinType), right_alias, right_env, left_alias, left_env)
        return [r[1],r[0],r[2],'relation']  # reverse order: p0/1
    elif checkPrimaryKey(left_table, left_name):
        debug('left keys (%s)' % left_alias)
        return joinWithKeys(joinType, left_alias, left_env, right_alias, right_env) + ['plain']
    elif checkPrimaryKey(right_table, right_name):
        debug('right keys (%s)' % right_alias)
        r = joinWithKeys(reverseJoinType(joinType), right_alias, right_env, left_alias, left_env) + ['plain']
        return [r[1],r[0],r[2],'plain']
    # elif getEnvCard(left_env) == 1:
    #     # pending('adding a new case for scalar on left/right, see q15')
    #     return joinWithScalar(joinType, left_alias, left_env, right_alias, right_env, True) + ['plain']
    # elif getEnvCard(right_env) == 1:
    #     return joinWithScalar(joinType, left_alias, left_env, right_alias, right_env, False) + ['plain']
    else: # plain join
        debug('plain join for (%s.%s) and (%s.%s)' % (left_table, left_name, right_table, right_name))
        return joinWithoutRelation(joinType, left_alias, left_env, right_alias, right_env) + ['plain']
    # below can be commented out
    if isList:
        todo('list join (multi-column join) -- need check')
        return joinColumnsGeneral(left_alias, left_name, right_alias, right_name, isList)
        # joinColumnsGeneral
        # return ['<none>', '<none>', 'unknown']
    else:
        # left side is a keyed column
        if left_name in keycol_list:
            e0 = genEnum(left_alias, right_alias)
            t0 = genValues(e0) # left_index
            t1 = genKeys(e0)
            t2 = genLt(t0, genLength(t1))
            t3 = genWhere(t2)  # right_index
            return [t0, t3, 'indexing']
        else:
            # right side is a keyed column
            if right_name in keycol_list:
                e0 = genEnum(right_alias, left_alias)
                t0 = genValues(e0) # right_index
                t1 = genKeys(e0)
                t2 = genLt(t0, genLength(t1))
                t3 = genWhere(t2)  # left_index
                return [t3, t0, 'indexing']
            else:
                # q15
                # joinColumnsGeneral
                # unexpected('Both sides are non-key (%s = %s)' % (left_name, right_name))
                return joinColumnsGeneral(left_alias, left_name, right_alias, right_name)


def joinColumnsWithMode(mode, left_name, left_env, right_name, right_env):
    # left_alias  = findMaskAliasByName(left_name, left_env)
    # right_alias = findMaskAliasByName(right_name, right_env)
    left_alias  = findAliasByName(left_name, left_env)
    right_alias = findAliasByName(right_name, right_env)
    if mode == '>':
        funcOp = genGt
    elif mode == '<':
        funcOp = genLt
    elif mode == '<>':
        funcOp = genNot
    else:
        pending('wrong mode found: %s' % mode)
    t0 = funcOp(left_alias, right_alias)
    t1 = genVector(genSum(t0), '0:i64')
    # mask = getEnvMask(left_env) if getEnvMask(left_env) else getEnvMask(right_env)
    # if mask: t0 = genAnd(t0, mask)
    # printEnv(left_env)
    # printEnv(right_env)
    # raw_input()
    if getEnvCard(left_env) == 1:
        return [t1, t0, 'masking', 'plain']
    elif getEnvCard(right_env) == 1:
        return [t0, t1, 'masking', 'plain']
    else:
        pending('no cardinality == 1: %d vs %d' % (getEnvCard(left_env), getEnvCard(right_env)))

# only for q17
def joinColumnsSpecial(joinType, d, env2):
    opSet = {}
    left_env, right_env = env2
    if d['expression'] == 'and':
        for d0 in d['arguments']:
            if d0['expression'] == 'comparison': #q17, d0['mode'] == '<' | 'is'
                mode = d0['mode'];
                left_key = d0['left']['iu']
                right_key= d0['right']['iu']
                opSet[mode] = [left_key, right_key]
                if mode == '<':
                    joinWithKeys('equi_join', left_key, left_env, right_key, right_env)
                elif mode == 'is':
                    joinWithKeys('equi_join', left_key, left_env, right_key, right_env)
    # 1st join
    left1, right1 = opSet['is']
    r_left1, r_right1, r_type = joinWithKeys('equi_join', left1, left_env, right1, right_env) #return 'indexing'
    # 2nd join
    left2, right2 = opSet['<']
    left_alias = findAliasByName(left2, left_env)
    right_alias= findAliasByName(right2, right_env)
    t0 = genIndex(left_alias, r_left1)
    t1 = genIndex(right_alias, r_right1)
    t2 = genLt(t0, t1)
    t3 = genCompress(t2, r_left1)
    t4 = genCompress(t2, r_right1)
    return [t3,t4,'indexing', 'plain']

def joinColumnsMultiple(joinType, d, env2):
    if joinType != 'equi_join':
        pending('multiple columns not supported for join: %s' % joinType)
    left_alias = []; right_alias = []
    left_env, right_env = env2
    for d0 in d['arguments']:
        if d0['expression'] == 'comparison':
            typ,_ = checkExpr(d0)
            if typ != 3:
                pending('unexpected d0: ' + d0)
            else:
                name0 = d0['left']['iu']
                name1 = d0['right']['iu']
                side0 = findSideByName2(name0, env2)
                side1 = findSideByName2(name1, env2)
                if side0 == side1: pending('join error')
                left_name  = name0 if side0 == 0 else name1
                right_name = name0 if side1 == 0 else name1
                left_alias.append(findAliasByName(left_name, left_env))
                right_alias.append(findAliasByName(right_name, right_env))
                print 'n0(%s, %d), n1(%s, %d)' % (name0,side0,name1,side1)
    t0 = genList(left_alias)
    t1 = genList(right_alias)
    t2 = genJoinIndex('@eq', t0, t1)
    t3 = genIndex(t2, '0:i64')
    t4 = genIndex(t2, '1:i64')
    return [t3, t4, 'indexing', 'plain']


"""
Update environment functions
"""
def updateEnvWithMask(mask, env, func_name='lambda1'):
    alias = actionCompress(mask, env['cols_a'], env)
    return encodeEnv(func_name, env['cols_n'], alias, env['cols_t'], mask, env['cols_a']) #update alias

def updateEnvWithMaskMask(mask, env, func_name='lambda2'):
    alias = actionCompress(mask, env['mask_a'], env)
    return encodeEnv(func_name, env['cols_n'], alias, env['cols_t'], mask, env['mask_a']) #update alias

def updateEnvWithMaskNoCopy(mask, env, func_name='lambda3'):
    if getEnvMask(env):  # update mask
        mask  = genAnd(mask, getEnvMask(env))
        alias = getEnvAlias(env)
    else:
        alias = actionCompress(mask, getEnvAlias(env), env)
    printEnv(encodeEnv(func_name, getEnvName(env), alias, getEnvType(env), mask, getEnvAlias(env)))
    raw_input('no copy')

def updateMaskInEnv(mask, env):
    alias = getEnvAlias(env)
    newAlias = actionCompress(mask, alias, env) if mask is not None else alias
    return encodeEnv(getEnvTable(env), getEnvName(env), newAlias, getEnvType(env), mask, alias)

def updateEnvWithCols(a, env, new_name=None):
    new_env = copy.deepcopy(env)
    new_env['cols_n'] = a
    if new_name:
        new_env['table'] = new_name
    return new_env

def updateEnvWithAlias(a, env):
    new_env = copy.deepcopy(env)
    new_env['cols_a'] = a
    return new_env

def updateEnvWithPartialAlias(n, a, env):
    cols_n = getEnvName(env)
    cols_a = getEnvAlias(env)
    if n not in cols_n:
        unexpected('name %s, alias %s not found' % (n,a))
    else:
        cols_a[cols_n.index(n)] = a

def updateEnvWithIndex(ind, env):
    alias = env['cols_a']
    new_a = []
    for a in alias:
        new_a.append(genIndex(getEnumValue(a, env), ind))
    return updateEnvWithAlias(new_a, env)

def updateEnvWithMaskIndex(ind, env):
    alias = env['mask_a']
    new_a = []
    for a in alias:
        new_a.append(genIndex(getEnumValue(a, env), ind))
    return updateEnvWithAlias(new_a, env)

def updateEnvWithMaskIndex2(ind1, ind2, env1, env2):
    return combineEnv2(\
        updateEnvWithMaskIndex(ind1,env1) if ind1 else env1,\
        updateEnvWithMaskIndex(ind2,env2) if ind2 else env2)

def updateEnvWithMaskMask2(ind1, ind2, env1, env2):
    # print 'input: ind1 = %s; ind2 = %s' % (ind1, ind2)
    # printEnv(env1)
    # updateEnvWithMaskMask(ind1,env1)
    # raw_input()
    return combineEnv2(\
        updateEnvWithMaskMask(ind1,env1) if ind1 else env1,\
        updateEnvWithMaskMask(ind2,env2) if ind2 else env2)

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
    t2 = genGeq(t0, t1)
    return updateEnvWithMask(t2, env)

def updateEnvWithIndex2(ind1, ind2, env1, env2):
    return combineEnv2(updateEnvWithIndex(ind1,env1), updateEnvWithIndex(ind2,env2))

def updateEnvWithMask2(mask1, mask2, env1, env2):
    # printEnv(env1)
    # printEnv(env2)
    # raw_input()
    return combineEnv2(updateEnvWithMask(mask1,env1), updateEnvWithMask(mask2,env2))

def updateEnvWithSpecial(names, left_size, env):
    """ q3, group join * size """
    env_names = getEnvName(env)
    env_alias = getEnvAlias(env)
    new_a = []
    for c,n in enumerate(env_names):
        a = env_alias[c]
        if n in names:
            new_a.append(genMul(getEnumValue(a, env), left_size))
        else:
            new_a.append(a)
    return updateEnvWithAlias(new_a, env)

"""
A temporary solution for mapping_key, need to save this info into env
"""
def loadTables(d, table_name, table_alias):
    global mapping_key
    cols  = []; alias = []; types = []
    for v in d:
        nam = v['iu'][0]
        typ = strType(v['iu'][1])
        col = v['name']
        if isOkAny2Enum(table_name, col):
            typ = 'enum' # convert to symbol
        elif isOkStr2Sym(table_name, col, typ):
            typ = 'sym' # convert from char to symbol
        vid = genColumnValue(table_alias, '`'+col+':sym', typ)
        if nam != col:
            mapping_key[nam] = col
        # vid = genLoadTableWithCols(nam, typ, table_alias)
        cols.append(nam); alias.append(vid); types.append(typ)
    return cols,alias,types

"""
Initializations
"""
def initRelations():
    global primary_key, foreign_key
    primary_key = {}  # dictionary
    primary_key['region']   = ['r_regionkey']
    primary_key['nation']   = ['n_nationkey']
    primary_key['part']     = ['p_partkey']
    primary_key['supplier'] = ['s_suppkey']
    primary_key['partsupp'] = ['ps_partkey', 'ps_suppkey']
    primary_key['customer'] = ['c_custkey']
    primary_key['lineitem'] = ['l_orderkey', 'l_linenumber']
    primary_key['orders']   = ['o_orderkey']
    foreign_key = {} # foreign keys
    def pack(c0, t1, c1):
        return [c0, t1, c1] if isinstance(c0, list) else [[c0], t1, [c1]]
    foreign_key['nation']   = [pack('n_regionkey', 'region'  , 'r_regionkey')]
    foreign_key['supplier'] = [pack('s_nationkey', 'nation'  , 'n_nationkey')]
    foreign_key['customer'] = [pack('c_nationkey', 'nation'  , 'n_nationkey')]
    foreign_key['partsupp'] = [pack('ps_suppkey' , 'supplier', 's_suppkey'  ),
                               pack('ps_partkey' , 'part'    , 'p_partkey'  )]
    foreign_key['orders']   = [pack('o_custkey'  , 'customer', 'c_custkey'  )]
    foreign_key['lineitem'] = [pack('l_orderkey' , 'orders'  , 'o_orderkey'),
                               pack(['l_partkey', 'l_suppkey'], 'partsupp', ['ps_partkey', 'ps_suppkey'])]
    initSpecialColumnTypes()

def initSpecialColumnTypes():
    global table_types
    table_types = {}
    table_types['nation']   = ['n_name']
    table_types['region']   = ['r_name']
    table_types['part']     = ['p_brand', 'p_type', 'p_container']
    table_types['customer'] = ['c_name', 'c_mktsegment']
    table_types['orders']   = ['o_orderpriority']
    table_types['lineitem'] = ['l_shipinstruct', 'l_shipmode']


"""
Helper functions:
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

def stringValue(typ, value, withType=True):
    global isOpSubstring
    def date2date(d):
        global standard_date
        current_date = standard_date + timedelta(int(d) - 2451545)
        return '%d.%02d.%02d'%(current_date.year,current_date.month,current_date.day)
    t = strType(typ)
    if t == 'sym' and isOpSubstring:
        t = 'str' #chf
    if t == 'str':
        z = '"' + value +'"'
    elif t == 'char':
        z = "'" + value + "'"
    elif t == 'sym':
        z = '`' + value if isSimpleSymbol(value) else '`"' + value + '"'
    elif t == 'd':
        z = date2date(value)
    elif t == 'f64': # Example: "type": [ "Numeric", 12, 2 ], "value": 2400 }
        digit = int(typ[2])
        z = '%g' % (float(value)/(10**digit))
    else:
        z = str(value)
    return z+':'+t if withType else z

def actionCompress(vid, cols, env):
    alias = []
    for c in cols:
        a = genCompress(vid, getEnumValue(c, env))
        alias.append(a)
    return alias


def encodeEnv(table_name, cols_names, cols_alias, cols_types, mask='', mask_alias=[], card=-1):
    return {
        "table"    : table_name,
        "cols_n"   : cols_names,
        "cols_a"   : cols_alias,
        "cols_t"   : cols_types,
        "mask"     : mask,
        "mask_a"   : mask_alias, # mask/mask_a ==> cols_a
        "card"     : card        # cardinality: -1 for unknown
    }

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
    t0 = genWhere(x0)    ; t1 = genWhere(y0)
    t2 = genLength(t0)   ; t3 = genLength(t1)
    t4 = genVector(t3,t0); t5 = genVector(t2,t1)
    return [t4, t5]
    # w2 = genDuplicate(w0); w3 = genReplicate(w1)
    # return [w2, w3]

def genVectorAnd(vec_and):
    size = len(vec_and)
    if size == 1:
        return vec_and[0]
    elif size > 1:
        t0 = genAnd(vec_and[0], vec_and[1])
        for x in vec_and[2:]:
            t0 = genAnd(t0, x)
        return t0
    else:
        unexpected('length 0 found in vec_and')

def genVectorOr(vec_or):
    size = len(vec_or)
    if size == 1:
        return vec_or[0]
    elif size > 1:
        t0 = genOr(vec_or[0], vec_or[1])
        for x in vec_or[2:]:
            t0 = genOr(t0, x)
        return t0
    else:
        unexpected('length 0 found in vec_or')


def findKeepList(d):
    r = []
    for x in d:
        if x['operation'] == 'keep' or x['operation'] == 'any':
            r.append(x['iu'][0])
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
        aaa = findAliasByIndex(source, env)
        # print nam, typ, aaa, indx
        # raw_input()
        if indx == None:
            names.append(nam)
            alias.append(aaa)
            types.append(typ)
        else:
            names.append(nam)
            alias.append(genRaze(genIndex(aaa, indx)))
            types.append(typ)

def addEnvValuesFromVector(names, alias, types, d, vector):
    for c,a in enumerate(d):
        sId = int(a['source'])
        if sId == 4294967295:
            continue
        nam = a['iu'][0]
        typ = strType(a['iu'][1])
        aaa = vector[c]
        names.append(nam)
        alias.append(aaa)
        types.append(typ)


def isOkStr2Sym(tab, col, typ):
    global table_types
    return typ == 'str' and tab in table_types and col in table_types[tab]

def isOkAny2Enum(tab, col):
    return checkForeignKey(tab, col)

#c_custkey2 -> c_custkey
def getMappedKey(key):
    global mapping_key
    return mapping_key[key] if key in mapping_key else key

def checkRelation(left_table, left_name, right_table, right_name):
    left_name  = getMappedKey(left_name)
    right_name = getMappedKey(right_name)
    left_col   = packColumnName(left_name)
    right_col  = packColumnName(right_name)
    if left_table in primary_key and right_table in foreign_key:
        if sameListString(primary_key[left_table],left_col):
            fkeys = foreign_key[right_table]
            for fk in fkeys:
                if fk[1] == left_table and sameListString(fk[0], right_col) and sameListString(fk[2], left_col):
                    return True
    return False

def checkPrimaryKey(table, name):
    # if name == 'l_orderkey2':
    #     print table, getMappedKey(name)
    #     raw_input()
    name = getMappedKey(name)
    return table in primary_key and sameListString(primary_key[table], packColumnName(name))

def checkForeignKey(table, name):
    global foreign_key
    name = getMappedKey(name)
    if table in foreign_key:
        for x in foreign_key[table]:
            if sameListString(x[0], packColumnName(name)):
                return True
    return False

def getCard(d):
    return int(d['cardinality'])

def setProperty(d, env):
    """Q2: earlyprobe has no cardinality"""
    env['card'] = int(d['cardinality']) if 'cardinality' in d else 0
    opId = d['operatorId']
    if getEnvTable(env).startswith('lambda'): # assign a unique ID
        env['table'] = getEnvTable(env) + '_' + str(opId)
    return env

def getEnumValue(x, env):
    if not env:  # empty dict
        return x
    if getAliasType(x,env) == 'enum':
        ind = getEnvAlias(env).index(x)
        getEnvType(env)[ind] = '!'  # set type unknown
        getEnvAlias(env)[ind] = z = genFetch(x)
        return z
    return x

if __name__ == '__main__':
    start = time.time()
    main()
    info('Elapsed time: %.2lf ms' % (1000*(time.time() - start)))

