#!/usr/bin/python
#SPIKE CONSOLE
#Copyright: Dave Aitel, Immunity, Inc. 2002
#PORTIONS (small) cut and pasted from FrogEdit
#and other Python demos (under the PYTHON license)
#note: gotta ask the wxPython authors why asking
#for 2 menubars generates all sorts of confusing
#GTk+ errors instead of smoothly telling me it's
#an error!
#grr. Passing in NULL to some wxfunctions segfaults too! That's lame.

import sys, os
from   wxPython.wx import *
from   string import *
from wxPython.lib.editor import wxEditor

from httpwizard import HTTPWizard
from runspkwizard import RunSPKWizard

class spkEdit(wxEditor):
    def __init__(self,parent):
        self.parent=parent
        wxEditor.__init__(self, parent, -1, style=wxSUNKEN_BORDER)
        self.UnTouchBuffer()

    def OnModified(self, evt):
        self.TouchBuffer()
        return

        
class spkFrame(wxFrame):
    def __init__(self,filename=NULL,initial=NULL):
        wxFrame.__init__(self,NULL,-1,"SPIKE script",size=(400,400))
        self.CreateStatusBar()
        self.SetStatusText("Spike Script Window")

        self.Show()

        #here I do some wacky magic
        win = wxPanel(self,-1)
        self.editor=spkEdit(win)
        box = wxBoxSizer(wxVERTICAL)
        box.Add(self.editor, 1, wxALL|wxGROW, 1)
        win.SetSizer(box)
        win.SetAutoLayout(true)


        
        #perhaps we were given some text by a wizard
        if initial!=NULL:
            self.editor.SetText(initial)
        self.editor.UnTouchBuffer()
        
        menuFile = wxMenu()

        saveID=wxNewId()
        saveasID=wxNewId()
        insertID=wxNewId()
        runID=wxNewId()

        menuFile.Append(saveID,"&Save", "Save this File")
        EVT_MENU(self, saveID, self.OnSave)
        menuFile.Append(saveasID,"S&ave As ...", "Save this File as something else")
        EVT_MENU(self, saveasID, self.OnSaveas)
        menuFile.Append(insertID,"&Insert ", "Insert a file at the curser")
        EVT_MENU(self, insertID, self.OnInsert)
        menuRun.Append(runID,"&Run this file")
        EVT_MENU(self, runID, self.OnRun)
        
        self.MainMenu = wxMenuBar()
        self.MainMenu.Append(menuFile, "&File");

        
       
        self.SetMenuBar(self.MainMenu)
        self.editor.SetFocus()


    def SelectFileDialog(self, defaultDir=None, defaultFile=None, wildCard=None):
        if defaultDir == None:
            defaultDir = "."
        if defaultFile == None:
            defaultFile = ""
        if wildCard == None:
            wildCard = "*.*"
        fileName = None
        fileDialog = wxFileDialog(self, "Choose a file", defaultDir, defaultFile, wildCard, wxOPEN|wxMULTIPLE)
        result = fileDialog.ShowModal()
        if result == wxID_OK:
            fileName = fileDialog.GetPath()
            #wxLogMessage('You selected: %s\n' % fileName)
        fileDialog.Destroy()
        return fileName

    def SaveFileError(self, savefileName):
        wxLogMessage('Save file error.')
        self.MessageDialog("Error saving file '%s'!" % savefileName, "Error")
        
    def SaveFile(self, savefileName):
        contents = string.join(self.editor.GetText(), '\n')
        f = open(savefileName, 'w')
        if f==NULL:
            return FALSE
        f.write(contents)
        f.close()
        self.editor.UnTouchBuffer()
        self.fileName=savefileName
        return TRUE



    def OnSave(self,event):
        if self.fileName is None:
            return self.OnSaveFileAs(event)
        #wxLogMessage("Saving %s..." % self.fileName)
        if self.SaveFile(self.fileName) is not TRUE:
            self.SaveFileError(self.fileName)
        self.editor.SetFocus()
        return
    
    def OnSaveas(self,event):
        fileName = self.SelectFileDialog()
        if fileName is not None:
            self.fileName = fileName
            #wxLogMessage("Saving %s..." % self.fileName)
            if self.SaveFile(self.fileName) is not TRUE:
                self.SaveFileError(self.fileName)
        self.editor.SetFocus()
        return
    
    def OnInsert(self,event):
        return

#need to run this file to test something
    def OnRun(self,event):
        if self.editor.BufferWasTouched():
            dlg = wxMessageDialog(frame, 'You must save first.',
                          'A Error Message Box', wxOK | wxICON_INFORMATION)
            dlg.ShowModal()
            dlg.Destroy()
            return


        wiz = RunSPKWizard(self.fileName)
        wiz.Go()
        
        return



