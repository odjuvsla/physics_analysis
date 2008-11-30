import sys
import os
import time
import threading

#########################################
# Download files thread class
#########################################
class Download ( threading.Thread ):
    
    def run ( self ):
        global threadN
        alienshCommand =  "/home/odjuvsla/Workspace/alice/alien/api/bin/aliensh file:alscript" + repr(threadN)
        #print alienshCommand
        os.system(alienshCommand)
        
#########################################
# Functions
#########################################

# Check the number of parts in run
def CheckNumberOfParts():
    #os.system(aliensh + " -c ls " + gridPath + " > altemp")
    fi = open("altemp", "r")
    k = 0
    while fi.readline() != "":
        print k
        k = k+1
    fi.close
    print "Number of parts to download: " + repr(k)
    return k    

# Run convertion on all parts (n) when they are downloaded
def RunConverter(s):
    print "Hi! I'm " + repr(pid) + ", and I will convert files!"
    while s > 0:
        stemp = s
        for pnb in allPartNb:
            dir = "run" + runNb+"." + pnb
            listreturn = os.listdir(dir)
            for files in listreturn:
                filePath = dir+"/"+files
                fileSize = os.path.getsize(filePath)
                if fileSize > minFileSize:
                    print "part number: " + repr(pnb)
                    if fileSize == fileSizes[int(pnb)/10]:
                        r = os.system("deroot " + filePath + " tmp.date" + " " + dir + "/")
                        if r == 0:
                            s = s-1
                            os.system("rm " + filePath)
                            os.system("rm tmp.date")
                            print "deroot of file " + files + " done!"
                        else:
                            print "deroot of file: " + files + " failed, I will try again later" 
                    else:
                        fileSizes[int(pnb)/10] = fileSize
        if s == stemp:
            print "Oh well, no files ready yet. I will take a look again in a minute"
        time.sleep(60)
    return

#########################################
# Start of the program
#########################################

# Some definitions
aliensh = "/home/odjuvsla/Workspace/alice/alien/api/bin/aliensh" # path to aliensh
gridPath = "/alice/data/2007/LHC07a_PHOS/00000"                  # general path to data (in alien) (without run number)
rawPath = "/raw/"                                                # the raw sub directory (in alien)
stdfileName = "0700000"                                          # prefix to all filenames (in alien)
fileSuffix = "001."                                              # mid suffix to all files (in alien)
rootSuffix = ".root"                                             # file suffix (in alien)
minFileSize = 10000000                                           # minimum size of raw data

####################################################################
# Get the arguments, we need either 5 arguments (or -h)
####################################################################
# The 5 arguments should be this:
# 1: The run number
# 2: The first part number 
# 3: The last part number 
# 4: Number of threads used to download files
# 5: If the files should be converted with deroot to raw files
#    (true if argument = -c, any other value => false)
####################################################################
# E.g. this is file with run # 8249, part # 180: 07000008249001.1800
####################################################################
if sys.argv[1] == "-h":
    print "\n####################################################################\n# We need either 5 arguments (or -h)\n####################################################################\n# The 5 arguments should be this:\n# 1: The run number\n# 2: The first part number \n# 3: The last part number \n# 4: Number of threads used to download files\n# 5: If the files should be converted with deroot to raw files\n#    (true if argument = -c, any other value => false)\n####################################################################\n# E.g. this is file with run # 8249, part # 180: 07000008249001.1800\n####################################################################\n"
    sys.exit()

runNb = sys.argv[1]                                             
partNbStart = sys.argv[2]
partNbStop = sys.argv[3]
numberOfThreads = sys.argv[4]
if sys.argv[5] == "-c":
    doConvert = 1
else:
    doConvert = 0

gridPath = gridPath + runNb + rawPath
allFilenames = []
allPartNb = []


if partNbStop == "-1":
    rangeEnd = CheckNumberOfParts()
    rangeEnd = rangeEnd + 1

else:
    rangeEnd = int(partNbStop) + 1 

if numberOfThreads > int(rangeEnd) - int(partNbStart) + 1:
    print "Number of threads larger than number of files, cutting down on the threads"
    numberOfThreads = int(rangeEnd) - int(partNbStart)


for i in range(int(partNbStart), rangeEnd):
    
    partNb = repr(i*10)
    allPartNb.append(partNb)

    dirname = "run" + runNb + "." + partNb
    command = "mkdir " + dirname
    os.system(command)

    scriptNumber = i % numberOfThreads

    fileName = stdfileName + runNb + fileSuffix + partNb + rootSuffix
    
    allFilenames.append(fileName)

    fullPath = gridPath + fileName

    command = "cp " + fullPath + " file:" + dirname + "/"
    f = open("alscript" + repr(scriptNumber), "w")    
    f.write(command + "\n")
    f.close()
    print 



time.sleep(1)

oldpid = os.getpid()

pid = os.fork()

n = rangeEnd - int(partNbStart)

fileSizes=[None]*(n+1)

if pid != 0:
    if doConvert:
        RunConverter(n)
    sys.exit()
else:
    
    print "Hi! I'm the good old " + repr(oldpid) + ", and I will download the files:"
    for b in allFilenames:
        print b
    for threadN in range(0, int(numberOfThreads)):
        #Download().start()
       
        #os.system(alienshCommand)

# End of program








    
#/home/odjuvsla/Workspace/alice/alien/api/bin/aliensh -c cp /alice/data/2007/LHC07a_PHOS/000008249/raw/07000008249001.1800.root file:8249/
