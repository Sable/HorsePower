
import json

def scanMain(obj, dep):
    if isinstance(obj, list):
        for seg in obj:
            scanMain(seg, dep+1)
    if 'operator' in obj:
        print ' '*dep ,
        print obj['operator']
        if 'input' in obj:
            scanMain(obj['input'], dep+2)

counter = 0

def genId():
    global counter
    counter = counter + 1
    return ('id_%d' % counter)

def scanMainDot(obj, parent, pid):
    dep = 4
    if isinstance(obj, list):
        for seg in obj:
            scanMainDot(seg, parent, pid)
    if 'operator' in obj:
        op = obj['operator']
        id = genId()
        if parent != None:
            print ' '*dep ,
            print '%s -> %s' % (pid, id)
        print ' '*dep ,
        print '%s [ label = "%s" ]' % (id, op)
        if 'input' in obj:
            scanMainDot(obj['input'], op, id)

def compile(jsonObj, kind='raw'):
    if kind == 'raw':
        scanMain(jsonObj, 0);
    elif kind == 'dot':
        counter = 0
        print 'digraph query{'
        scanMainDot(jsonObj, None, None);
        print '}'