#from FrogEdit demo
def chomp(line):
    line = string.split(line,'\n')[0]
    return string.split(line,'\r')[0]

class rawEdit(wxEditor):
    def __init__(self,parent):
        self.parent=parent
        wxEditor.__init__(self, parent, -1, style=wxSUNKEN_BORDER)

        
    def load(self,filename):
        print "Raw loading ", filename
        f=open(filename,"r")
        #read entire file
        contents=f.readlines()
        f.close()
        contents = [chomp(line) for line in contents]
        if len(contents) == 0:
            contents = [""]
        self.SetText(contents)
        
class rawFrame(wxFrame):
    def __init__(self,filename):
        wxFrame.__init__(self, NULL, -1,"Raw Edit Window" + filename,wxDefaultPosition, wxSize(200, 150))
        self.CreateStatusBar()
        self.SetStatusText("Raw Edit Window")
                
        self.editor=rawEdit(self)

        self.editor.load(filename)

        menuFile = wxMenu()

        saveID=wxNewId()
        saveasID=wxNewId()
        insertID=wxNewId()

        menuFile.Append(saveID,"&Save", "Save this File")
        EVT_MENU(self, saveID, self.OnSave)
        menuFile.Append(saveasID,"S&ave As ...", "Save this File as something else")
        EVT_MENU(self, saveasID, self.OnSaveas)
        menuFile.Append(insertID,"I&nsert ", "Insert a file at the curser")
        EVT_MENU(self, insertID, self.OnInsert)

        wizardMenu = wxMenu()
        runhttpwizardID=wxNewId();
        wizardMenu.Append(runhttpwizardID, "Run &HTTP Wizard","Run HTTP Wizard Transform")
        EVT_MENU(self, runhttpwizardID, self.OnRunHTTPWizard)
                
        self.MainMenu = wxMenuBar()
        self.MainMenu.Append(menuFile, "&File");
        self.MainMenu.Append(wizardMenu, "&Wizards");
       
        self.SetMenuBar(self.MainMenu)
        return

    def OnSave(self,event):
        return
    
    def OnSaveas(self,event):
        return
    
    def OnInsert(self,event):
        return

#run an HTTP wizard, transforming our contents into a SPIKE script
    def OnRunHTTPWizard(self,event):
        print "OnRunHTTPWizard called!"
        #for line in self.editor.GetText():
        #    print line
        
        data=self.editor.GetText()
        wiz=HTTPWizard()
        output=wiz.castspell(data)
        #print output
        win = spkFrame(initial=output)
        
    

class MyFrame(wxFrame):
    def __init__(self, parent, ID, title):
        wxFrame.__init__(self, parent, ID, title,
                         wxDefaultPosition, wxSize(200, 150))
        self.CreateStatusBar()
        self.SetStatusText("This is the statusbar")

        menuFile = wxMenu()

        openfileID=wxNewId()
        menuFile.Append(openfileID,"O&pen", "Open a File")
        menuFile.AppendSeparator()
        exitID=wxNewId()
        menuFile.Append(exitID, "E&xit", "Terminate the program")

        menuHelp = wxMenu()
        aboutID=wxNewId()
        menuHelp.Append(aboutID, "&About",
                    "More information about this program")

        menuBar = wxMenuBar()
        menuBar.Append(menuFile, "&File");
        menuBar.Append(menuHelp, "&Help");

        self.SetMenuBar(menuBar)

        EVT_MENU(self, aboutID, self.OnAbout)
        EVT_MENU(self, openfileID, self.openFile)
        EVT_MENU(self, exitID,  self.TimeToQuit)
        
    def OnAbout(self, event):
        dlg = wxMessageDialog(self, "This sample program shows off\n"
                              "frames, menus, statusbars, and this\n"
                              "message dialog.",
                              "About Me", wxOK | wxICON_INFORMATION)
        dlg.ShowModal()
        dlg.Destroy()


    def TimeToQuit(self, event):
        self.Close(true)

    def openFile(self,event):
        dlg = wxFileDialog(self, "Choose a file (SPIKE Script or Raw)", ".", "", "*.*", wxOPEN)
        
        if dlg.ShowModal() == wxID_OK:
            for path in dlg.GetPaths():
                print "You selected: ", path
                extention = path[-4:len(path)]
                if extention == ".spk":
                    print "Opening a SPIKE script"
                    win = spkFrame(filename=path)
                    win.Show(true)
                else:
                    print "Opening a raw file "+path
                    win = rawFrame(path)
                    win.Show(true)
        
        dlg.Destroy()

class MyApp(wxApp):
    def OnInit(self):
        frame = MyFrame(NULL, -1, "Hello from wxPython")
        frame.Show(true)
        self.SetTopWindow(frame)
        return true

app = MyApp(0)
app.MainLoop()

        
