import numpy as np
import time  as tm

def BlkSchls(timet,sptprice,strike,rate,volatility,time,otypeC):
    xSqrtTime     = np.sqrt(time)
    xLogTerm      = logValues = np.log(sptprice * 1.0 / strike)
    xRiskFreeRate = rate
    otype         = otypeC=='P'
    xDen          = volatility * xSqrtTime
    xPowerTerm    = volatility * (volatility * 0.5)
    d1 = xD1      = (xLogTerm + time * (xRiskFreeRate + xPowerTerm)) / xDen
    d2 = xD2      = xD1 - xDen
    invs2xPI      = 0.39894228040143270286
    NofXd1        = CNDF( d1, invs2xPI )
    NofXd2        = CNDF( d2, invs2xPI )
    FutureValueX  = strike * np.exp(- rate * time)
    OptionPrice   = otype * ((FutureValueX * (1.0 - NofXd2)) - (sptprice * (1.0 - NofXd1)))
    OptionPrice   = OptionPrice + (~otype) * ((sptprice * NofXd1) - (FutureValueX * NofXd2))
    return OptionPrice

def CNDF(InputX, invs2xPI):
    sign          = InputX < 0
    xInput        = np.abs(InputX)
    expValues     = np.exp(-0.5 * (InputX * InputX))
    xNPrimeofX    = invs2xPI * expValues
    xK2           = 1.0 / (1.0 + 0.2316419 * xInput)
    xK2_2         = xK2 * xK2
    xK2_3         = xK2 * xK2_2 
    xK2_4         = xK2 * xK2_3 
    xK2_5         = xK2 * xK2_4
    xLocal_1      = xK2 * 0.319381530 
    xLocal_2      = (xK2_2 * -0.356563782) + (xK2_3 * 1.781477937) + (xK2_4 * -1.821255978) + xK2_5 * 1.330274429
    xLocal        = 1.0 - xNPrimeofX * (xLocal_2 + xLocal_1)
    OutputX       = (sign * (1.0 - xLocal)) + (~sign) * xLocal
    return OutputX

def runBS(check):
    input_file = 'in_1M.txt'
    # load data files
    start = tm.time()
    sptprice,strike,rate,volatility,time,otypeC=loadFile(input_file)
    end   = tm.time()
    print "%s is loaded within (ms): %lf" % (input_file, (end-start)*1000)
    start = tm.time()
    price = BlkSchls(0,sptprice,strike,rate,volatility,time,otypeC)
    end   = tm.time()
    print "The elapsed time is (ms): %lf" % ((end-start) * 1000)
    if check:
        print price

def loadFile(fileName):
    # fullPath = "/Users/wukefe/Documents/GitHub/array17-ecc/src/blackscholes/c/data/" + fileName
    fullPath = "../../src/HorseIR/data/pl/" + fileName
    fp = open(fullPath, 'r')
    lineNo    = 0
    sptprice   = []
    strike     = []
    rate       = []
    volatility = []
    time       = []
    otypeC     = []
    for line in fp:
        if lineNo == 0:
            totalLine = int(line)
        else:
            lineItem = line.split(' ')
            sptprice.append  (float(lineItem[0]))
            strike.append    (float(lineItem[1]))
            rate.append      (float(lineItem[2]))
            volatility.append(float(lineItem[4]))
            time.append      (float(lineItem[5]))
            otypeC.append    (lineItem[6])
        lineNo = lineNo + 1
    return np.array(sptprice),np.array(strike),np.array(rate),np.array(volatility), np.array(time), np.array(otypeC)

if __name__ == "__main__":
    runBS(1)

