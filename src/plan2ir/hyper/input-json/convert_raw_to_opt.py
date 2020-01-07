import json
import sys
from collections import OrderedDict

def readLines(filename,typ='.json'):
    return "".join([trim(x) for x in open(filename+typ,'r').readlines()])

def trim(text):
    return text.strip(' \r\n')

# 0 -> Unoptimized Plan
# 1 -> Unnesting
# 2 -> Predicate Pushdown
# 3 -> Operator Reordering
# 4 -> Physical Operator Mapping
# 5 -> Optimized Plan

def main():
    if len(sys.argv) != 2:
        print "Usage: python %s <file>" % sys.argv[0]
        sys.exit(1)
    name = sys.argv[1]
    plan = json.loads(readLines(name, ''), object_pairs_hook=OrderedDict)['optimizersteps']
    numb = len(plan)
    opt  = numb - 1 # output optimized one, i.e. 5
    #opt = 0  # unopt
    # for k in range(numb):
    #     print '%d -> %s' % (k,plan[k]['name'])
    #decide = raw_input("Extract one plan? > ")
    # print '-'*10
    # print 'opt = ' + str(opt)
    # print '-'*10
    print json.dumps(plan[opt]['plan'], indent=2)

if __name__ == '__main__':
    main()

