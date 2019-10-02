import sys

def getInfo(fileId):
    filename = 't%d.hir' % fileId
    f = open(filename, 'r')
    line = f.readline().strip(' \r\n')
    #for line in f:
    #    print line
    f.close()
    return line

def createInfo(line):
    info = line.split(':')[1].strip()
    return info

def msg(x, line):
    print '- Test #%d: %s' % (x,line)

def msg_pass(x, line):
    print '#%d: %s' % (x,line)

def main():
    if len(sys.argv) == 2:
        tot = int(sys.argv[1])
        for x in range(tot):
            line = createInfo(getInfo(x))
            msg(x, line)
    elif len(sys.argv) == 3:
        id = int(sys.argv[2])
        line = createInfo(getInfo(id))
        msg_pass(id, line)
    else:
        print 'Usage 1: python %s <num>' % sys.argv[0]
        print 'Usage 2: python %s get <num>' % sys.argv[0]
        exit(1)


if __name__ == '__main__':
    main()

