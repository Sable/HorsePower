"""
Global variables
"""
from codelib import *

code_block = []

line_no = 0
var_num = 0

chain_def = {}
chain_use = {}

mapping_enum  = {} # chf

"""
Conventions
  Single arg
    x
  Two args
    a, b
  Three args
    x, m, n
"""

def insertDef(n):
    global chain_def, line_no
    if n in chain_def:
        errorMsg('key %s already exist in chain_def' % n)
    chain_def[n] = { 'def_line': line_no, 'def_use': [] }

def insertUse(v, n):
    global chain_def, line_no
    if n not in chain_use:
        chain_use[n] = { 'use_line': line_no-1, 'use_def': [] }
    if v.startswith("t"):
        chain_def[v]['def_use'].append(n)
        chain_use[n]['use_def'].append(v)

def genNamebyNum():
    global var_num
    vName = 't%d' % var_num
    var_num = var_num + 1
    insertDef(vName)
    return vName

def genPrint(s):
    global line_no, code_block
    line_no = line_no + 1
    # print s
    code_block.append(s)

def genAssignment(expr,cast=''):
    targ = genNamebyNum()
    if cast:
        genPrint('%s:%s = check_cast(%s, %s);' % (targ,cast,expr,cast))
    else:
        genPrint('%s:? = %s;' % (targ, expr))
    return targ

def genNiladic(description):
    targ = genAssignment('@%s()'%description)
    insertUse('', targ)
    return targ

def genMonadic(description, value):
    targ = genAssignment('@%s(%s)'%(description,value))
    insertUse(value, targ)
    return targ

def genDyadic(description, value_0, value_1, cast=''):
    if description == 'eq' or description == 'neq': # chf
        value_1 = genSymFromString(value_1)
    elif description == 'like':
        value_1 = genStringFromSym(value_1)
    targ = genAssignment('@%s(%s,%s)'%(description,value_0,value_1),cast)
    insertUse(value_0, targ)
    insertUse(value_1, targ)
    return targ

def genTriple(description, value_0, value_1, value_2):
    targ = genAssignment('@%s(%s,%s,%s)'%(description,value_0,value_1,value_2))
    insertUse(value_0, targ)
    insertUse(value_1, targ)
    insertUse(value_2, targ)
    return targ

def genList(value_list):
    targ = genAssignment('@list(%s)' % stringList(value_list))
    for v in value_list:
        insertUse(v, targ)
    return targ

def genLiteral(value_literal):
    targ = genAssignment(value_literal)
    insertUse('', targ)
    return targ

def genCopy(value):
    targ = genAssignment(value)
    insertUse(value, targ)
    if value == 't44':
        print 'targ = %s' % targ
    return targ

def genSort(value_0, value_literal):
    targ = genAssignment('@order(%s,%s)' % (value_0,value_literal))
    insertUse(value_0, targ)
    return targ

def genOuter(operator, value_0, value_1):
    targ = genAssignment('@outer(%s,%s,%s)'%(operator,value_0,value_1))
    insertUse(value_0, targ)
    insertUse(value_1, targ)
    return targ

def genCheckCast(expr, typ):
    return 'check_cast(%s, %s)' % (expr,typ)

###############  Monadic

def genLoadTable(x):
    return genMonadic('load_table', '`'+x+':sym')

def genValues(x):
    return genMonadic('values', x)

def genKeys(x):
    return genMonadic('keys', x)

def genLength(x):
    return genMonadic('len', x)

def genSum(x):
    return genMonadic('sum', x)

def genAvg(x):
    return genMonadic('avg', x)

def genCount(x):
    return genLength(x)

def genGroup(x):
    return genMonadic('group', x)

def genRaze(x):
    return genMonadic('raze', x)

def genNot(x):
    return genMonadic('not', x)

def genMax(x):
    return genMonadic('max', x)

def genMin(x):
    return genMonadic('min', x)

def genRange(x):
    return genMonadic('range', x)

def genWhere(x):
    return genMonadic('where', x)

def genDuplicate(x):
    return genMonadic('dup', x)

def genReplicate(x):
    return genMonadic('rep', x)

def genUnique(x):
    return genMonadic('unique', x)

def genDateYear(x):
    return genMonadic('date_year', x)

def genDateMonth(x):
    return genMonadic('date_month', x)

def genDateDay(x):
    return genMonadic('date_day', x)

def genEnlist(x):
    return genMonadic('enlist', x)

###############  Dyadic 

