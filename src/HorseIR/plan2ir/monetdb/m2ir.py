import json, sys, time

sys.path.append("../hyper")
from codelib import *

import text2json, json2ir

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
    #print json.dumps(text2json.scanPlan(plan))
    json2ir.compile(text2json.scanPlan(plan))


if __name__ == '__main__':
    start = time.time()
    main()
    info('Elapsed time: %.2lf ms' % (1000*(time.time() - start)))
