"""
Helper functions
"""

import sys, traceback
import copy
import json

# I/O operations
def readLines(filename,typ='.json'):
    return "".join([trim(x) for x in open(filename+typ,'r').readlines()])

def trim(text):
    return text.strip(' \r\n')

def debug(msg=''):
    sys.stderr.write('// %s\n' % msg)
    # print '// %s' % msg

def info(msg=''):
    sys.stdout.write('// %s\n' % msg)

def m2p(c):
    return {
        '[': 'geq',
        '(': 'gt',
        ']': 'leq',
        ')': 'lt',
        '<': 'lt',
        '>': 'gt',
        '=': 'eq',
        '>=': 'geq',
        '<=': 'leq',
        '<>': 'neq',
        'is': 'eq' # q17
    }.get(c, '')

def fixMode(c):
    return {
        '<' : '>' ,
        '<=': '>=',
        '>' : '<' ,
        '>=': '<='
    }.get(c, c)

def isArithOps(op):
    return {
        'mul' : True,
        'sub' : True,
        'add' : True,
        'div' : True
    }.get(op, False)

def isLogicOps(op):
    return {
        'and': True,
        'or' : True
    }.get(op, False)

def isUnaryOps(op):
    return {
        'not': True
    }.get(op, False)

def readFromStdin():
    lines = []
    for line in sys.stdin:
        lines.append(line)
    return lines

# def stringLiteral(x, sep=','):
#     literal_set = x['const_value']
#     literal_typ = x['const_type']
#     temp = ''
#     if isinstance(literal_set, list):
#         for (i,v) in enumerate(literal_set):
#             if i > 0: temp = temp + sep
#             temp = temp + str(v)
#     else:
#         temp = str(v)
#     return temp

def stringList(x, sep=','):
    temp  = ''
    for (i,v) in enumerate(x):
        if i > 0: temp = temp + sep
        temp = temp + str(v)
    return temp

def strType(t):
    if len(t) > 0:
        return {
            'Integer': 'i64' ,
            'BigInt' : 'i64' ,
            'Numeric': 'f64' ,
            'Char1'  : 'char',
            'Date'   : 'd'   ,
            'Char'   : 'sym' , # str  -> sym
            'Varchar': 'sym'   # char -> sym
        }.get(t[0], ('<Invalid type %s>' % t[0]))
    else:
        raise ValueError(('Invalid type %s found' % t))

def getEnvTable(env): return env['table']
def getEnvName (env): return env['cols_n']
def getEnvAlias(env): return env['cols_a']
def getEnvType (env): return env['cols_t']
def getEnvMask (env): return env['mask']
def getEnvMaskA(env): return env['mask_a']
def getEnvCard (env): return env['card']

def isEnv2(env): return isinstance(env, list)
def isDict(x): return isinstance(x, dict)
def isList(x): return isinstance(x, list)


def printEnv(env):
    print 'Environment node: {name: "%s", mask: "%s", card: "%d"}' % (getEnvTable(env), getEnvMask(env), getEnvCard(env))
    alias = env['cols_a']
    names = env['cols_n']
    types = env['cols_t']
    maska = env['mask_a']
    if getEnvMask(env):
        for x in range(len(alias)):
            print ' %-16s : %-5s -> %3s (%3s)' % (names[x],types[x],alias[x],maska[x])
    else:
        for x in range(len(alias)):
            print ' %-16s : %-5s -> %3s' % (names[x],types[x],alias[x])

def getAliasType(n, env):
    if not n in getEnvAlias(env):
        # TODO: may improve later
        return n
    ind = getEnvAlias(env).index(n)
    if ind < 0:
        unexpected('name not found: %s' % n)
    return getEnvType(env)[ind]

def findAliasByName(n, env):
    names = env['cols_n']
    if n in names:
        return env['cols_a'][names.index(n)]
    else:
        unexpected('name %s not found in env' % n)

def findAliasByName2(n, env2):
    for c,e in enumerate(env2):
        if n in getEnvName(e):
            return c,getEnvAlias(e)[getEnvName(e).index(n)]
    unexpected('name %s not found in %d env' % (n,len(env2)))

def findSideByName2(n, env2):
    for c,e in enumerate(env2):
        if n in getEnvName(e): return c
    unexpected('name %s not found in %d env' % (n,len(env2)))

def findMaskAliasByName(n, env):
    if n in getEnvName(env):
        if getEnvMask(env):
            return getEnvMaskA(env)[getEnvName(env).index(n)]
        else:
            return getEnvAlias(env)[getEnvName(env).index(n)]

def findMaskAliasByName2(n, env2):
    for c,e in enumerate(env2):
        if n in getEnvName(e):
            if getEnvMask(e):
                return c,getEnvMaskA(e)[getEnvName(e).index(n)]
            else:
                return c,getEnvAlias(e)[getEnvName(e).index(n)]
    unexpected('name %s not found in %d env' % (n,len(env2)))


def findAliasByIndex(d, env):
    names = env['cols_n']
    if d >= 0 and d < len(names):
        return env['cols_a'][d]
    else:
        unexpected('indexing error: %d not in [0, %d)' % (d,len(names)))

def findTableByColumn(d, env):
    names = env['cols_n']
    if d in names:
        return env['table']
    else:
        return None

def findTableByColumn2(d, env1, env2):
    r0 = findTableByColumn(d, env1)
    if r0:
        return r0
    else:
        r1 = findTableByColumn(d, env2)
        if r1: return r1
        else: unexpected('column %s not found in table %s and %s' % (d, env1['table'],env2['table']))

def whichTableByName(d, env2):
    if findTableByColumn(d, env2[0]):
        return 0
    elif findTableByColumn(d, env2[1]):
        return 1
    else:
        unexpected('name (%s) not found in both tables' % d)

def addOrderToEnv(d, env):
    if 'order' not in env:
        env['order'] = d
    else:
        unexpected('existed order %s (vs new order %s)' % (env['order'], d))
    return env

# update and make a copy
def copyEnvWithMask(mask, env):
    new_env = copy.deepcopy(env)
    new_env['mask'] = mask
    # order ?
    return new_env

"""
check if two strings/list strings are the same
"""
def sameListString(a, b):
    if isinstance(a, basestring) and isinstance(b, basestring):
        return a == b
    elif isinstance(a, list) and isinstance(b, list) and len(a)==len(b):
        for x in range(len(a)):
            if not sameListString(a[x], b[x]): return False
        return True
    else: return False

def packColumnName(x):
        return x if isinstance(x, list) else [x]

def printJSON(x):
    print json.dumps(x, sort_keys=False, indent=2)

"""
Handle exceptions
"""

def errorMsg(msg, description):
    raise ValueError('[%s] %s\n' % (description,msg))

def pending(msg):
    errorMsg(msg, 'Pending')

def unexpected(msg):
    errorMsg(msg, 'Unexpected')

def warning(msg):
    errorMsg(msg, 'Warning')

def wrong(msg):
    errorMsg(msg, 'Wrong')

def todo(msg):
    debug('[TODO]: %s' % msg)

def stop(msg=''):
    if msg:
        print msg
    raw_input('stop ...')
