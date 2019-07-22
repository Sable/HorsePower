import json, sys, time, copy

sys.path.append("../hyper")

from codegen  import *
from parsing  import *
from datetime import date, timedelta
from util     import *

db_tpch      = {}
as_map       = {}  # id0.id1: currently only use id1 as key
primary_key  = {}
foreign_key  = {}

def scanMain(d, env):
    if 'operator' in d:
        op = d['operator']
        print op
        if 'input' in d:
            child = d['input']
            newEnvs = []
            if isinstance(child, list):
                for t in child:
                    newEnvs.append(scanMain(t, env))
            else:
                newEnvs.append(scanMain(child, env))
            num = len(newEnvs)
            if num == 1:
                env1 = newEnvs[0]
            elif num == 2:
                env2 = newEnvs
            else:
                unexpected("Support envs more than 2: %d" % num)
        else:
            env1 = env
        if   op == 'project'       : rtn = scanProject      (d, env1)
        elif op == 'groupby'       : rtn = scanGroupby      (d, env1)
        elif op == 'select'        : rtn = scanSelect       (d, env1)
        elif op == 'table'         : rtn = scanTable        (d, env1)
        elif op == 'top'           : rtn = scanTop          (d, env1)
        elif op == 'joinidx'       : rtn = scanJoinidx      (d, env2)
        elif op == 'join'          : rtn = scanJoin         (d, env2)
        elif op == 'semijoin'      : rtn = scanSemijoin     (d, env2)
        elif op == 'antijoin'      : rtn = scanAntijoin     (d, env2)
        elif op == 'leftouterjoin' : rtn = scanLeftouterjoin(d, env2) # only q13
        return rtn
    else:
        print json.dumps(d)
        unexpected("No operator found.")

def scanProject(d, env):
    newEnv = scanOutput(d['output'], env)
    if 'order' in d:
        return scanOrder(d['order'], newEnv)
    else:
        return newEnv

def scanGroupby(d, env):
    b1 = d['block1']
    b2 = d['block2']
    if emptyList(b1):
        env2 = scanBlock(b2, env)
        return combineGroupEnv(None, env2)
    else:
        env2 = scanBlock(b2, env)
        env1 = scanBlock(b1, env)
        return combineGroupEnv(env1, env2)
    pass

def scanSelect(d, env):
    mask = getTableCond(d['conditions'], env)
    return updateEnvWithMask(mask, env)

def scanTable(d, env):
    table       = getTableName (d['table'])
    table_alias = genLoadTable (table) # table id
    table_cols  = getTableCols (d['columns'], table)
    table_names = getTableTabs (table_cols, table)
    table_types  = getTableTypes(table_cols, table)
    table_alias  = getTableAlias(table_alias, table_cols, table_types)
    #print table_cols, table_alias
    #stop('scanTable')
    return encodeEnv(table_names, table_cols, table_alias, table_types)

def scanTop(d, env):
    todo('top')
    pass

def scanJoinidx(d, env):
    todo('join index')
    pass

"""
TODO: understand how to interpret conditions for joins
"""
def scanJoin(d, env2):
    printEnv(env2[0])
    printEnv(env2[1])
    print d
    todo('join')
    pass

def scanSemijoin(d, env2):
    #printEnv(env0)
    #printEnv(env1)
    #print d['condition']
    cond = d['condition']
    num = len(cond)
    if num == 3: # binary + single condition
        return handleSemiJoin(cond, env2)
    else:
        unexpected("Support num != 3: %d" % num)
    pass

def scanAntijoin(d, env):
    todo('antijoin')
    pass

def scanLeftouterjoin(d, env):
    # only q13.
    left_env, right_env = env
    printEnv(left_env)
    printEnv(right_env)
    todo('left outerjoin')
    pass

def scanOutput(d, env):
    return scanBlock(d, env)

