import json, sys, time, copy

sys.path.append("../hyper")

from codegen  import *
from parsing  import *
from datetime import date, timedelta
from util     import *

db_tpch = {}
as_map  = {}

def scanMain(d, env):
    if 'operator' in d:
        op = d['operator']
        print op
        newEnv = scanMain(d['input'], env) if 'input' in d else env
        if   op == 'project'       : rtn = scanProject      (d, newEnv)
        elif op == 'groupby'       : rtn = scanGroupby      (d, newEnv)
        elif op == 'select'        : rtn = scanSelect       (d, newEnv)
        elif op == 'table'         : rtn = scanTable        (d, newEnv)
        elif op == 'top'           : rtn = scanTop          (d, newEnv)
        elif op == 'joinidx'       : rtn = scanJoinidx      (d, newEnv)
        elif op == 'join'          : rtn = scanJoin         (d, newEnv)
        elif op == 'semijoin'      : rtn = scanSemijoin     (d, newEnv)
        elif op == 'antijoin'      : rtn = scanAntijoin     (d, newEnv)
        elif op == 'leftouterjoin' : rtn = scanLeftouterjoin(d, newEnv)
        return rtn
    else:
        unexpected("No operator found.")

def scanProject(d, env):
    return scanOutput(d['output'], env)

def scanGroupby(d, env):
    b1 = d['block1']
    b2 = d['block2']
    if emptyList(b1):
        return scanSimpleBlock(b2, env)
    else:
        unexpected("Support b1")
    pass

def scanSelect(d, env):
    mask = getTableCond(d['conditions'], env)
    return updateEnvWithMask(mask, env)

def scanTable(d, env):
    table         = getTableName (d['table'])
    current_table = genLoadTable (table) # table id
    table_cols    = getTableCols (d['columns'], table)
    table_names   = getTableTabs (table_cols, table)
    table_alias   = getTableAlias(table_cols, current_table, table)
    table_types   = getTableTypes(table_cols, table)
    #print table_cols, table_alias
    #stop('scanTable')
    return encodeEnv(table_names, table_cols, table_alias, table_types)

def scanTop(d, env):
    pass

def scanJoinidx(d, env):
    pass

def scanJoin(d, env):
    pass

def scanSemijoin(d, env):
    pass

def scanAntijoin(d, env):
    pass

def scanLeftouterjoin(d, env):
    pass

def scanOutput(d, env):
    global as_map  # THINK: a real global or clean here?
    env_alias = []; env_table = []; env_cols = []; env_types = []
    for t in d:
        num = len(t)
        if num == 1:
            v0 = getValuesV(t[0], env)
            v1 = t[0]['value']['id']
        elif num == 2:
            if t[1]['type'] == 'as':
                typ = getExprType(t[0])
                v0 = getValuesV(t[0], env)
                v1 = getValuesV(t[1], env) #id0.id1
                as_map[v1[1]] = v0
            else:
                unexpected('Need "as"')
        else:
            unexpected('Add support num: %d' % num)
        #print v0,v1
        tab,col = v1
        env_alias.append(v0)
        env_table.append(tab)
        env_cols.append(col)
        env_types.append('myType')
    return encodeEnv(env_table, env_cols, env_alias, env_types)

def scanSimpleBlock(d, env):
    printEnv(env)
    newEnv = scanOutput(d, env)
    printEnv(newEnv)
    stop('here')
    return newEnv

# return final results
def scanHeader(env):
    # printEnv(env)
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

def getIdName(name, first='sys'):
    id = name['id']
    if id[0] == first:
        return id[1]
    else:
        unexpected('Not a valid %s.<name>' % first)

def getTableName(names):
    return getIdName(names)

def getTableCols(cols, table):
    rtn = []
    for col in cols:
        # maybe: check col's type
        rtn.append(getIdName(col['value'], table))
    return rtn

def getTableAlias(cols, table_alias, table_name):
    global db_tpch
    rtn = []
    for col in cols:
        rtn.append(genColumnValue(table_alias, '`'+col+':sym', db_tpch[table_name][col]))
    return rtn

def getTableTabs(cols, table):
    rtn = []
    for col in cols:
        rtn.append(table)
    return rtn

def getTableTypes(cols, table):
    rtn = []
    for col in cols:
        rtn.append('myType')   # pesudo type, fix later
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

def getDecimal(v, env):
    return getLiteralValue(v, env)

def getLiteralTiny(v, env):
    return getLiteralValue(v, env)

def getNameVar(v, env):
    global as_map
    if 'id' in v['value']:
        id0, id1 = v['value']['id'] # id0.id1
    else:
        id1 = v['value']  # type: string (i.e. var)
    if id1 in as_map:
        return as_map[id1]
    else:
        indx = getEnvName(env).index(id1)
        if indx >= 0 :
            if getEnvTable(env)[indx] == id0:
                return getEnvAlias(env)[indx]
            else:
                unexpected('column %s need table %s, but %s found' % (id1,id0,getEnvTable(env)[indx]))
        else:
            unexpected('column %s not found' % id1)

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

def getExprValue(v, env):
    def checkFunc(func, args, num):
        if len(args) == num:
            return {
                "sql_add" : "plus" ,
                "sql_sub" : "minus",
                "sql_mul" : "mul"  ,
                "sum"     : "sum"
            }.get(func, "<func>")
        else:
            unexpected("Argument inbalanced: %d vs. %d" % (len(args),num))

    def isBinaryFunc(func):
        binary_op_list = [ 'sql_add', 'sql_sub', 'sql_mul' ]
        return (func in binary_op_list)

    def isUnaryFunc(func):
        unary_op_list = [ 'sum' ]
        return (func in unary_op_list)

    expr = v['value']
    func = getIdName(expr['function'])
    if isBinaryFunc(func):
        op = checkFunc(func, expr['args'], 2)
        v0 = getValuesV(expr['args'][0], env)
        v1 = getValuesV(expr['args'][1], env)
        return genDyadic(op, v0, v1)
    elif isUnaryFunc(func):
        op = checkFunc(func, expr['args'], 1)
        v0 = getValuesV(expr['args'][0], env)
        return genMonadic(op, v0)
    else:
        unexpected("Support func %s" % func)
    pass

def getAs(v, env):
    return v['value']['id']

def getValuesV(v, env):
    if 'type' in v:
        t = v['type']
        if t == 'date':
            return getLiteralDate(v, env)
        elif t == 'decimal':
            return getDecimal(v, env)
        elif t == 'tinyint':
            return getLiteralTiny(v, env)
        elif t == 'month_interval':
            return getIntervalMonth(v, env)
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
        else:
            unexpected('Add support for %s' % t)
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
        "decimal": "float",
        "date"   : "date"
    }.get(d['type'], "unknownType")

def getExpr(expr, env):
    def genExpr3(d):
        v0 = getValuesV(d[0], env)
        v1 = getValuesV(d[1], env) # op1
        v2 = getValuesV(d[2], env)
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


def compile(src):
    initDatabase()
    scanHeader(scanMain(src, {})) # {} dict
    printAllCode()

