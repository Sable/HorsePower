import json, sys, time
import text2json, json2ir, jsonPrinter

# sys.path.append("../hyper")
from util import *

"""
Entry/Main
"""
def main():
    numArgs = len(sys.argv)
    if numArgs == 1:
        if not sys.stdin.isatty():
            genPlan(readFromStdin())
        else:
            usage()
    elif numArgs == 2:
        plan = open(sys.argv[1],'r').readlines()
        genPlan(plan)
    elif numArgs == 3:
        op   = sys.argv[1]
        plan = open(sys.argv[2],'r').readlines()
        genPlanWithOp(op, plan)
    else:
        usage()

def usage():
    programName = sys.argv[0]
    print "Usage:"
    print "\t(1) cat <file> | python %s" % programName
    print "\t(2) python %s <file>" % programName
    print "\t(3) python %s show/dot/dump <file>" % programName
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
    info('Elapsed time: %.2lf ms' % (1000.0*(time.time() - start)))

