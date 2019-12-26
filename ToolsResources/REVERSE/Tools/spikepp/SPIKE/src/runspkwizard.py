#!/usr/bin/python
#runspkwizard.py

#pops up a window which offers the users the ability to run
#a .spk file!

class RunSPKWizard(wxWizard):
    def __init__(self,filename):
        fileName=filename
        wxWizard.init(NULL, -1, "Run a SPIKE script "+filename)
        target = localhost
        port = 80
        skipvariables=0
        skipfuzzstrings=0
        generatePulldowns()


    def Go(self):
        Create()
        RunWizard(firstpage)

    def generatePulldowns(self)
        pulldowns = [ "./generic_web_server_fuzz "+target" "+port+" "+filename+" "+skipvariables+" "+skipfuzzstrings ]
        aboutmessages = ["Sends requests to a web page to attempt to crash it or find ODBC errors"]