def scanBlock(d, env):
    global as_map  # THINK: a real global or clean here? as_map.clear()
    env_alias = []; env_table = []; env_cols = []; env_types = []; env_maska = []
    for t in d:
        num = len(t)
        if num > 0 and isinstance(t[0], list):
            # vector for asc/desc
            for vec in t:
                for d in vec:
                    if 'order' in d:
                        v0 = getValuesV(d, env)
                        indx = findIdInEnv(d['value']['id'], env)
                    else:
                        unexpected('unknown')
                    env_alias.append({"kind":"sort", "values":v0})
                    env_table.append(getEnvTable(env)[indx])
                    env_cols.append(getEnvName(env)[indx])
                    env_types.append(getEnvType(env)[indx])
                    env_maska.append(getEnvMaskA(env)[indx])
            continue
        if num == 1:
            v0 = getValuesV(t[0], env)
            v1 = t[0]['value']['id']
            tab,col = v1
            indx  = findIdInEnv(v1, env)
            typ   = getEnvType(env)[indx]
            maska = getEnvMaskA(env)[indx]
            #typ = getEnvType(env)[indx] #typ = getTypeFromEnv(v1, env)
            #maska = getEnvMaskA(env)[indx]
        elif num == 2:
            if t[1]['type'] == 'as':
                typ = getExprType(t[0])
                v0 = getValuesV(t[0], env)
                v1 = getValuesV(t[1], env) #id0.id1
                tab,col = v1
                maska = None  # unknown currently
                if not isinstance(v0, dict):
                    as_map[v1[1]] = v0
            else:
                unexpected('Need "as"')
        else:
            unexpected('Add support num: %d' % num)
        #print v0,v1
        env_alias.append(v0)  # could be dict in aggregation
        env_table.append(tab)
        env_cols.append(col)
        env_types.append(typ)
        env_maska.append(maska)
    # keep mask and maska
    return encodeEnv(env_table, env_cols , env_alias, env_types, getEnvMask(env), env_maska)

def scanOrder(d, env):
    order_alias = []
    order_sort  = []
    for vec in d:
        for d in vec:
            if 'order' in d:
                order_alias.append(getValuesV(d, env))
                order_sort.append(str2bool(d['order']))
            else:
                unexpected('unknown')
    a0 = genSort(genList(order_alias), strLiterals(order_sort, 'bool'))
    return updateEnvWithIndex(a0, env)

# return final results
def scanHeader(env):
    printEnv(env)
    stop('header')
    a0 = genLiteral(strSymbolVec(getEnvName(env)))
    a1 = genList(getEnvAlias(env))
    a2 = genTable(a0, a1)
    genReturn(a2)

"""
Helper functions defined below
"""

def encodeEnv(table_names, cols_names, cols_alias, cols_types, mask='', mask_alias=[]):
    return {
        "tables" : table_names,
        "cols_n" : cols_names,
        "cols_a" : cols_alias,
        "cols_t" : cols_types,
        "mask"   : mask,
        "mask_a" : mask_alias  # mask/mask_a ==> cols_a
    }

def updateEnvWithMask(mask, env):
    alias = actionCompress(mask, env['cols_a'], env)
    return encodeEnv(getEnvTable(env),
                     getEnvName(env),
                     alias,
                     getEnvType(env),
                     mask,
                     getEnvAlias(env)) #update alias

def updateEnvWithAlias(new_alias, env):
    new_env = copy.deepcopy(env)
    new_env['cols_a'] = new_alias
    return new_env

def updateEnvWithIndex(indx, env):
    alias = getEnvAlias(env)
    new_alias = []
    for x in alias:
        new_alias.append(genIndex(x, indx))
    return updateEnvWithAlias(new_alias, env)

def updateEnvWithMaskMask(mask, env):
    alias = actionCompress(mask, getEnvMaskA(env), env)
    return encodeEnv(getEnvTable(env),
                     getEnvName(env),
                     alias,
                     getEnvType(env),
                     mask,
                     getEnvMaskA(env))

