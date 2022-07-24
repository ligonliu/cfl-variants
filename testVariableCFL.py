import os,random,subprocess,sys

def saveCompleteGraph(num_nodes=5, fileName=''):
    if fileName=='':
        fileName = os.tmpnam()

    fp = open(fileName, 'w')

    for i in range(0,num_nodes):
        line = str([j for j in range(0,num_nodes) if j!=i])
        line = line.replace('[','').replace(']','').replace(',','')
        fp.write(line + '\n')

    fp.close()
    return fileName


# saveCompleteGraph(fileName='5complete.adjList')

# Usage: Release/VariableCFL adjList.txt numChansReq.txt [numTotalChan]

def generateRandomIntegerLines(num_lines=5, fileName = '', l_inc=1, u_exc= 6):
    if fileName == '':
        fileName = os.tmpnam()

    fp = open(fileName, 'w')

    for i in range(0,num_lines):
        fp.write(str(random.randint(l_inc,u_exc-1)) + '\n')

    fp.close()
    return fileName

import numpy as np

def main():
    adjListFile = "5complete.adjList"

    numTotalChan = 25

    results = []

    FNULL = open(os.devnull, 'w')


    
    for i in range(0,10000):

        randomSeed = random.randint(0, sys.maxint/2)

        numColorsRequestedFile = "testColorReqs.txt"

        cmdline = "./Release/VariableCFL " + adjListFile + " " + numColorsRequestedFile + " numTotalChan={0}".format(numTotalChan) + " randomSeed={0}".format(randomSeed)

        #print(cmdline)

        sys.stderr.write("\b\b\b\b\b\b\b\b\b\b\b{0}/10000".format(i))

        subprocess.call(cmdline, shell=True, stderr=FNULL, stdout=FNULL)

        outputFileName = "{0}_numTotalChan{1}.out".format(adjListFile,numTotalChan)

        #print(outputFileName)

        fp = open(outputFileName)

        allLines = []

        while True:
            line = fp.readline()
            if len(line)==0:
                break
            elif len(line)==1:
                continue
            allLines.append(line)

        lastline = allLines[len(allLines)-1]

        if lastline[0:9]=="# Success":

            # find number of cycles
            lastlinesplit = lastline.split()

            num_cycles = int(lastlinesplit[len(lastlinesplit)-2])

            results.append((True,num_cycles))
        else:
            results.append((False,0))

    sys.stderr.write("\b\b\b\b\b\b\b\b\b\b\b")

    numSuccess = sum(x[0] for x in results)

    print("{0} successes of {1}".format(numSuccess,len(results)))

    success_cycles = np.array([x[1] for x in results if x[0]])

    print("Avg. cycles: {0}".format(np.mean(success_cycles)))

    print("Standard deviation of cycles: {0}".format(np.std(success_cycles)))


    outputFileName = "{0}_numTotalChan{1}.cycles".format(adjListFile,numTotalChan)

    fp = open(outputFileName,'w')

    for cycle in success_cycles:
        fp.write("{0}\n".format(cycle))

    fp.close()

main()

