import sys
import os

aliensh = "/home/odjuvsla/Workspace/alice/alien/api/bin/aliensh" # path to aliensh
gridPath = "/alice/sim/LHC10a18/"                  # general path to data (in alien) (without run number)
stdfileName = "galice.root"                                          # Filename to download

runNb = sys.argv[1]                                             
numberOfEvents = int(sys.argv[2])

path = gridPath + runNb

tmpFilename = "tmp_" + runNb +"_galice.txt"
scriptFilename ="script_" + runNb + "_galice.txt"

if os.system("ls " + runNb) != 0:
    command = "mkdir " + runNb
    os.system(command)

command = aliensh + " -c 'ls " + path + "' > " + tmpFilename


os.system(command)

tmpfile = open(tmpFilename, "r")

evNb = -1

scriptFile = open(scriptFilename, "w")

dirname = ""

n = 0
while 1:
    evNb = tmpfile.readline()
    if evNb == "" :
        break
    dirname =  runNb + "/" + evNb.strip()
    if os.system("ls " + dirname) != 0:
        command = "mkdir " + dirname
        os.system(command)
    command = "cp " + path + "/" + evNb.strip() + "/" + stdfileName+ " file:" + dirname + "/" + stdfileName+ " \n"
    scriptFile.write(command)
    n = n+1
    if n > numberOfEvents:
        break
tmpfile.close()
scriptFile.close()    

command = aliensh + " file:" + scriptFilename
os.system(command)