def updateEnvWithInfo(info, env2):
    def updateEnvWithInfoMask():
        if info0 and info1:
            todo('Support')
        elif info0:
            # case semi-join: [p0, None, 'masking']
            printEnv(env0)
            print info0
            return updateEnvWithMaskMask(info0, env0)
        elif info1:
            return updateEnvWithMaskMask(info1, env1)
        else:
            unexpected("Both sides are none")
    def updateEnvWithInfoKeep():
        if info0 and info1:
            todo('keep and merge both')
        elif info0:
            return env0
        elif info1:
            return env1
        else:
            unexpected("Both sides are none")
    # start
    info0, info1, kind = info
    env0,env1 = env2
    # masking/keep/indexing/...
    if kind == 'masking':
        return updateEnvWithInfoMask()
    elif kind == 'keep':
        return updateEnvWithInfoKeep()
    else:
        todo('support kind: %s' % kind)

def actionCompress(vid, cols, env):
    alias = []
    for c in cols:
        a = genCompress(vid, getEnumValue(c, env))
        alias.append(a)
    return alias

def getEnumValue(x, env):
    if not env:  # empty dict
        return x
    if getAliasType(x,env) == 'enum':
        ind = getEnvAlias(env).index(x)
        getEnvType(env)[ind] = '!'  # set type unknown
        getEnvAlias(env)[ind] = z = genFetch(x)
        return z
    else:
        return x

def isName(x):
    return True if 'type' in x and x['type'] == 'name' else False

def isProperty(x, p):
    return True if 'property' in x and x['property'] == p else False

def getIdName(name, first='sys'):
    id0,id1 = name['id']
    if id1 == '%TID%':
        return None
    if id0 == first:
        return id1
    else:
        unexpected('Not a valid %s.<name>' % first)

def getTableName(names):
    return getIdName(names)

def getTableCols(cols, table):
    """
    Ignore any column field if it does not have a 'name' field
    """
    rtn = []
    for col in cols:
        # maybe: check col's type
        if isName(col) and not isProperty(col, 'hashidx'):
            id = getIdName(col['value'], table)
            if id:
                rtn.append(id)
    return rtn

def getTableAlias(table_alias, cols, types):
    rtn = []
    for c,col in enumerate(cols):
        rtn.append(genColumnValue(table_alias, '`'+col+':sym', types[c]))
    return rtn

def getTableTabs(cols, table):
    rtn = []
    for col in cols:
        rtn.append(table)
    return rtn

def getTableTypes(cols, table):
    rtn = []
    for col in cols:
        if isOkAny2Enum(table, col):
            rtn.append('enum')
        else:
            rtn.append(db_tpch[table][col])
    return rtn

# delete
def getExprOp(expr, kind):
    if expr['type'] == kind:
        return expr['value']
    else:
        unexpected('Require kind %s, but %s found'%(kind,expr['type']))

def getLiteralValue(v, env):
    return getValuesV(v['value'], env)

def getLiteralDate(v, env):
    # format: YYYY-MM-DD
    return ("'%s':date" % getLiteralValue(v, env))

def getIntervalMonth(v, env):
    return getLiteralValue(v, env)

def getIntervalSec(v, env):
    warning('Need check interval')
    return getLiteralValue(v, env)

def getDecimal(v, env):
    return getLiteralValue(v, env)

def getDouble(v, env):
    return getLiteralValue(v, env)

def getLiteralTiny(v, env):
    return getLiteralValue(v, env)

def getNameVar(v, env):
    global as_map
    if 'id' in v['value']:
        id0, id1 = v['value']['id'] # id0.id1
    else:
        id0 = None
        id1 = v['value']  # type: string (i.e. var)
    if id1 in as_map:
        return as_map[id1]
    else:
        indx = getEnvName(env).index(id1)
        if indx >= 0 :
            if id0 == None or getEnvTable(env)[indx] == id0:
                return getEnvAlias(env)[indx]
            else:
                unexpected('column %s need table %s, but %s found' % (id1,id0,getEnvTable(env)[indx]))
        else:
            unexpected('column %s not found' % id1)

def getItemClob(v, env):
    val = v['value']
    typ = val['type']
    if typ == 'string':
        if len(val['value']) == 0:
            return None
        else:
            return ("'%s':str" % val['value'])  # gen const string
    elif typ == 'char':
        return getValuesV(val['value'], env)
    else:
        unexpected('unknown type: %s' % typ)

def getFilter(v, env):
    op = v['value']
    return {
        "like": "like"
    }.get(op, "<unkown filter: %s" % op)

