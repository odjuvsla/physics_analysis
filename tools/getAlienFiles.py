#!/usr/bin/env python
import sys
import os
import threading

class DownloadFileThread(threading.Thread):

    def __init__(self, threadNumber):
        threading.Thread.__init__(self)
        self.threadNumber = threadNumber

    def run(self):
        
        alienshCommand =  "aliensh file:alscript" + repr(self.threadNumber) + " > status" + repr(self.threadNumber) + " &"
    #    os.system(alienshCommand)
        print alienshCommand

class Downloader():
    
    def __init__(self, filepath, destdir, nthreads=1):

        self.filepath = str(filepath)
        self.nthreads = int(nthreads)
        self.destdir = destdir

        self.pathParts = []
        self.scanFilepath()
        self.makeScript()

    def start(self):

#        for i in range(self.nthreads):
#            t = DownloadFileThread(i)
#            t.start()

        for i in range(self.nthreads):
            alienshCommand =  "aliensh file:alscript" + repr(i)# + " > status" + repr(i) + " &"
            os.system(alienshCommand)
            print alienshCommand
            
    def scanFilepath(self):

        cnt = 0
        for part in self.filepath.split("*"):
#            self.pathParts[cnt] = part
            self.pathParts.append(part)

        print len(self.pathParts)


    def makeScript(self):
        
        command = "aliensh -c \"find " + self.filepath + " AliESDs.root\" > lstemp "
        os.system(command)
        print command
        F = open("lstemp", "r")
        lines = F.readlines()
        
        runNb = 0
        gdc = 0
        cnt = 0
        os.system("rm alscript*")

        for l in lines:

            dirname = (l.split("/"))[-2:-1]
            if dirname:
                cnt = cnt + 1

        filesPerThread = cnt/self.nthreads + 1

        print "# of files: " + repr(cnt) + ", # of threads: " + repr(self.nthreads) + " -> " + repr(filesPerThread) + " files per thread"

        cnt = 0
        threadNumber = 0
        for l in lines:

            dirname = (l.split("/"))[-2:-1]
            if dirname:
#                print dirname
                #print (dirname[2:11])
                runNb = int((dirname[0])[2:11])
                gdc = int((dirname[0].split(".")[0])[-3:])
                part = int((dirname[0].split(".")[1]))

#                print "Run #: " + repr(runNb)
#                print "GDC #: " + repr(gdc)
#                print "Part #: " + repr(part)
                
                finalDestination = self.destdir + "/" + repr(runNb) + "/" + repr(gdc) + "/" + repr(part)

                mkdir = "mkdir -p " + finalDestination

                downcommand = "cp " + l.rstrip(" \n") + " file:" + finalDestination + "/."

                os.system(mkdir)
                
                threadNumber = cnt/filesPerThread

#                print "File #: " + repr(cnt) + ", thread #: " + repr(threadNumber)

                f = open("alscript" + repr(threadNumber), "a")
                f.write(downcommand + "\n")
                f.close()
                
                cnt = cnt + 1
                
        nthreads = threadNumber + 1
        if self.nthreads != nthreads:

            print "Changed the number of threads from " + repr(self.nthreads) + " to " + repr(nthreads)
            self.nthreads = nthreads


from optparse import OptionParser


parser = OptionParser()

parser.add_option("-f", "--file", dest="filepath",
                  help="The path to the file(s) to get from alien. Accepts \"*\"s as wildcards")
parser.add_option("-d", "--destination", dest="destination",
                  help="The destination for the download")
parser.add_option("-n", "--nthreads", dest="nthreads",
                  help="Number of download threads to run")

(options, args) = parser.parse_args()

nthreads = 1

if not options.filepath:
    print "You have to specify a filepath to download! Idiot!"
    exit(-1)
if not options.destination:
    print "You have to specify a destination to download to! Idiot!"
    exit(-1)
if options.nthreads:
    nthreads = options.nthreads

downloader = Downloader(options.filepath, options.destination, nthreads)

downloader.start()




