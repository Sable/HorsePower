import sys

"""
Usage:
    grep -REin "program slicing|time" gen | python profile_horseir.py
"""

def main():
    info_line = {}; info_time = {}
    for line in sys.stdin:
        parts = line.strip(' \r\n').split('//')
        header,content = parts  # two parts
        qid = getQueryId(header)
        if 'Program slicing' in content:
            temp = content.split('(')[-1].split(' ')
            info_line[qid] = [dropLast(temp[1]), dropLast(temp[3])]
        elif 'Elapsed time' in content:
            temp = content.split(' ')[-2]
            info_time[qid] = float(temp)
        else:
            sys.stderr.write('Error')
            exit(1)
        # print content
    printInfo(info_line, info_time)
    pass

def printInfo(info_line, info_time):
    print 'Note: change = (before - after) / before * 100%'
    string_head = 'qid | time (ms) | before | after | change'
    print '-'*len(string_head)
    print string_head
    print '-'*len(string_head)
    for x in range(22):
        qid = 'q%d' % (x+1)
        num = info_line[qid]
        ttt = info_time[qid]
        print '%-3s | %9.2lf | %6d | %5d | %5.lf%%' % (qid, ttt, num[0], num[1], compute(num[0],num[1]))
    print '-'*len(string_head)
    print 'Average time (ms): %.2lf' % getAverageTime(info_time)
    print 'Line number (total, average): %d, %d' % getLineInfo(info_line)

def getQueryId(x):
    return x.split('/')[1].split('.')[0]

def dropLast(x):
    return int(x[:-1])

def compute(x, y):
    return (x-y)*100.0/x

def getAverageTime(info_time):
    tot = 0
    for x in info_time:
        tot = tot + info_time[x]
    return tot*1.0/len(info_time)

def getLineInfo(info_line):
    tot = 0
    for x in info_line:
        tot = tot + info_line[x][1]
    return tot, tot/len(info_line)

if __name__ == '__main__':
    main()