def getBinaryOp(v, env):
    op = v['value']
    return {
        "<=": "leq",
        ">=": "geq",
        "<" : "lt" ,
        ">" : "gt" ,
        "=" : "eq" ,
        "!=": "neq"
        }.get(op, "<unknown binary op: %s" % op)

def getArgValues(args, env):
    global as_map
    rtn = []
    prev = None
    for x in args:
        typ = x['type']
        if typ == 'as':
            if prev:
                t = getValuesV(x, env)
                as_map[t[1]] = prev
                prev = None
            else:
                unexpected('Previous node not found for as')
        else:
            prev = getValuesV(x, env)
            rtn.append(prev)
    stop(rtn)
    return rtn

def getExprValue(v, env):
    def checkFunc(func, args, num):
        if len(args) == num:
            return {
                "sql_add" : "plus" ,
                "sql_sub" : "minus",
                "sql_mul" : "mul"  ,
                "sum"     : "sum"  ,
                "avg"     : "avg"  ,
                "count"   : "count"
            }.get(func, "<func>")
        else:
            print args
            unexpected("Argument inbalanced: %d vs. %d" % (len(args),num))

    def isBinaryFunc(func):
        binary_op_list = [ 'sql_add', 'sql_sub', 'sql_mul' ]
        return (func in binary_op_list)

    def isUnaryFunc(func):
        unary_op_list = [ 'sum', 'avg' ]
        return (func in unary_op_list)

    def isNilFunc(func):
        nil_op_list = [ 'count' ]
        return (func in nil_op_list)

    def isReduction(func):
        reduction_op_list = [ 'sum', 'avg', 'count' ]
        return (func in reduction_op_list)

    # start
    expr = v['value']
    func = getIdName(expr['function'])
    args = getArgValues(expr['args'], env)
    if isBinaryFunc(func):
        op = checkFunc(func, args, 2)
        v0 = args[0]
        v1 = args[1]
        return genDyadic(op, v0, v1)
    elif isUnaryFunc(func):
        op = checkFunc(func, args, 1)
        v0 = args[0]
        if isReduction(op):
            return { "kind": "aggr", "op": op, "num": 1, "values": [v0] }
        else:
            return genMonadic(op, v0)
    elif isNilFunc(func):
        op = checkFunc(func, args, 0)
        if isReduction(op):
            return { "kind": "aggr", "op": op, "num": 0, "values": [] }
        else:
            return genNiladic(op)
    else:
        unexpected("Support func %s" % func)
    pass

def getAs(v, env):
    return v['value']['id']

def getLiteralChar(v, env):
    return ("'%s'" % v['value'])

def getValuesV(v, env):
    if 'type' in v:
        t = v['type']
        if t == 'date':
            return getLiteralDate(v, env)
        elif t == 'decimal':
            return getDecimal(v, env)
        elif t == 'double':
            return getDouble(v, env)
        elif t == 'tinyint':
            return getLiteralTiny(v, env)
        elif t == 'month_interval':
            return getIntervalMonth(v, env)
        elif t == 'sec_interval':
            return getIntervalSec(v, env)
        elif t == 'name':
            return getNameVar(v, env)
        elif t == 'binary_op':
            return getBinaryOp(v, env)
        elif t == 'expr':
            return getExprValue(v, env)
        elif t == 'as':
            return getAs(v, env)
        elif t == 'string':   # variable
            return getNameVar(v, env) 
        elif t == 'char':
            return getLiteralChar(v, env)
        elif t == 'clob':
            return getItemClob(v, env)
        elif t == 'filter':
            return getFilter(v, env)
        else:
            print v
            unexpected('Add support for %s' % t)
    elif isinstance(v, list):
        items = getArgValues(v, env) # q13
        num = len(items)
        if num == 1:
            return items[0]
        elif num == 2:
            if not items[1]:
                return items[0]
        cellType = v[0]['type']
        if cellType == 'char':
            return strLiterals(items, 'str')
        else:
            unexpected('Unknown case: %s' % v)
    else:
        return v

