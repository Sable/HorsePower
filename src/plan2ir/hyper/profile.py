import json, sys
from codegen import *

"""
Usage:
    python profile.py opt

Improve:
    - check details rather than only constructs
        + e.g. the number of returned fields
"""

def main():
    if len(sys.argv) != 2:
        print "Usage: python profile.py opt"
        exit(1)
    data = []
    #data.append(processMain(19))
    for id in range(22):
        data.append(processMain(1+id))
    # print data
    profileData(data)
    profileOperator(data, 'join')
    profileOpUnique(data, 'join')

def profileOperator(data, op):
    opMax = 3
    for c,value in enumerate(data):
        sys.stdout.write(('q'+str(c+1)).ljust(opMax) + '| ')
        print value[op]

def profileOpUnique(data, op):
    unique = set()
    for c,value in enumerate(data):
        for item in value[op]:
            unique.add(item)
    print unique

def profileData(data):
    # fetch row information
    rows = {}
    for value in data:
        for x in value.keys():
            if not x in rows:
                rows[x] = []
            rows[x].append(len(value[x]))
    # print table
    opKeys = data[0].keys()
    opMax  = maxLen(opKeys)
    lineSize = opMax
    sys.stdout.write('operator'.ljust(opMax))
    for id in range(22):
        item = ('|q%d' % (id+1)).ljust(4)
        sys.stdout.write(item)
        lineSize = lineSize + len(item)
    print ''
    print '-'*lineSize
    for op in opKeys:
        sys.stdout.write('%s' % op.ljust(opMax))
        for c,n in enumerate(rows[op]):
            # itemSize = 3 if c < 9 else 4
            itemSize = 4
            sys.stdout.write(('| %d' % n).ljust(itemSize))
        print ''
    print '-'*lineSize
    # print rows
    tot = 0
    for x in rows:
        tot = tot + sum(rows[x])
    print 'total = ' + str(tot)
    # check with: (should be the same result)
    # grep -Rin "operator\"" opt | wc -l

def maxLen(vector):
    x = 0
    for v in vector:
        x = max(x, len(v))
    return x


def processMain(id):
    name = '%s/q%d.json' % (sys.argv[1],id)
    plan = json.loads(readLines(name, ''))
    data = profilePlan(plan)
    return data

def profilePlan(plan):
    return {
        'tablescan' : searchOperator(plan, ['tablescan']),
        'tempscan'  : searchOperator(plan, ['tempscan']),
        'groupby'   : searchOperator(plan, ['groupby']),
        'sort'      : searchOperator(plan, ['sort']),
        'map'       : searchOperator(plan, ['map']),
        'select'    : searchOperator(plan, ['select']),
        'earlyprobe': searchOperator(plan, ['earlyprobe']),
        'join'      : searchOperator(plan, ['join']),
        'temp'      : searchOperator(plan, ['temp'], isExact=True) #q15
    }

def profileStat(data):
    val = {}
    for x in data.keys():
        val[x] = len(data[x])
    return val

def prettyJSON(x):
    print json.dumps(x, indent=4)

def searchOperator(plan, ops, isExact=False):
    rtn = []
    if isinstance(plan, dict):
        if 'operator' in plan:
            # if op == 'join':
            #   print plan['operator']
            for op in ops:
                matched = (op == plan['operator']) if isExact else (op in plan['operator'])
                if matched:
                    rtn.append(plan['operator'].encode('ascii', 'ignore'))
                    break
        for key in plan.keys():
            value = plan[key]
            rtn = rtn + searchOperator(value, ops, isExact)
    elif isinstance(plan, list):
        for value in plan:
            rtn = rtn + searchOperator(value, ops, isExact)
    return rtn

if __name__ == "__main__":
    main()
