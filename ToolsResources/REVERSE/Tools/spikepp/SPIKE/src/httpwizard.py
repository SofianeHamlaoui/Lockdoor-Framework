#!/usr/bin/python

import sys, os

changelog=""" 

1.1 Edited to handle multiple requests in one stream (by checking if the first
word is all caps)

1.0 Released
"""

def variableizeString(instr):
    """
    Takes in a string, converts it to list of variables and strings
    """
    #we want to split 6733b66bc15c38e231030c87f58d602e01d0a8e3, sd=6bc330f6
    #into s_string_variable("6733b66bc15c38e231030c87f58d602e01d0a8e3"); s_string(", ");
    #s_string_variable("sd"); s_string_variable("6bc330f6")
    out=[]
    if instr.count(", "):
        commas=instr.split(", ")
        first=1
        for c in commas:
            if not first:
                out += ["s_string(\", \");"]
            first=0
            out += variableizeString(c)
        return out
    if instr.count(","):
        commas=instr.split(",")
        first=1
        for c in commas:
            if not first:
                out += ["s_string(\",\");"]
            first=0
            out += variableizeString(c)
        return out
    if instr.count("="):
        vars=instr.split("=")
        first=1
        for c in vars:
            if not first:
                out += ["s_string(\"=\");"]
            first=0
            out += variableizeString(c)
        return out
    if instr.count(";"):
        vars=instr.split(";")
        first=1
        for c in vars:
            if not first:
                out += ["s_string(\";\");"]
            first=0
            out += variableizeString(c)
        return out
    #default action
    return ["s_string_variable(\"%s\");"%instr]
            
    
def variableizeHeader(instr):
    """
    converts a header string into a lot of variables, if necessary
    """
    output=[]
    front=instr.split(": ")[0]
    back=": ".join(instr.split(": ")[1:])
    #the front of a header is just the header
    output+=["s_string(\"%s: \");"%front]
    #now the value...

    vars=variableizeString(back)
    return output+vars
    

class HTTPWizard:
    def castspell(self,data):
        firstline=data[0]
        #create a place to store our output!
        output=[]
        #print firstline
        #handle the first line in this if block if it has any arguments
        if firstline.count("?") > 0:
            lastchar=firstline.index("?")
            firstpart=firstline[:lastchar]
            output.append( "s_string(\""+firstpart +"?\");" ) 
            arguments=firstline[lastchar:]
            lastchar=arguments.index(" ")
            httpver=arguments[lastchar:]
            arguments=arguments[1:lastchar]

            #print "arguments="+arguments
            output.append( "s_setfirstvariable();" )
            output.append( "s_string_variables('&',\""+arguments+"\");" )
            output.append( "s_string(\""+httpver+"\\r\\n\");" )
        else:
            output.append( "s_string(\""+firstline +"\\r\\n\");" )
        
        
        #now we're in the header handling loop
        #this actually makes a copy of data, how lame is that?
        i=0
        for line in data[1:]:
            i=i+1
            #replace quotes with backslashed quotes
            line=line.replace("\"","\\\"")
            #handle cookies (or not)
            wehandlecookies=0
            if wehandlecookies and line.count("Cookie:"):
                output.append( "s_string_or_env(\"COOKIE\",\""+line+"\\r\\n\");" )
            #handle content length
            elif (line.count("Content-Length:") or line.count("Content-length:")):
                output.append( "s_string(\"Content-Length: \");" )
                output.append( "s_blocksize_string(\"post\",7);" )
                output.append( "s_string(\"\\r\\n\");" )
            #find the newline
            elif line.count(":") == 0:
                output.append( "s_string(\""+line+"\\r\\n\");" )
                break;
            else: 
                output+=variableizeHeader(line)
                output.append("s_string(\"\\r\\n\");")
                
            
        #Handle the Body    
        body=data[i+1:]

        output.append( "s_block_start(\"post\");" )

        newheader=0
        for line in body:
            words=line.split(" ")
            if words[0].upper()==words[0]:
                #we found a new header, not a body
                newheader=1
                break
            #backslashes
            line=line.replace("\\","\\\\")
            #quotes
            line=line.replace("\"","\\\"")
            #any variables
            if (len(line)>0):
                output.append( "s_setfirstvariable();" )
                output.append( "s_string_variables('&',\""+line+"\");" )
                output.append( "s_string(\"\\r\\n\"); ")

        output.append( "s_block_end(\"post\");" )
        if newheader:
            output+=self.castspell(body)
        return output
            
#from FrogEdit demo
def chomp(line):
    line = string.split(line,'\n')[0]
    return string.split(line,'\r')[0]

if __name__ == '__main__':
    import string
    if len(sys.argv) > 1:
        f=open(sys.argv[1],"r")
        #read entire file
        contents=f.readlines()
        f.close()
        contents = [chomp(line) for line in contents]
        wiz = HTTPWizard()
        output=wiz.castspell(contents)
        for line in output:
            print line        
    else:
        print "Usage: httpwizard.py file"
        