def genArrayWithOp(cell, op):
    numCell = len(cell)
    if numCell == 1:
        return cell[0]
    elif numCell > 1:
        rtn = genDyadic(op, cell[0], cell[1]);
        for c in cell[2:]:
            rtn = genDyadic('and', rtn, c)
        return rtn
    else:
        unexpected('Size of array must > 0')

def getExprType(d):
    return {
        "decimal": "f32",
        "date"   : "date"
    }.get(d['type'], "?")

def getTypeFromEnv(d, env):
    id0, id1 = d
    idx = getEnvName(env).index(id1)
    if idx >= 0:
        if id0 == getEnvTable(env)[idx]:
            return getEnvType(env)[idx]
        else:
            unexpected('table needs %s, but %s found' % (id0, getEnvTable(env)[idx]))
    else:
        unexpected('%s.%s not found' % (id0,id1))

def getExpr(expr, env):
    def genExpr3(d):
        v0 = getValuesV(d[0], env)
        v1 = getValuesV(d[1], env) # op1
        v2 = getValuesV(d[2], env)
        if 'neg' in d[1] and d[1]['neg'] == 1:
            return genNot(genDyadic(v1, v0, v2))
        else:
            return genDyadic(v1, v0, v2)
    def genExpr5(d):
        v0 = getValuesV(d[0], env)
        v1 = getValuesV(d[1], env) # op1
        v2 = getValuesV(d[2], env)
        v3 = getValuesV(d[3], env) # op2
        v4 = getValuesV(d[4], env)
        return genDyadic(v3, genDyadic(v1, v0, v2), v4)
    # 3/5 parameters
    num = len(expr)
    if num == 3:
        return genExpr3(expr)
    elif num == 5:
        return genExpr5(expr)
    else:
        unexpected('Support num: %d' % num)

def getTableCond(conds, env):
    cell = []
    for cond in conds:
        cell.append(getExpr(cond, env))
    return genArrayWithOp(cell, 'and')

