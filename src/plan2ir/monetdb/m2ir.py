import json, sys, time

sys.path.append("../hyper")
from codelib import *

import text2json, json2ir, jsonPrinter

"""
Entry/Main
"""
def main():
    numArgs = len(sys.argv)
    if numArgs == 1:
        genPlan(readFromStdin())
    elif numArgs == 2:
        genPlan(open(sys.argv[1],'r').readlines())
    elif numArgs == 3:
        op   = sys.argv[1]
        plan = open(sys.argv[2],'r').readlines()
        genPlanWithOp(op, plan)
    else:
        print "Usage:"
        print "\t(1) cat <file> | python %s" % sys.argv[0]
        print "\t(2) python %s <file>" % sys.argv[0]
        print "\t(3) python %s show/dot/dump <file>" % sys.argv[0]
        sys.exit(1)

def genPlan(plan):
    json2ir.compile(text2json.scanPlan(plan))

def genPlanWithOp(op, plan):
    if op == 'dump':   # dump json objects
        print json.dumps(text2json.scanPlan(plan))
    elif op == 'show': # show operator trees
        jsonPrinter.compile(text2json.scanPlan(plan))
    elif op == 'dot':  # show operator trees in dots
        jsonPrinter.compile(text2json.scanPlan(plan), 'dot')
    else:
        unexpected('Unknown op: %s' % op)

if __name__ == '__main__':
    start = time.time()
    main()
    info('Elapsed time: %.2lf ms' % (1000*(time.time() - start)))
