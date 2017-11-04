import numpy as np
import time  as tm

def PageRank(webGraph):
    v = np.sum(webGraph, 0) # refcount
    h = np.sum(webGraph, 1) # urlcount
    s = np.argsort(v)
    return [s, v[s], h[s]]

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
    fileName = "pr_in10.tbl"
    start = tm.time()
    webGraph = loadFile(fileName)
    end   = tm.time()
    logMsg('Loading data (%s) with (ms): %lf' % (fileName, ((end-start) * 1000)))
    start = tm.time()
    result= PageRank(webGraph)
    end   = tm.time()
    logMsg("The elapsed time is (ms): %lf" % ((end-start) * 1000))
    return result

if __name__ == "__main__":
    runPR()

