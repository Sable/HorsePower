import sys

def createNext(id):
    fileName = 't%d.hir' % id
    print '>> generating %s' % fileName
    fp = open(fileName, 'w')
    fp.write("// %d : <add notes> " % id)
    fp.close()
    print '>> %s generated successfully!' % fileName

def main():
    if len(sys.argv) != 1:
        print 'Usage: python %s' % sys.argv[0]
        exit(1)
    maxValue = -1
    for line in sys.stdin:
        items = line.strip(' \r\n').split(' ')
        for item in items:
            if item.startswith('t'):
                val = int(item[1:len(item)-4])
                if val > maxValue:
                    maxValue = val
    nextItem = maxValue + 1
    createNext(nextItem)

if __name__ == '__main__':
    main()

