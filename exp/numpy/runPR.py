import numpy as np
import time  as tm

def PageRank(webGraph):
    v = np.sum(webGraph, 0) # inbound
    h = np.sum(webGraph, 1) # outbound
    u = np.arange(len(v))
    s = np.arange(len(v))
    s[np.argsort(-1*v, kind='mergesort')] = u;
    return [u, h, v, s]

def loadFile(fileName):
    fullPath = "../../src/HorseIR/data/pl/" + fileName
    fp = open(fullPath, 'r')
    lineNo   = 0
    webGraph = []
    for line in fp:
        if lineNo > 0:
            lineItem = line.split(' ')
            nums     = []
            for dest in lineItem:
                nums.append(int(dest))
            webGraph.append(nums)
        lineNo = lineNo + 1
    print 'Total number of lines: %d' % lineNo
    return np.array(webGraph)

def logMsg(msg):
    f = open('temp-pagerank.log', 'a')
    f.write(msg + '\n')
    f.close()

def runPR():
    fileName = "pr_in1K.tbl"
    start = tm.time()
    webGraph = loadFile(fileName)
    end   = tm.time()
    logMsg('Loading data (%s) with (ms): %lf' % (fileName, ((end-start) * 1000)))
    start = tm.time()
    result= PageRank(webGraph)
    end   = tm.time()
    logMsg("The elapsed time is (ms): %lf" % ((end-start) * 1000))
    for x in range(10):
        print '%d %d %d %d' % (result[0][x],result[1][x],result[2][x],result[3][x])
    return result

if __name__ == "__main__":
    runPR()