"""
Init
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
    pass

def initDatabase():
    global db_tpch
    # TODO: change str to sym for some columns
    db_tpch['nation'] = {
        'n_nationkey'     : 'i32',
        'n_name'          : 'sym',
        'n_regionkey'     : 'i32',
        'n_comment'       : 'str'
    }
    db_tpch['region'] = {
        'r_regionkey'     : 'i32',
        'r_name'          : 'sym',
        'r_comment'       : 'str'
    }
    db_tpch['part'] = {
        'p_partkey'       : 'i32',
        'p_name'          : 'str',
        'p_mfgr'          : 'str',
        'p_brand'         : 'sym',
        'p_type'          : 'sym',
        'p_size'          : 'i32',
        'p_container'     : 'sym',
        'p_retailprice'   : 'f32',
        'p_comment'       : 'str'
    }
    db_tpch['supplier'] = {
        's_suppkey'       : 'i32',
        's_name'          : 'str',
        's_address'       : 'str',
        's_nationkey'     : 'i32',
        's_phone'         : 'str',
        's_acctbal'       : 'f32',
        'p_comment'       : 'str'
    }
    db_tpch['partsupp'] = {
        'ps_partkey'      : 'i32',
        'ps_suppkey'      : 'i32',
        'ps_availqty'     : 'i32',
        'ps_supplycost'   : 'f32',
        'ps_comment'      : 'str'
    }
    db_tpch['customer'] = {
        'c_custkey'       : 'i32',
        'c_name'          : 'sym',
        'c_address'       : 'str',
        'c_nationkey'     : 'i32',
        'c_phone'         : 'str',
        'c_acctbal'       : 'f32',
        'c_mktsegment'    : 'sym',
        'c_comment'       : 'str'
    }
    db_tpch['orders'] = {
        'o_orderkey'      : 'i32' ,
        'o_custkey'       : 'i32' ,
        'o_orderstatus'   : 'char',
        'o_totalprice'    : 'f32' ,
        'o_orderdate'     : 'date',
        'o_orderpriority' : 'sym' ,
        'o_clerk'         : 'str' ,
        'o_shippriority'  : 'i32' ,
        'o_comment'       : 'str'
    }
    db_tpch['lineitem'] = {
        'l_orderkey'      : 'i32' ,
        'l_partkey'       : 'i32' ,
        'l_suppkey'       : 'i32' ,
        'l_linenumber'    : 'i32' ,
        'l_quantity'      : 'f32' ,
        'l_extendedprice' : 'f32' ,
        'l_discount'      : 'f32' ,
        'l_tax'           : 'f32' ,
        'l_returnflag'    : 'char',
        'l_linestatus'    : 'char',
        'l_shipdate'      : 'date',
        'l_commitdate'    : 'date',
        'l_receiptdate'   : 'date',
        'l_shipinstruct'  : 'sym' ,
        'l_shipmode'      : 'sym' ,
        'l_comment'       : 'str'
    }

def getEnvId(x, env2):
    if isName(x):
        id0,id1 = x['value']['id']
        env0, env1 = env2
        if id1 in getEnvName(env0) and id0 == getEnvTable(env0)[getEnvName(env0).index(id1)]:
            return 0
        if id1 in getEnvName(env1) and id0 == getEnvTable(env1)[getEnvName(env1).index(id1)]:
            return 1
    unexpected("Unknown x: %s" % x)

def findIdInEnv(id, env):
    id0,id1 = id
    tables = getEnvTable(env)
    names  = getEnvName(env)
    for c,x in enumerate(names):
        if x == id1 and tables[c] == id0:
            return c
    todo('Not found %s.%s' % (id0,id1))

def handleSemiJoin(cond, env2):
    def genSemiJoinSides(id0, id1, left_env, right_env, side):
        printRelation(id0, id1)
        mask0 = getEnvMask(left_env)
        mask1 = getEnvMask(right_env)
        index0 = findIdInEnv(id0,  left_env)
        index1 = findIdInEnv(id1, right_env)
        if mask0 and mask1:
            alias0 = getEnvMaskA(left_env)[index0]
            alias1 = getEnvMaskA(right_env)[index1]
            # print alias0, alias1, mask0, mask1
            if side == 'key-to-fkey':
                # alias0 (key), alias1 (fkey)
                a0 = genValues(alias1)
                a1 = genCompress(mask1, a0)
                a2 = genVector(genLength(mask0), '0:bool')
                a3 = genIndexA(a2, a1, '1:bool')
                p0 = genAnd(mask0, a3)
            else:
                # alias0 (fkey), alias1 (key)
                a0 = genValues(alias0)
                a1 = genIndex(mask1, a0)
                p0 = genAnd(mask0, a1)
        elif mask0:
            alias0 = getEnvMaskA(left_env)[index0]
            alias1 = getEnvAlias(right_env)[index1]
            if side == 'key-to-fkey':
                # alias0 (key), alias1 (fkey)
                a0 = genVector(genLength(mask0), '0:bool')
                a2 = genIndexA(a1, alias1, '1:bool')
                p0 = genAnd(mask0, a1)
            else:
                # alias0 (fkey), alias1 (key)
                p0 = mask0
        elif mask1:
            alias0 = getEnvAlias(left_env)[index0]
            alias1 = getEnvMaskA(right_env)[index1]
            if side == 'key-to-fkey':
                # alias0 (key), alias1 (fkey)
                a0 = genCompress(mask1, alias0)
                a1 = genVector(genLength(alias0), '0:bool')
                p0 = genIndexA(a1, a0, '1:bool')
            else:
                # alias0 (fkey), alias1 (key)
                a0 = genValue(alias0)
                p0 = genIndex(mask1, a0)
        else:
            # no mask for both sides
            if side == 'key-to-fkey':
                return ['left', None, 'keep']   # take left_env
            else:
                return [None, 'right', 'keep']  # take right_env
        return [p0, None, 'masking']

    def genSemiJoinGeneral(id0, id1, left_env, right_env):
        mask0 = getEnvMask(left_env)
        mask1 = getEnvMask(right_env)
        index0 = findIdInEnv(id0, left_env)
        index1 = findIdInEnv(id1, right_env)
        alias0 = getEnvAlias(left_env)[index0]
        alias1 = getEnvAlias(right_env)[index1]
        p0 = genMember(alias0, alias1) # left rule
        return [p0, None, 'masking']

    arg0,op,arg1 = cond
    indx0 = getEnvId(arg0, env2)
    indx1 = getEnvId(arg1, env2)
    left_env, right_env = env2
    if indx0 != indx1:
        if indx0 == 1: # swap
            arg0, arg1 = [arg1, arg0]
    else:
        unexpected("Columns %s and %s come from the same table" % (arg0, arg1))
    horse_op = op['value']
    alias0 = getValuesV(arg0,  left_env)
    alias1 = getValuesV(arg1, right_env)
    id0 = getNameId(arg0)
    id1 = getNameId(arg1)
    if horse_op == '=':
        if checkRelationWithId(id0, id1):
            info = genSemiJoinSides(id0, id1, left_env, right_env, 'key-to-fkey')
        elif checkRelationWithId(id1, id0):
            info = genSemiJoinSides(id0, id1, left_env, right_env, 'fkey-to-key')
        else:
            info = genSemiJoinGeneral(id0, id1, left_env, right_env)
    else:
        unexpected('Support non-equal: %s' % horse_op)
    return updateEnvWithInfo(info, env2)

def checkRelationWithId(left, right):
    return checkRelation(left[0], left[1], right[0], right[1])

def checkRelation(left_table, left_name, right_table, right_name):
    global primary_key, foreign_key
    left_name  = left_name
    right_name = right_name
    left_col   = packColumnName(left_name)
    right_col  = packColumnName(right_name)
    if left_table in primary_key and right_table in foreign_key:
        if sameListString(primary_key[left_table],left_col):
            fkeys = foreign_key[right_table]
            for fk in fkeys:
                if fk[1] == left_table and sameListString(fk[0], right_col) and sameListString(fk[2], left_col):
                    return True
    return False

def isOkAny2Enum(tab, col):
    return checkForeignKey(tab, col)

def checkForeignKeyWithID(id): # id: table.col
    return checkForeignKey(id[0], id[1])

def checkForeignKey(table, name):
    global foreign_key
    if table in foreign_key:
        for x in foreign_key[table]:
            if sameListString(x[0], packColumnName(name)):
                return True
    return False

def combineGroupEnv(env1, env2):
    def genCodeNormal(x):
        op  = x['op']
        val = x['values']
        num = x['num']
        if num == 1:
            return genMonadic(op, val[0])
        elif num == 2:
            return genDyadic(op, val[0], val[1])
        elif num == 0:
            return genNiladic(op)
        else:
            unexpected("unknown ...")
    def genCodeVector():
        alias2 = getEnvAlias(env2)
        new_alias = []
        for x in alias2:
            if isinstance(x, dict):
                new_alias.append(genCodeNormal(x))
            else:
                unexpected("unknown")
        return new_alias
    def genCodeEach(x, indx):
        op  = x['op']
        val = x['values']
        if op == 'sum':
            a0 = genEachRight('@index', val[0], indx)
            a1 = genEach     ('@sum'  , a0)
            r0 = genRaze     (a1)
        elif op == 'avg':
            a0 = genEachRight('@index', val[0], indx)
            a1 = genEach     ('@avg'  , a0)
            r0 = genRaze     (a1)
        elif op == 'count':
            a0 = genEach     ('@len'  , indx)
            r0 = genRaze     (a0)
        else:
            unexpected("Support %s" % op)
        return r0
    def genCodeReduction(key, val):
        alias2 = getEnvAlias(env2)
        new_alias = []
        for x in alias2:
            if isinstance(x, dict):
                new_alias.append(genCodeEach(x, val))
            else:
                new_alias.append(genIndex(x, key))
        return new_alias
    if env1 != None:
        # ...
        printEnv(env1)
        printEnv(env2)
        t0 = genList(getEnvAlias(env1))
        t1 = genGroup(t0)
        t2 = genKeys(t1)
        t3 = genValues(t1)
        return updateEnvWithAlias(genCodeReduction(t2, t3), env2)
    else:
        # printEnv(env2)
        return updateEnvWithAlias(genCodeVector(), env2)

def init():
    initDatabase()
    initRelations()

def compile(src):
    init()
    scanHeader(scanMain(src, {})) # {} dict
    printAllCode()

