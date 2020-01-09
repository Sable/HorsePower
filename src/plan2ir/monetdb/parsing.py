import sys

from util import *

def isLetter(x):
    return (x >= 'a' and x <='z') or (x>='A' and x<='Z')

def isDigit(x):
    return (x >= '0' and x<='9')

def isUnderScore(x):
    return (x == '_')

def isDelimiter(x):
    return (x in '().[],')

def nextName(line, size):
    k = 1; size = len(line)
    while k < size:
        if isLetter(line[k]) or isDigit(line[k]) or isUnderScore(line[k]):
            k = k + 1
        else:
            break
    return line[:k],k

def nextString(line, size):
    k = 1
    while k < size:
        if line[k] == '"':
            break
        else:
            k = k + 1
    if k < size and line[k] == '"':
        return line[:k+1],(k+1)
    else:
        print line
        wrong('the char " needs an ending "')

def nextOp(line, ch):
    next = line[1]
    if next == '=':
        return '%c%c'%(ch,next), 2
    else:
        return ch, 1

def nextNumber(line, size):
    k = 1
    while k < size:
        t = line[k]
        if isDigit(t) or t == '.':
            k = k + 1
        else:
            break
    return line[:k],k

def nextToken(line):
    size = len(line)
    if size > 0:
        ch = line[0]
        if isDelimiter(ch):
            return ch,1
        elif isLetter(ch):
            return nextName(line, size)
        elif ch == '"':
            return nextString(line, size)
        elif ch in '<>=!':
            return nextOp(line, ch)
        elif isDigit(ch):
            return nextNumber(line, size)
        elif ch == '&':
            return ch,1
        else:
            wrong("unknown ch: %c" % ch)
    else:
        return "", 0

