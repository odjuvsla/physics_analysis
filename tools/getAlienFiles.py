import sys
import os

aliensh = "/home/odjuvsla/Workspace/alice/alien/api/bin/aliensh" # path to aliensh
gridPath = " /alice/sim/PDC_09/LHC09a4/"                  # general path to data (in alien) (without run number)
stdfileName = "AliESDs.root"                                          # Filename to download

runNb = sys.argv[1]                                             
numberOfFolders = int(sys.argv[2])

path = gridPath + runNb

tmpFilename = "tmp_" + runNb +".txt"
scriptFilename ="script_" + runNb + ".txt"

if os.system("ls " + runNb) != 0:
    command = "mkdir " + runNb
    os.system(command)

command = aliensh + " -c 'ls " + path + "' > " + tmpFilename


os.system(command)

tmpfile = open(tmpFilename, "r")

folderNb = -1

scriptFile = open(scriptFilename, "w")

dirname = ""

n = 0
while 1:
    folderNb = tmpfile.readline()
    if folderNb == "" :
        break
    dirname =  runNb + "/" + folderNb.strip()
    if os.system("ls " + dirname) != 0:
        command = "mkdir " + dirname
        os.system(command)
    command = "cp " + path + "/" + folderNb.strip() + "/" + stdfileName+ " file:" + dirname + "/" + stdfileName+ " \n"
    scriptFile.write(command)
    n = n+1
    if n > numberOfFolders:
        break
tmpfile.close()
scriptFile.close()    

command = aliensh + " file:" + scriptFilename
os.system(command)
#print command
