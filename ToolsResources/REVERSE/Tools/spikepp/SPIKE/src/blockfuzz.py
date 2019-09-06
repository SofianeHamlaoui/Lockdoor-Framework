#!/usr/bin/env python
"""
blockfuzz.py
fuzzes a server with one/one block substitution for FTP, SMTP and other protocols

You'll see. :>
"""
fuzzfilename="templinefuzz.spk"
skipvar=0
skipstr=0

def createfuzzfile(cleanfile,varfile,currentblock):
    done=0
    #each file currently points at the next block of lines to read in
    #open the files as requested
    cleanfilelines=open(cleanfile).readlines()
    varfilelines=open(varfile).readlines()

    #terminate the blocks in case the user forgot
    cleanfilelines.append("//endblock\n")
    varfilelines.append("//endblock\n")
    
    outlines=[]
    cleanblock={}
    a=0
    cleanblock[-1]=0
    i=0
    for line in cleanfilelines:
        print "Parsing Line:%s"%line
        if line.count("//endblock"):
            print "Found cleanfile endblock at %d:%d"%(i,a)
            cleanblock[i]=a
            i+=1
        a+=1

    cleanblock[i]=a
    varblock={}
    varblock[-1]=0
    a=0
    i=0
    for line in varfilelines:
        print "Parsing Line:%s"%line
        if line.count("//endblock"):
            print "Found endblock in varfile at %d:%d"%(i,a)
            varblock[i]=a
            i+=1
        a+=1
    varblock[i]=a
    
    numblocks=len(cleanblock.keys())
    print "Number of blocks is %d %s"%(numblocks,str(cleanblock))
    #ok, now we know where each block starts
    #-1 for start and -1 for end
    if currentblock==numblocks-2:
        done=1

    for i in range(0,cleanblock[currentblock]):
        outlines.append(cleanfilelines[i])

    if varblock.has_key(currentblock):
        endblock=varblock[currentblock]
    else:
        endblock=len(varfilelines)

    print "The end of the block %d is %d:%d"%(currentblock,varblock[currentblock-1],endblock)
    #now add the variables
    for i in range(varblock[currentblock-1],endblock):
        print "i=%d"%i
        outlines.append(varfilelines[i])

    #now add the rest of the clean file
    if cleanblock.has_key(currentblock+1):
        endblock=varblock[currentblock+1]
    else:
        endblock=len(cleanfilelines)

    for i in range(cleanblock[currentblock],endblock):
        outlines.append(cleanfilelines[i])

    outfile=open(fuzzfilename,"w")
    for line in outlines:
        outfile.write(line)
    outfile.close()
    return done

def runfuzz(host,port):
    import os
    os.system("./generic_send_tcp %s %d %s %d %d"%(host,port,fuzzfilename,skipvar,skipstr))
    
    
def usage():
    import sys
    print "Usage: %s -c cleanfile -v varfile [-s startline:0] -h host -p port"%sys.argv[0]
    print "You'll probably want to use Control-Z to stop this."
    sys.exit(1)
    

if __name__=="__main__":
        
    import getopt,sys

    try:
        (opts,args)=getopt.getopt(sys.argv[1:],"c:v:s:h:p:")
    except getopt.GetoptError:
        #print help
        usage()
        
    cleanfile=""
    varfile=""
    startline=0
    for o,a in opts:
        if o in ["-c"]:
            cleanfile=a
        if o in ["-v"]:
            varfile=a
        if o in ["-s"]:
            startline=int(a)
        if o in ["-h"]:
            host=a
        if o in ["-p"]:
            port=int(a)
            

    if cleanfile=="" or varfile=="" or host=="" or port==0:
        usage()


    currentblock=startline
    #actually fuzz each line
    done=0
    while not done:
        done=createfuzzfile(cleanfile,varfile,currentblock)
        runfuzz(host,port)
        currentblock+=1

    print "Done fuzzing all blocks!"

    
