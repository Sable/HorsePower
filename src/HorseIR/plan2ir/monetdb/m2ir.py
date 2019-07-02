import json, sys, time, copy

sys.path.append("../hyper")

from codelib import *
from parsing import *
from datetime import date, timedelta

ptr = 0

"""
Entry/Main
"""
def main():
    numArgs = len(sys.argv)
    if numArgs == 1:
        plan = readFromStdin()
    elif numArgs == 2:
        plan = open(sys.argv[1],'r').readlines()
    else:
        print "Usage:"
        print "\t(1) cat <file> | python %s" % sys.argv[0]
        print "\t(2) python %s <file>" % sys.argv[0]
        sys.exit(1)
    scanPlan(plan)

def scanPlan(plan):
    tokens = []
    for line in plan:
        line = trim(line).lstrip(' |')
        if line.startswith("%"):
            continue
        else:
            tokens = tokens + scanTokens(line)
    setGlobalTokens(tokens)
    #print tokens
    #printJSON(scanMain())
    print scanMain()

def scanMain():
    if   isKeyword('project') : return scanProject ()
    elif isKeyword('select')  : return scanSelect  ()
    elif isKeyword('table')   : return scanTable   ()
    elif isKeyword('join')    : return scanJoin    ()
    elif isKeyword('semijoin'): return scanSemiJoin()
    elif isKeyword('antijoin'): return scanAntiJoin()
    elif isKeyword('top')     : return scanTop     ()
    elif isGroupby()          : return scanGroupby ()
    elif isLeftOuterJoin()    : return scanLeftOuterJoin()
    else: wrong("not supported yet: %s" % curToken())

def scanTokens(text):
    tokens = []
    while True:
        word, k = nextToken(text)
        if k < 1: break
        else: tokens.append(word)
        text = trim(text[k:])
    #print word, k
    return tokens

def printCur():
    global gPtr, gTokens
    print gTokens[gPtr:]


def setGlobalTokens(tokens):
    global gPtr, gTokens, gSize
    gPtr = 0
    gTokens = tokens
    gSize = len(tokens)

def curToken(offset=0):
    global gPtr, gTokens, gSize
    if gPtr + offset < gSize:
        return gTokens[gPtr + offset]
    else:
        wrong("Size overflow: %d vs %d" % (gPtr,gSize))

def consume(s): 
    global gPtr
    if curToken() == s:
        gPtr = gPtr + 1
    else:
        printCur()
        wrong("Fail to consume %s, but %s found" % (s,curToken()))

def tryToken(s):
    if curToken() == s:
        consume(s)
        return True
    else:
        return False

def fetchToken():
    global gPtr
    token = curToken()
    gPtr = gPtr + 1
    if isID(token):
        return token[1:len(token)-1]
    return token

def isID(name):
    return True if name[0] == '"' else False

def isKeyword(name):
    return tryToken(name)

def isGroupby():
    return tryToken('group') and tryToken('by')

def isLeftOuterJoin():
    return tryToken('left') and tryToken('outer') and tryToken('join')

def isLastToken():
    global gPtr, gSize
    return True if gPtr >= gSize else False

"""
scan functions
"""

def scanProject():
    consume('(')
    content = scanMain()
    consume(')')
    exprs = scanProjectExprs()
    if (not isLastToken()) and curToken() == '[':
        exprs.append(scanProjectExprs())
    return {
        'operator'  : 'project',
        'input'     : content,
        'output'    : exprs
    }

def scanSelect():
    consume('(')
    content = scanMain()
    print curToken(-2)
    consume(')')
    conditions = scanSelectExprs()
    return {
        'operator'  : 'select',
        'input'     : content,
        'conditions': conditions
    }

def scanTable():
    consume('(')
    name = fetchIdName() #0
    consume(')')
    columns = scanTableColumns()
    consume("COUNT")
    return {
        'operator': 'table',
        'table'   : name,
        'columns' : columns
    }

def scanGroupby():
    consume('(')
    content = scanMain()
    consume(')')
    cond1 = scanGroupbyCond()
    cond2 = scanGroupbyCond()
    return {
        'operator': 'groupby',
        'input'   : content,
        'block1'  : cond1,
        'block2'  : cond2
    }

def scanJoinCore(joinKind):
    content = []
    consume('(')
    while not tryToken(')'):
        content.append(scanMain())
        tryToken(',')
    condition = scanJoinCondition()
    print curToken(1)
    return {
        'operator' : joinKind,
        'input'    : content,
        'condition': condition
    }

def scanJoin():
    return scanJoinCore('join')

def scanSemiJoin():
    return scanJoinCore('semijoin')

def scanLeftOuterJoin():
    return scanJoinCore('leftouterjoin')

def scanAntiJoin():
    return scanJoinCore('antijoin')

def scanTop():
    consume("N")
    consume("(")
    content = scanMain()
    consume(")")
    limit = scanTopLimit()
    return {
        'operator': 'top',
        'input'   : content,
        'limit'   : limit
    }

"""
helper functions
"""

