"""
Helper functions
"""

import sys, traceback, copy, json

def getEnvTable(env): return env['tables']
def getEnvName (env): return env['cols_n']
def getEnvAlias(env): return env['cols_a']
def getEnvType (env): return env['cols_t']
def getEnvMask (env): return env['mask']
def getEnvMaskA(env): return env['mask_a']

def printEnv(env):
    print 'Environment node: {mask: "%s"}' % (getEnvMask(env))
    table = env['tables']
    alias = env['cols_a']
    names = env['cols_n']
    types = env['cols_t']
    maska = env['mask_a']
    if getEnvMask(env):
        for x in range(len(alias)):
            print ' %10s.%-16s : %-5s -> %3s (%3s)' % (table[x],names[x],types[x],alias[x],maska[x])
    else:
        for x in range(len(alias)):
            print ' %10s.%-16s : %-5s -> %3s' % (table[x],names[x],types[x],alias[x])

"""
Tiny functions
"""

def emptyList(x):
    return True if len(x) == 0 else False

def strSymbolVec(x):
    if len(x) == 1:
        return '`' + x[0] + ':sym'
    elif len(x) > 1:
        temp = '`' + x[0]
        for t in x[1:]:
            temp = temp + ', `' + t
        return '(' + t + '):sym'
    else:
        unexpected('Vector is empty: %d' % len(x))

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
    errorMsg(msg, 'TODO')

def stop(msg=''):
    if msg:
        print msg
    raw_input('stop ...')