def genEnum(a, b):
    return genDyadic('enum', a, b)

def genCompress(a, b):
    return genDyadic('compress', a, b)

def genVector(a, b):
    return genDyadic('vector', a, b)

def genEach(a, b):
    return genDyadic('each', a, b)

def genIndex(a, b, cast=''):
    if a in mapping_enum:  # chf
        a = mapping_enum[a]
    return genDyadic('index', a, b, cast)

def genTable(a, b):
    return genDyadic('table', a, b)

def genAnd(a, b):
    return genDyadic('and',a, b)

def genOr(a, b):
    return genDyadic('or',a, b)

def genMul(a, b):
    return genDyadic('mul', a, b)

def genPlus(a, b):
    return genDyadic('plus', a, b)

def genLike(a, b):
    return genDyadic('like', a, b)

def genMember(a, b):
    return genDyadic('member', a, b)

def genGeq(a, b):
    return genDyadic('geq', a, b)

def genGt(a, b):
    return genDyadic('gt', a, b)

def genLeq(a, b):
    return genDyadic('leq', a, b)

def genLt(a, b):
    return genDyadic('lt', a, b)

def genEq(a, b):
    return genDyadic('eq', a, b)

def genNeq(a, b):
    return genDyadic('neq', a, b)

def genSubString(a, b):
    return genDyadic('sub_string', a, b)

def genCons(a, b):
    return genDyadic('cons', a, b)  # concatenate

def genIndexOf(a, b):
    return genDyadic('index_of', a, b)

def genColumnValue(a, b, cast=''):
    rtn = genDyadic('column_value',a,b,cast)
    if cast == 'enum' and (not rtn in mapping_enum): #chf, q16
        mapping_enum[rtn] = genValues(rtn);
    return rtn

###############  >2

def genIndexA(x, m, n):
    return genTriple('index_a', x, m, n) 

def genEachRight(x, m, n):
    return genTriple('each_right', x, m, n)

def genBetween(x, m, n):
    return genAnd(genGeq(x, m), genLeq(x, n))

###############  other helper

def genReturn(x):
    genPrint('return %s;' % x)

def genModuleBegin(module):
    print('module %s{' % module)
    print('  import Builtin.*;')
    print('  def main() : table{')

def genModuleEnd():
    print('  }')
    print('}')

def genArithOps(op):
    return {
        'mul': 'mul',
        'sub': 'minus',
        'add': 'plus',
        'div': 'div'
    }.get(op, op)

def genSymFromString(x):
    if x.startswith('"'):
        return '`' + x + ':sym'
    else:
        return x + ':str'

def genStringFromSym(x):
    if x.startswith('`"'):
        return x[1:] + ':str'
    elif x.startswith('`'):
        return '"' + x[1:] + '"' + ':str'
    else:
        return '**error**'*10

###############

def printAllCode(mask=[], isDebug=False):
    global code_block
    head_lines = 5
    debug('Program slicing (before: %d, after %d)' % (len(code_block)+head_lines, len(mask)+head_lines))
    if not isDebug:
        genModuleBegin('default')
    if mask:
        for m in mask:
            b = code_block[m]
            print (('    %s' % b) if not isDebug else '[%3d] %s' % (m,b))
    else:
        for c,b in enumerate(code_block):
            print (('    %s' % b) if not isDebug else '[%3d] %s' % (c,b))
    if not isDebug:
        genModuleEnd()

def traverseDef():
    global chain_def
    traverseDefSub('t0')

def traverseDefSub(rt):
    global chain_def
    cell = chain_def[rt]
    # print '%s : %s : %s' % (rt, cell['def_line'], cell['def_use'])
    for x in cell['def_use']:
        traverseDefSub(x)

def traverseUse():
    global chain_use,var_num,line_no
    debug('-'*30)
    map_line = {}
    traverseUseSub('t%d' % (var_num-1), map_line)
    line_array = [line_no-1] # always keep the last return stmt
    for k in map_line.keys():
        line_array.append(k)
    # print '-'*30
    # print '%s => size: %d' % (sorted(line_array),len(line_array))
    return sorted(line_array)

def traverseUseSub(rt, m):
    global chain_use
    cell = chain_use[rt]
    lineno = cell['use_line']
    if lineno not in m: m[lineno] = 1
    # print '%-5s : %-5s : %s' % (rt, cell['use_line'], cell['use_def'])
    for x in cell['use_def']:
        traverseUseSub(x, m)

def printVarNum():
    print 'Final var_num = %d' % var_num