def fetchIdName():
    names = [ fetchToken() ]
    while tryToken('.'):
        names.append(fetchToken())
    return {'id': names}

def scanTableColumns():
    columns = []
    consume('[')
    while not tryToken(']'):
        item = scanItem()
        if item['type'] == 'joinidx':
            lastIndx = len(columns) - 1
            columns[lastIndx] = {
                'operator': 'joinidx',
                'left'    : columns[lastIndx],
                'right'   : scanItem()
            }
            #print columns[lastIndx]
            #stop()
        else:
            columns.append(item)
        tryToken(',')
    return columns

def scanSelectExprs():
    exprs = []
    consume('[')
    while not tryToken(']'):
        exprs.append(scanSelectStmt())
        tryToken(',')
    return exprs

def scanProjectExprs():
    return scanSelectExprs()

def scanGroupbyCond():
    return scanSelectExprs()

def scanTopLimit():
    return scanSelectExprs()

def scanSelectStmt():
    numOp = 0
    stmt = []
    while True:
        cur = curToken()
        if cur == ',' or cur == ']': break
        item = scanItem()
        if isinstance(item, dict) and item['type'] == 'arith':
            item['op'] = numOp
            numOp = numOp + 1
        stmt.append(item)
    numStmt = len(stmt)
    if numStmt != 3 and numStmt != 5:
        print len(stmt)
        #print stmt
        todo("add support")
    return stmt
    #if tryToken("date"):

def isBinaryOp(cur):
    return True if cur in [ '<', '<=', '=', '!=', '>', '>=', 'in', 'or' ] else False

def scanItem():
    item = scanItemCore()
    if item == 'p_partkey':
        stop()
    # order tag
    if tryToken("ASC"):
        item['order'] = 'asc'
    elif tryToken("DESC"):
        item['order'] = 'desc'
    # null tag
    if tryToken("NOT"):
        consume("NULL")
        item['isNull'] = 'notnull'
    elif tryToken("NULL"):
        item['isNull'] = 'null'
    # hash tag
    if tryToken("HASHCOL"):
        item['property'] = 'hashcol'
    elif tryToken("HASHIDX"):
        item['property'] = 'hashidx'
    return item

def consumeConstype():
    fetchToken()
    if tryToken('('):
        fetchToken()
        if tryToken(','):
            fetchToken()
        consume(')')

def scanItemCore():
    cur = curToken()
    if cur in [ "date", "tinyint", "smallint", "int", "bigint", "month_interval", "boolean" ]:
        consume(cur)
        return { "type": cur, "value": fetchToken() }
    elif tryToken("sec_interval"):
        consume('('); intv = fetchToken(); consume(')')
        return { "type": cur, "value": fetchToken(), 'interval': intv}
    elif cur in [ "decimal", "varchar" ]:
        consumeConstype()
        if tryToken('['):
            expr = scanItem()
            consume(']')
        else:
            expr = fetchToken()
        return { "type": cur, "value": expr }
    elif cur in [ "char", "double" ]:
        consumeConstype()
        if tryToken('['):
            expr = scanItem()
            consume(']')
        elif tryToken('('):
            charSize = fetchToken()
            consume(')')
            return { "type": cur, "const_size": charSize, "value": fetchToken() }
        else:
            expr = fetchToken()
        return { "type": cur, "value": expr }
    elif isBinaryOp(cur):
        return { "type": "binary_op", "value": fetchToken() }
    elif tryToken('clob'):
        return { "type": "clob" , "value": fetchClob() }
    elif tryToken('('):
        return scanSelectItemSub()
    elif tryToken('as'):
        return { "type": "as", "value": fetchIdName() }
    elif tryToken('FILTER'):
        return { "type": "filter", "value": fetchToken(), "neg": 0 }
    elif tryToken('!'):
        consume('FILTER')
        return { "type": "filter", "value": fetchToken(), "neg": 1 }
    elif tryToken('JOINIDX'):
        return { "type": "joinidx", "value": "" }
    elif curToken(1) == '.': # second token
        if isID(cur):
            return { "type": "name" , "value": fetchIdName() }
        else:
            id = fetchIdName()
            if tryToken('no'): # optional
                consume('nil')
            if tryToken('('):
                return { "type": "expr" , "value": fetchExpr(id) }
            else:
                return { "type": "expr" , "value": id }
    elif isID(cur):
        return { "type": "string", "value": fetchToken() }
    else:
        printCur()
        wrong('unknown')
    pass

def fetchExpr(func):
    items = []
    while not tryToken(')'):
        items.append(scanItem())
        tryToken(',')
    return { "function": func, "args": items }

def fetchClob():
    if tryToken('['):
        item = scanItem()
        consume(']')
    else:
        item = scanItem()
    return item

def scanSelectItemSub():
    items = []
    while not tryToken(')'):
        items.append(scanItem())
        tryToken(',')
    return items

def scanJoinCondition():
    return scanTableColumns()

if __name__ == '__main__':
    start = time.time()
    main()
    info('Elapsed time: %.2lf ms' % (1000*(time.time() - start)))
