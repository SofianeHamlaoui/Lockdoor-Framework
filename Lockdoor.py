#!/usr/bin/python3
import sys
import argparse
import signal
import os
import time
import http.client
import subprocess
import re
import urllib.request, urllib.error, urllib.parse
import socket
import urllib.request, urllib.parse, urllib.error
import json
import telnetlib
import glob
import signal
import random
import queue
import threading
import atexit
import requests
#req
import base64
from getpass import getpass
from sys import argv
from sys import platform
from platform import system
from urllib.parse import urlparse
from xml.dom import minidom
from optparse import OptionParser
from time import sleep
from pathlib import Path
#################
###Variables
yes = set(['yes', 'y', 'ye', 'Y'])
no = set(['no', 'n', 'nop', 'N'])
cwd = os.getcwd()
Version = "1.0_TEST"
verbose = yes
config = str(Path.home()) + "/.config/lockdoor"
###Directory
f = open('lockdoor.conf')
contents = f.read().rstrip('\n')
f.close()
installdirc = contents.replace('Location:', '')
#################
###REQ Functions
#Print Logo
def printlogo():
    print("""
\033[94m            ..',,,'..           \033[0m    
\033[94m         .',;;;;;;;;,'.         \033[0m    
\033[94m      ..,;;;;;;;;;;;;;;,..      \033[0m    
\033[94m     .,;;;,'..'''''.',;;;,.     \033[0m    
\033[94m     .;;;;.  ..   .. .;;;;'     \033[0m\033[91m (                                         \033[0m    
\033[94m     .,;;;.  ...     .;;;;.     \033[0m\033[91m )\ )               )  (                   \033[0m     
\033[94m      ..,;,.  ...   .,;,..      \033[0m\033[91m (()/(            ( /(  )\ )           (   \033[0m    
\033[94m        .';;'.    .',;'.        \033[0m\033[91m /(_))  (    (   )\())(()/(  (    (   )(   \033[0m    
\033[94m    ..',,;;;;;,,,,;;;;;,,'..    \033[0m\033[91m (_))    )\   )\ ((_)\  ((_)) )\   )\ (()\ \033[0m    
\033[94m  .','.....................''.  \033[0m\033[91m | |    ((_) ((_)| |(_) _| | ((_) ((_) ((_)\033[0m    
\033[94m .',..',,,,,,,,,,,,,,,,,,,..,,. \033[0m\033[91m | |__ / _ \/ _| | / // _` |/ _ \/ _ \| '_|\033[0m    
\033[94m .;,..,;;;;;;'....';;;;;;;..,;. \033[0m\033[91m |____|\___/\__| |_\_\\__,_|\___/\___/|_|  \033[0m    
\033[94m ';;..,;;;;;,..,,..';;;;;,..,;' \033[0m\033[92m           Sofiane Hamlaoui | 2019         \033[0m    
\033[94m.';;..,;;;;,. .... .,;;;;,..;;,.\033[0m\033[92m Lockdoor : A Penetration Testing framework\033[0m   
\033[94m ';;..,;;;;'  ....  .;;;;,..;;,. \033[0m   
\033[94m .,;'.';;;;'.  ..  .';;;;,.';,.  \033[0m
\033[94m   ....;;;;;,'''''',;;;;;'...    \033[0m    
\033[94m       ..................\033[0m

""")
#Press Enter to Continue
def oktocont():
    ans = input("\033[0;36mPress Enter to Continue...\033[0m")
#ClearScreen
def clearscreen():
    os.system('clear')
#Print Space
def Printspace():
    print("")
#Check root
def checkroot():
    #Linux
    if platform == "linux" or platform == "linux2":
        if os.geteuid() != 0:
            exit("              \033[1;33;40m-[!]- This Tool Must Run As ROOT -[!]-\033[0m")
        else:
            print("                        \033[1;33;40m-[!]- Running As ROOT -[!]-\033[0m")
    #Cygwin
    elif platform == "cygwin":
        print("                        \033[1;33;40m-[!]- Running As ROOT -[!]-\033[0m")
#Confirm location
def confirmlocation():
    Printspace()
    confirm = input("\033[91m[!]\033[0m    \033[94mThe Installation directory is : " + "\033[92m" + installdirc + "\033[90m" + "\033[94m" + " Confirm ? (Y/N) : \033[91m")
    if not confirm in no:
        Printspace()
        print("          \033[94m Installation directory confirmed \033[0m" + "\033[92m" + installdirc + "\033[90m")
        Printspace()
        oktocont()
        clearscreen()
    else:
        Printspace()
        print("             Change the configuration file \033[90m(\033[92m lockdoor.conf \033[90m)")
        confirmlocation()
#Show About 
def showabout():
    clearscreen()
    printlogo()
    print("""
    
    #############################################################
    #                   Lockdoor Framework                      #
    #  A Penetration Testing framework with CyberSec Resources  #
    #############################################################
    #    -- Version: Test 0.2 07/09/2019                        #
    #    -- Developer: Sofiane Hamlaoui                         #
    #    -- Thanks: No One                                      #
    #############################################################   

                        \033[94m-[!]-Description-[!]-\033[91m
   LockDoor is a Framework aimed at helping penetration testers, 
   bug bounty hunters And cyber security engineers. 
   This tool is designed for Debian/Ubuntu/ArchLinux based 
   distributions to create a similar and familiar distribution 
   for Penetration Testing. But containing the favorite and the most used tools by 
   Pentesters. 
   As pentesters, most of us has his personal ' /pentest/ ' directory so this 
   Framework is helping you to build a perfect one.
    """)
    oktocont()
    menuprinc()
#Updating Lockdoor
def updatelockdoor():
    choiceupdate = input("            \033[92m[Confirmation]\033[90m\033[94m You want to update Lockdoor ? (Y / N) : \033[90m")
    if choiceupdate in yes:
        print("NOT SUPPORTED YET")
#Check network connection        
def checkconnect():
    url='http://www.google.com/'
    try:
        _ = requests.get(url, timeout=1)
        return True
    except requests.ConnectionError:
        exit("          \033[1;33;40m-[!]- YOU NEED INTERNET CONNECTION TO RUN LOCKDOOR -[!]-\033[0m")
    return 
def vercheck():
    with urllib.request.urlopen('https://raw.githubusercontent.com/SofianeHamlaoui/Lockdoor-Framework/master/VERSION') as response:
        resp = str(response.read())
        if Version in resp:
           print("                   \033[94m LOCKDOOR IS UP TO DATE \033[0m")
        else:
            ans = input("           \033[91m[!]\033[0m   \033[94mLOCKDOOR ISN'T UP TO DATE ! UPDATE IT ? (Y/N) : \033[0m")
            Printspace()
            if not ans in yes:
                print("                 \033[91m[!]\033[0m   \033[94mLOCKDOOR IS OUTDATED \033[0m")
            else:
                updatelockdoor()
def init():
    clearscreen()
    printlogo()
    checkroot()
    checkconnect()
    confirmlocation()
    Printspace()
    printlogo()
    vercheck()
    Printspace()
################# 
#INIT
init()
# Choosing Verbose mode 
verbose = input("\033[91m[!]\033[0m    \033[94mUse Verbose mode for tools installation process  ? (Y/N) : \033[91m")
if not verbose in yes:
    null = " >/dev/null"
else:
    null = ""
####################################
# ROOT Menu 
def menuprinc():
    clearscreen()
    printlogo()
    print("""
\033[94m         [ R00T MENU ] \033[0m

        Make A Choice :

    1)  Information Gathering
    2)  Web Hacking
    3)  UNKOWN (Soon)
    ------------------------
    a)    About  Lockdoor
    u)    Update Lockdoor
    q)    Leave  Lockdoor
       """)
    choice = input("\033[92mLockdoor~# \033[0m")
    os.system('clear')
    if choice == "1":
        infogathering()
    elif choice == "2":
        webhack()
    elif choice == "3":
        print("UNKOWN")
    elif choice == "a":
        showabout()
    elif choice == "u":
        updatelockdoor()
    elif choice == "q":
        clearscreen(), sys.exit()
    elif choice == "":
        menuprinc()
    else:
        menuprinc()

def infogathering():
    clearscreen()
    printlogo()
    print("""
    [ INFORMATION GATHERING ]

        Make A Choice :
    -[!]----- Auto ------[!]-

        1)  DirSearch
        2)  brut3k1t
        3)  Gobuster
        4)  Enyx
        5)  Goohak
        6)  Nasnum
        7)  Sublist3r
        8)  Wafw00f
        9)  Sherlock
        10) Snmpwn
        11) Striker
        12) TheHarvester
        13) URLextractor
        14) Denumerator

    -[!]----- Manual ------[!]-

        15) Photon
        16) Raccoon
        17) dnsrecon

    -[!]----- FrameWorks ------[!]-

        18) ReconDog
        19) RED_HAWK
        20) Dracnmap
    ------------------------
    b)    Back to ROOT MENU
    q)    Leave  Lockdoor
       """)
    choice = input("\033[92mLockdoor@Enum~# \033[0m")
    os.system('clear')
    if choice == "1":
      dirsearch()
    elif choice == "2":
      brut3k1t()
    elif choice == "3":
      gobuster()
    elif choice == "4":
      enyx()
    elif choice == "5":
      Goohak()
    elif choice == "6":
      Nasnum()
    elif choice == "7":
      Sublist3r()
    elif choice == "8":
      wafw00f()
    elif choice == "9":
      sherlock()
    elif choice == "10":
      snmpwn()
    elif choice == "11":
      Striker()
    elif choice == "12":
      theHarvester()
    elif choice == "13":
      URLextractor()
    elif choice == "14":
      denumerator()
    elif choice == "15":
      Photon()
    elif choice == "16":
      Raccoon()
    elif choice == "17":
      dnsrecon()
    elif choice == "18":
      ReconDog()
    elif choice == "19":
      RED_HAWK()
    elif choice == "20":
      Dracnmap()
    elif choice == "b":
      menuprinc()
    elif choice == "q":
      menuprinc()
    elif choice == "":
      infogathering()
    else:
      infogathering()

def webhack():
    clearscreen()
    printlogo()
    print("""
        [ WEB HACKING  ]

        Make A Choice :
    -[!]----- Auto ------[!]-

        1)  Spaghetti
        2)  BruteXSS
        3)  Jdorker

    -[!]----- Manual ------[!]-

        4)  CMSmap
        5)  droopescan
        6)  Optiva
        7)  dnsrecon
        8)  V3n0M
        9)  Atscan
        10) WPSeku
        11) XSStrike
        12) Joomscan
        13) WPscan

    -[!]----- FrameWorks ------[!]-

        14) Dzjecter
    ------------------------
    b)    Back to ROOT MENU
    q)    Leave  Lockdoor
       """)
    choice = input("\033[92mLockdoor@Web~# \033[0m")
    os.system('clear')
    if choice == "1":
      Spaghetti()
    elif choice == "2":
      BruteXSS()
    elif choice == "3":
      Jdorker()
    elif choice == "4":
      CMSmap()
    elif choice == "5":
      droopescan()
    elif choice == "6":
      Optiva()
    elif choice == "7":
      dnsrecon()
    elif choice == "8":
      V3n0M()
    elif choice == "9":
      Atscan()
    elif choice == "10":
      WPSeku()
    elif choice == "11":
      XSStrike()
    elif choice == "12":
      Joomscan()
    elif choice == "13":
      WPscan()
    elif choice == "14":
      Dzjecter()
    elif choice == "b":
      menuprinc()
    elif choice == "q":
      menuprinc()
    elif choice == "":
      webhack()
    else:
      webhack()

# Fonctions for tools
###### INFO-GATHERING : Auto

def dirsearch():
    tool_dir = "/INFO-GATH/Tools/dirsearch"
    dirsearch.title = "DirSearch : Web path scanner "
    if os.path.exists('/usr/local/bin/DirSearch'):
        printlogo()
        Printspace()
        os.system("git clone https://github.com/maurosoria/dirsearch.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + dirsearch.title + "\033[90m")
        Printspace()
        choice = input("Select a Target : ")
        extchoice = input("Select an extention (php,asp,aspx ..) : ")
        printlogo()
        Printspace()
        os.system("dirsearch" + " -u " + choice + " -e " + extchoice)
        Printspace()
        oktocont()
        infogathering()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + dirsearch.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/maurosoria/dirsearch.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + dirsearch.title + "\033[90m")
        Printspace()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/DirSearch""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/DirSearch""")
        os.system("echo python3 " + installdirc + tool_dir + "/dirsearch.py >> /usr/local/bin/DirSearch")
        os.system("chmod +x /usr/local/bin/DirSearch")
        print("You can now use " + "\033[91m" + dirsearch.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        Printspace()
        oktocont()
        infogathering()

def brut3k1t():
    tool_dir = "/INFO-GATH/Tools/brut3k1t"
    brut3k1t.title = "brut3k1t : A security-oriented bruteforce framework"
    if os.path.exists('/usr/bin/brut3k1t'):
        printlogo()
        Printspace()
        os.system("git clone https://github.com/ex0dus-0x/brut3k1t.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + brut3k1t.title + "\033[90m")
        Printspace()
        service = input("Select a service (ssh,smtp,xmpp,facebook,instagram,twitter,md5):")
        user = input("Choose a user : ")
        address = input("Enter an address : ")
        Printspace()
        print("      \033[92m    Check files on " + installdirc + "/PASSWORD/wordlists/\033[90m")
        Printspace()
        wordlist = input("Select the wordlist file : ")
        printlogo()
        Printspace()
        os.system("brut3k1t -s"  + service + " -a " + address + " -u " + user + " -w " + wordlist)
        oktocont()
        infogathering()
    else:
        clearscreen()
        printlogo()
        Printspace()
        print("\033[92m           " + brut3k1t.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/ex0dus-0x/brut3k1t.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + brut3k1t.title + "\033[90m")
        Printspace()
        print("\033[91m\033[91mInstalling ...\033[0m\033[0m")
        Printspace()
        os.system("cd " + installdirc + tool_dir + " && python2 setup.py install" + null)
        os.system("ln -s /usr/bin/brut3k1t /usr/local/bin/brut3k1t")
        clearscreen()
        printlogo()
        Printspace()
        print(("You can now use " + "\033[91m" + brut3k1t.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        oktocont()
        infogathering()

def gobuster():
    tool_dir = "/INFO-GATH/Tools/gobuster"
    gobuster.title = "gobuster : A DNS and VHost busting tool written in Go "
    if os.path.exists('/usr/local/bin/gobuster'):
        printlogo()
        Printspace()
        os.system("git clone https://github.com/OJ/gobuster.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + gobuster.title + "\033[90m")
        Printspace()
        choice = input("Select a Target : ")
        Printspace()
        print("      \033[92m    Check files on " + installdirc + "/PASSWORD/wordlists/\033[90m")
        Printspace()
        wordlist = input("Select the wordlist file : ")
        printlogo()
        Printspace()
        os.system("gobuster -u " + choice + " -w " + wordlist)
        Printspace()
        oktocont()
        infogathering()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + gobuster.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/OJ/gobuster.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + gobuster.title + "\033[90m")
        Printspace()
        print("\033[91mInstalling ...\033[0m")
        Printspace()
        os.system("cd " + installdirc + tool_dir + " && make")
        clearscreen()
        printlogo()
        Printspace()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/gobuster""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/gobuster""")
        os.system("echo " + installdirc + tool_dir + "/gobuster >> /usr/local/bin/gobuster")
        os.system("chmod +x /usr/local/bin/gobuster")
        print(("You can now use " + "\033[91m" + gobuster.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        oktocont()
        infogathering()

def enyx():
    enyx.title = "Enyx : An SNMP IPv6 Enumeration Tool"
    tool_dir = "/INFO-GATH/Tools/Enyx"
    if os.path.exists('/usr/local/bin/enyx'):
        printlogo()
        Printspace()
        os.system("git clone https://github.com/trickster0/Enyx.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + enyx.title + "\033[90m")
        Printspace()
        snmpversion = input("Whats the snmp Version (1 or 2c) : ")
        communitystring = input("Whats the Community string (public or private) : ")
        ipad = input("Whats the IP : ")
        printlogo()
        Printspace()
        os.system("enyx " + snmpversion + " " + communitystring + " " + ipad)
        Printspace()
        oktocont()
        infogathering()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + enyx.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/trickster0/Enyx.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + enyx.title + "\033[90m")
        Printspace()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/enyx""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/enyx""")
        os.system("echo cd "+ installdirc + tool_dir + " >> /usr/local/bin/enyx")
        os.system("echo python2 enyx.py >> /usr/local/bin/enyx")
        os.system("chmod +x /usr/local/bin/enyx")
        print(("You can now use " + "\033[91m" + enyx.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        Printspace()
        oktocont()
        infogathering()

def Goohak():
    Goohak.title = "Goohak : Launch Google Hacking Queries Against A Target Domain (Dork)"
    tool_dir = "/INFO-GATH/Tools/Goohak"
    if os.path.exists('/usr/local/bin/Goohak'):
        printlogo()
        Printspace()
        os.system("git clone https://github.com/1N3/Goohak.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + Goohak.title + "\033[90m")
        Printspace()
        domaine = input("Select a Target: ")
        user = input("what's the current user you are using ( whoami ) ? ")
        printlogo()
        Printspace()
        os.system("su -c " + "'goohak " + domaine + "' " + user)
        Printspace()
        oktocont()
        infogathering()  
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + Goohak.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/1N3/Goohak.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + Goohak.title + "\033[90m")
        Printspace()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Goohak""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Goohak""")
        os.system("echo " + installdirc + tool_dir + "/goohak >> /usr/local/bin/Goohak")
        os.system("chmod +x " + installdirc + tool_dir + "/goohak")
        os.system("chmod +x /usr/local/bin/Goohak")
        Printspace()
        oktocont()
        infogathering()

def Nasnum():
    Nasnum.title = "Nasnum : The NAS Enumerator"
    tool_dir = "/INFO-GATH/Tools/Nasnum"
    if os.path.exists('/usr/local/bin/Nasnum'):
        printlogo()
        Printspace()
        os.system("git clone https://github.com/tcstool/Nasnum.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + Nasnum.title + "\033[90m")
        Printspace()
        smb = input("Want to grab Grab SMB info ? (Y/N) : ")
        nsf = input("Want to grab Grab NFS info ? (Y/N) : ")
        csnmp = input("Want to Check SNMP for connection data ? (Y/N) : ")
        anonftp = input("Want to Check for anonymous FTP and enumerate dirs/files ? (Y/N) : ")
        target = input("Select A Target : ")
        output = input("Select an output file : ")
        if not smb in no:
            smb = "-s"
        else:
            smb = ""
        if not nsf in no:
            nsf = "-n"
        else:
            nsf = ""
        if not csnmp in no:
            csnmp = "-b"
        else:
            csnmp = ""
        if not anonftp in no:
            anonftp = "-f"
        else:
            anonftp = ""
        printlogo()
        Printspace()
        os.system("Nasnum " + output + target + " " + smb + " " + nsf + " " + csnmp + " " + anonftp)
        Printspace()
        oktocont()
        infogathering()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + Nasnum.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/tcstool/Nasnum.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + Nasnum.title + "\033[90m")
        Printspace()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Nasnum""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Nasnum""")
        os.system("echo sh " + installdirc + tool_dir + "/nasnum.sh >> /usr/local/bin/Nasnum")
        os.system("chmod +x /usr/local/bin/Nasnum")
        print(("You can now use " + "\033[91m" + Nasnum.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        Printspace()
        oktocont()
        infogathering()

def Sublist3r():
    Sublist3r.title = "Sublist3r : Fast subdomains enumeration tool for penetration testers"
    tool_dir = "/INFO-GATH/Tools/Sublist3r"
    if os.path.exists('/usr/local/bin/Sublist3r'):
        printlogo()
        Printspace()
        os.system("git clone https://github.com/aboul3la/Sublist3r.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + Sublist3r.title + "\033[90m")
        Printspace()
        domaine = input("Select a Target: ")
        bruteforce = input("Enable the subbrute bruteforce module ? (Y/N) : ")
        verbose = input("Enable the verbose mode and display results in realtime ? (Y/N) : ")
        ports = input("Specify tcp ports for subdomains finding : ")
        threads = input("Number of threads to use for subbrute bruteforce ?  : ")
        engines = input("Specify a comma-separated list of search engines : ")
        if not domaine:
            domaine = ""
        else:
            domaine = " -d " + domaine
        if not ports:
            ports = ""
        else:
            ports = " -p " + ports
        if not threads:
            threads = ""
        else:
            threads = " -t " + threads
        if not engines:
            engines = ""
        else:
            engines = " -e " + engines
        if not bruteforce in no:
            bruteforce = "-b"
        else:
            bruteforce = ""
        if not verbose in no:
            verbose = " -v "
        else:
            verbose = ""
        printlogo()
        Printspace()
        os.system("Sublist3r" + domaine + " " + bruteforce + " " + verbose + ports + engines + threads)
        Printspace()
        oktocont()
        infogathering()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + Sublist3r.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/aboul3la/Sublist3r.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + Sublist3r.title + "\033[90m")
        Printspace()
        print("\033[91mInstalling ...\033[0m")
        Printspace()
        os.system("pip install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Sublist3r""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Sublist3r""")
        os.system("echo python3 " + installdirc + tool_dir + "/sublist3r.py >> /usr/local/bin/Sublist3r")
        os.system("chmod +x /usr/local/bin/Sublist3r")
        print(("You can now use " + "\033[91m" + Sublist3r.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        oktocont()
        infogathering()

def wafw00f():
    wafw00f.title = "wafw00f : identify and fingerprint Web Application Firewall "
    tool_dir = "/INFO-GATH/Tools/wafw00f"
    if os.path.exists('/usr/local/bin/wafw00f'):
        printlogo()
        Printspace()
        os.system("git clone https://github.com/EnableSecurity/wafw00f.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + wafw00f.title + "\033[90m")
        Printspace()
        domaine = input("Select a Target: ")
        printlogo()
        Printspace()
        os.system("wafw00f "  + domaine)
        Printspace()
        oktocont()
        infogathering()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + wafw00f.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/EnableSecurity/wafw00f.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + wafw00f.title + "\033[90m")
        clearscreen()
        printlogo()
        Printspace()
        print("\033[91mInstalling ...\033[0m")
        os.system("cd " + installdirc + tool_dir + " && python setup.py install" + null)
        os.system("ln -s /usr/bin/wafw00f /usr/local/bin/wafw00f")
        print(("You can now use " + "\033[91m" + wafw00f.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        Printspace()
        oktocont()
        infogathering()

def sherlock():

    sherlock.title = "sherlock : Find usernames across social networks"
    tool_dir = "/INFO-GATH/Tools/sherlock"
    if os.path.exists('/usr/local/bin/sherlock'):
        printlogo()
        Printspace()
        os.system("git clone https://github.com/sherlock-project/sherlock.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + sherlock.title + "\033[90m")
        Printspace()
        target = input("Enter the target username : ")
        printlogo()
        Printspace()
        os.system("python " + installdirc + tool_dir + "/sherlock.py " + target)
        oktocont()
        infogathering()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + sherlock.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/sherlock-project/sherlock.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + sherlock.title + "\033[90m")
        Printspace()
        print("\033[91mInstalling ...\033[0m.")
        Printspace()
        os.system("cd " + installdirc + tool_dir + " && pip3 install --upgrade setuptools && pip3 install -r requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/sherlock""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/sherlock""")
        os.system("echo python " + installdirc + tool_dir + "/sherlock.py >> /usr/local/bin/sherlock")
        os.system("chmod +x /usr/local/bin/sherlock")
        print(("You can now use " + "\033[91m" + sherlock.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        Printspace()
        oktocont()
        infogathering()

def snmpwn():
    snmpwn.title = "snmpwn : An SNMPv3 User Enumerator and Attack tool"
    tool_dir = "/INFO-GATH/Tools/snmpwn"
    if os.path.exists('/usr/local/bin/snmpwn'):
        printlogo()
        Printspace()
        os.system("git clone https://github.com/hatlord/snmpwn.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + snmpwn.title + "\033[90m")
        Printspace()
        hosts = input("Enter the targets hosts file : ")
        users = input("List of users you want to try : ")
        passlist = input("Password list for attacks : ")
        enclist = input("Encryption Password List for AuthPriv types : ")
        hosts = "--hosts " + hosts
        users = "--users " + users
        passlist = "--passlist " + passlist
        enclist = "--enclist " + enclist
        printlogo()
        Printspace()
        os.system("snmpwn " + hosts + " " + users + " " + passlist + " " + enclist)
        Printspace()
        oktocont()
        infogathering()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + snmpwn.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/hatlord/snmpwn.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + snmpwn.title + "\033[90m")
        Printspace()
        clearscreen()
        printlogo()
        print("\033[91mInstalling ...\033[0m.")
        print("\033[91m-[!]-\033[90m    \033[94mTo install this tool you need a non-root user, Please choose one !\033[90m \033[91m-[!] \033[90m")
        user = input("\033[91m-[!]-\033[90m        \033[94mEnter the non-root user username\033[90m  : ")
        Printspace()
        os.system("su -c " + "'cd " + installdirc + tool_dir + " && gem install bundler && bundle install' " + user + null)
        Printspace()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/snmpwn""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/snmpwn""")
        os.system("echo ruby " + installdirc + tool_dir + "/snmpwn.rb >> /usr/local/bin/snmpwn")
        os.system("chmod +x /usr/local/bin/snmpwn")
        print(("You can now use " + "\033[91m" + snmpwn.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        Printspace()
        oktocont()
        infogathering()

def Striker():
    Striker.title = "Striker : an offensive information and vulnerability scanner."
    tool_dir = "/INFO-GATH/Tools/Striker"
    if os.path.exists('/usr/local/bin/Striker'):
        printlogo()
        Printspace()
        os.system("git clone https://github.com/s0md3v/Striker.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + Striker.title + "\033[90m")
        Printspace()
        target = input("Select a Target : ")
        printlogo()
        Printspace()
        os.system("python3 " + installdirc + tool_dir + "/striker.py " + target)
        Printspace()
        oktocont()
        infogathering()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + Striker.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/s0md3v/Striker.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + Striker.title + "\033[90m")
        Printspace()
        clearscreen()
        printlogo()
        print("\033[91mInstalling ...\033[0m.")
        Printspace()
        os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Striker""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Striker""")
        os.system("echo python3 " + installdirc + tool_dir + "/striker.py >> /usr/local/bin/Striker")
        os.system("chmod +x /usr/local/bin/Striker")
        print(("You can now use " + "\033[91m" + Striker.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        Printspace()
        oktocont()
        infogathering()

def theHarvester():
    theHarvester.title = "theHarvester : E-mails, subdomains and names Harvester "
    tool_dir = "/INFO-GATH/Tools/theHarvester"
    if os.path.exists('/usr/local/bin/theHarvester'):
        printlogo()
        Printspace()
        os.system("git clone https://github.com/laramies/theHarvester.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + theHarvester.title + "\033[90m")
        Printspace()
        target = input("Select a Target : ")
        target = " -d " + target
        Printspace()
        print(""" Sources : 
            baidu, bing, bingapi, censys, crtsh, dnsdumpster,
            dogpile, duckduckgo, exalead, github-code, google,
            hunter, intelx, linkedin, netcraft, securityTrails,
            threatcrowd, trello, twitter, vhost, virustotal, yahoo
            """)
        Printspace()
        source = input("Select a Soruce (Ex : bing,google,baidu) : ")
        printlogo()
        Printspace()
        os.system("theHarvester" + target + " -l 500 -b " + source)
        Printspace()
        oktocont()
        infogathering()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + theHarvester.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/laramies/theHarvester.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + theHarvester.title + "\033[90m")
        Printspace()
        clearscreen()
        printlogo()
        print("\033[91mInstalling ...\033[0m.")
        Printspace()
        os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("cd " + installdirc + tool_dir + " && python3 " + installdirc + tool_dir + "/setup.py install" + null)
        os.system("ln -s /usr/bin/theHarvester /usr/local/bin/theHarvester")
        print("You can now use " + "\033[91m" + theHarvester.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        Printspace()
        oktocont()
        infogathering()

def URLextractor():
    URLextractor.title = "URLextractor : an  Information gathering & website reconnaissance tool"
    tool_dir = "/INFO-GATH/Tools/URLextractor"
    if os.path.exists('/usr/local/bin/URLextractor'):
        printlogo()
        Printspace()
        os.system("git clone https://github.com/SofianeHamlaoui/URLextractor.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + URLextractor.title + "\033[90m")
        Printspace()
        target = input("Select a Target : ")
        printlogo()
        Printspace()
        os.system(installdirc + tool_dir + "/extractor.sh "+ target)
        Printspace()
        oktocont()
        infogathering()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + URLextractor.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/SofianeHamlaoui/URLextractor.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + URLextractor.title + "\033[90m")
        Printspace()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/URLextractor""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/URLextractor""")
        os.system("echo sh " + installdirc + tool_dir + "/extractor.sh >> /usr/local/bin/URLextractor")
        os.system("chmod +x /usr/local/bin/URLextractor")
        print("You can now use " + "\033[91m" + URLextractor.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        Printspace()
        oktocont()
        infogathering()

def denumerator():
    denumerator.title = "denumerator.py : Enumerates list of subdomains"
    tool_dir = "/INFO-GATH/Tools/"
    if os.path.exists('/usr/local/bin/denumerator'):
        printlogo()
        Printspace()
        os.system("wget -O " + installdirc + tool_dir + "denumerator.py https://raw.githubusercontent.com/bl4de/security-tools/master/denumerator/denumerator.py" + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + denumerator.title + "\033[90m")
        Printspace()
        targetlist = input("Enter the Targets list: ")
        printlogo()
        Printspace()
        os.system("denumerator -f " + targetlist + " -t 10")
        Printspace()
        oktocont()
        infogathering()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + denumerator.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("wget -O " + installdirc + tool_dir + "denumerator.py https://raw.githubusercontent.com/bl4de/security-tools/master/denumerator/denumerator.py" + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + denumerator.title + "\033[90m")
        Printspace()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/denumerator""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/denumerator""")
        os.system("echo python2 " + installdirc + tool_dir + "/denumerator.py >> /usr/local/bin/denumerator")
        os.system("chmod +x /usr/local/bin/denumerator")
        print("You can now use " + "\033[91m" + denumerator.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        clearscreen()
        printlogo()
        Printspace()

##### INFO-GATHERING : Manual

def Photon():
    Photon.title = "Photon : an incredibly fast crawler designed for OSINT."
    tool_dir = "/INFO-GATH/Tools/Photon"
    if os.path.exists('/usr/local/bin/Photon'):
        printlogo()
        Printspace()
        os.system("git clone https://github.com/s0md3v/Photon.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        Printspace()
        print("\033[92m           " + Photon.title + "\033[90m")
        Printspace()
        os.system("Photon")
        Printspace()
        print("\033[92mFor more options, Use Photon --help\033[0m")
        Printspace()
        oktocont()
        infogathering()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + Photon.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/s0md3v/Photon.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + Photon.title + "\033[90m")
        clearscreen()
        printlogo()
        print("\033[91mInstalling ...\033[0m.")
        Printspace()
        os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Photon""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Photon""")
        os.system("echo python3 " + installdirc + tool_dir + "/photon.py >> /usr/local/bin/Photon")
        os.system("chmod +x /usr/local/bin/Photon")
        print("You can now use " + "\033[91m" + Photon.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        Printspace()
        oktocont()
        infogathering()
        
def Raccoon():
    Raccoon.title = "Raccoon : an offensive security tool for reconnaissance and vulnerability scanning."
    tool_dir = "/INFO-GATH/Tools/Raccoon"
    if os.path.exists('/usr/local/bin/Raccoon'):
        printlogo()
        Printspace()
        os.system("git clone https://github.com/evyatarmeged/Raccoon.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        Printspace()
        print("\033[92m           " + Raccoon.title + "\033[90m")
        Printspace()
        target = input("Select a Target (use 'raccoon --help' for more options) :  ")
        Printspace()
        os.system("raccoon " + target)
        Printspace()
        print("\033[92mFor more options, Use raccoon --help\033[0m")
        Printspace()
        oktocont()
        infogathering()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + Raccoon.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/evyatarmeged/Raccoon.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + Raccoon.title + "\033[90m")
        Printspace()
        printlogo()
        print("\033[91mInstalling ...\033[0m.")
        Printspace()
        os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("cd " + installdirc + tool_dir + " && python3 setup.py install" + null)
        os.system("ln -s /usr/bin/raccoon /usr/local/bin/Raccoon")
        clearscreen()
        printlogo()
        Printspace()
        print(("You can now use " + "\033[91m" + Raccoon.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        oktocont()
        infogathering()
    
def dnsrecon():
    dnsrecon.title = "DnsRecon : A DNS Enumeration Script"
    tool_dir = "/INFO-GATH/Tools/DnsRecon"
    if os.path.exists('/usr/local/bin/dnsrecon'):
        printlogo()
        Printspace()
        os.system("git clone https://github.com/darkoperator/dnsrecon.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        Printspace()
        print("\033[92m           " + dnsrecon.title + "\033[90m")
        Printspace()
        target = input("Select a Target (use 'dnsrecon --help' for more options) : ")
        Printspace()
        os.system("python3 " + installdirc + tool_dir + "/dnsrecon.py -d " + target)
        Printspace()
        print("\033[92mFor more options, Use dnsrecon --help\033[0m")
        Printspace()
        oktocont()
        infogathering()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + dnsrecon.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/darkoperator/dnsrecon.git " + installdirc + tool_dir + null)
        clearscreen()
        print("\033[92m           " + dnsrecon.title + "\033[90m")
        Printspace()
        printlogo()
        print("\033[91mInstalling ...\033[0m.")
        Printspace()
        os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/dnsrecon""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/dnsrecon""")
        os.system("echo python3 " + installdirc + tool_dir + "/dnsrecon.py >> /usr/local/bin/dnsrecon")
        os.system("chmod +x /usr/local/bin/dnsrecon")
        clearscreen()
        printlogo()
        Printspace()
        print(("You can now use " + "\033[91m" + dnsrecon.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        oktocont()
        infogathering()

##### INFO-GATHERING : FrameWorks

def ReconDog():
    ReconDog.title = "ReconDog # Reconnaissance Swiss Army Knife"
    tool_dir = "/INFO-GATH/Tools/ReconDog"
    if os.path.exists('/usr/local/bin/ReconDog'):
        printlogo()
        Printspace()
        os.system("git clone https://github.com/s0md3v/ReconDog.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        Printspace()
        print("\033[92m           " + ReconDog.title + "\033[90m")
        Printspace()
        os.system("ReconDog")
        Printspace()
        oktocont()
        infogathering()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + ReconDog.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/s0md3v/ReconDog.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + ReconDog.title + "\033[90m")
        Printspace()
        print("\033[91mInstalling ...\033[0m.")
        Printspace()
        os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("chmod +x " + installdirc + tool_dir + "/dog")
        os.system("""echo "#!/bin/bash" > /usr/local/bin/ReconDog""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/ReconDog""")
        os.system("echo " + installdirc + tool_dir + "/dog >> /usr/local/bin/ReconDog")
        os.system("chmod +x /usr/local/bin/ReconDog")
        Printspace()
        print(("You can now use " + "\033[91m" + ReconDog.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        oktocont()
        infogathering()

def RED_HAWK():
    RED_HAWK.title = "RED_HAWK : All in one tool for Information Gathering, Vulnerability Scanning and Crawling"
    tool_dir = "/INFO-GATH/Tools/RED_HAWK"
    if os.path.exists('/usr/local/bin/RED_HAWK'):
        printlogo()
        Printspace()
        os.system("git clone https://github.com/Tuhinshubhra/RED_HAWK.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        Printspace()
        print("\033[92m           " + RED_HAWK.title + "\033[90m")
        Printspace()
        os.system("RED_HAWK")
        Printspace()
        oktocont()
        infogathering()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + RED_HAWK.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/Tuhinshubhra/RED_HAWK.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + RED_HAWK.title + "\033[90m")
        Printspace()
        print("\033[91mInstalling ...\033[0m.")
        Printspace()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/RED_HAWK""")
        os.system("echo php " + installdirc + tool_dir + "/rhawk.php >> /usr/local/bin/RED_HAWK")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/RED_HAWK""")
        os.system("chmod +x /usr/local/bin/RED_HAWK")
        Printspace()
        print(("You can now use " + "\033[91m" + RED_HAWK.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        oktocont()
        infogathering()
        
def Dracnmap():
    Dracnmap.title = "Dracnmap : Info Gathering Framework"
    tool_dir = "/INFO-GATH/Tools/Dracnmap"
    if os.path.exists('/usr/local/bin/Dracnmap'):
        printlogo()
        Printspace()
        os.system("git clone https://github.com/Screetsec/Dracnmap.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        Printspace()
        print("\033[92m           " + Dracnmap.title + "\033[90m")
        Printspace()
        os.system("Dracnmap")
        Printspace()
        oktocont()
        infogathering()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + Dracnmap.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/Screetsec/Dracnmap.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + Dracnmap.title + "\033[90m")
        Printspace()
        print("\033[91mInstalling ...\033[0m.")
        Printspace()
        os.system("chmod +x " + installdirc + tool_dir + "/dracnmap-v2.2.sh")
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Dracnmap""")
        os.system("echo " + installdirc + tool_dir + "/dracnmap-v2.2.sh >> /usr/local/bin/Dracnmap")
        os.system("chmod +x /usr/local/bin/Dracnmap")
        Printspace()
        print(("You can now use " + "\033[91m" + Dracnmap.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        oktocont()
        infogathering()

##### WEB HACKING : Auto

def Spaghetti():
    Spaghetti.title = "Spaghetti - Web Application Security Scanner"
    tool_dir = "/WEB/Tools/Spaghetti"
    if os.path.exists('/usr/local/bin/Spaghetti'):
        printlogo()
        Printspace()
        os.system("git clone https://github.com/c0ll3cti0n/Spaghetti.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + Spaghetti.title + "\033[90m")
        Printspace()
        target = input("Select a Target: ")
        Printspace()
        print("""Scan Options : 
            0:	Full Scan
            1:	Bruteforce (dirs,files,..)
            2:	Disclosure (ip,emails,..)
            3:	Attacks (sql,lfi,..)
            4:	Others (webdav,..)
            5:	Vulns (shellshock,..)
            6:	Fingerprint only
        """)
        scoption = input("choose a Scan Option: ")
        crawler = input("Use Deep crawling (Slow) ? (Y/N) : ")
        if crawler in yes:
            crawler = " --crawler "
        else:
            crawler = ""
        ranagent = input("use a random user agent ? (Y/N) : ")
        if ranagent in yes:
            ranagent = " --random-agent "
        else:
            ranagent = ""
        agent = input("Use a specified Agent ? (Y/N) : ")
        if agent in yes:
            agent = input("Specify the Agent ! : ")
            agent = " --agent " + agent
        else:
            agent = ""
        proxy = input("Set a proxy ? (Y/N) : ")
        if proxy in yes:
            proxy = input("Set the Proxy (host:port) : ")
            proxy = " --proxy " + proxy 
        verbose = input("Verbose output ? (Y/N) : ")
        if verbose in yes:
            verbose = "--verbose"
        else:
            verbose = ""
        printlogo()
        Printspace()
        os.system("cd  " + installdirc + tool_dir + " && python2 spaghetti.py -u " + target + " -s " + scoption + crawler + ranagent + agent + proxy + verbose)
        Printspace()
        oktocont()
        webhack()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + Spaghetti.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/c0ll3cti0n/Spaghetti.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + Spaghetti.title + "\033[90m")
        Printspace()
        clearscreen()
        printlogo()
        print("\033[91mInstalling ...\033[0m.")
        Printspace()
        os.system("cd " + installdirc + tool_dir + " && pip2 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Spaghetti""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Spaghetti""")
        os.system("echo python2 " + installdirc + tool_dir + "/spaghetti.py >> /usr/local/bin/Spaghetti")
        os.system("chmod +x /usr/local/bin/Spaghetti")
        print(("You can now use " + "\033[91m" + Spaghetti.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        Printspace()
        oktocont()
        webhack()

def BruteXSS():
    BruteXSS.title = "BruteXSS :  BruteXSS is a tool to find XSS vulnerabilities in web application "
    tool_dir = "/WEB/Tools/BruteXSS"
    if os.path.exists('/usr/local/bin/BruteXSS'):
        printlogo()
        Printspace()
        os.system("git clone https://github.com/rajeshmajumdar/BruteXSS " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + BruteXSS.title + "\033[90m")
        Printspace()
        os.system("BruteXSS")
        oktocont()
        webhack()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + BruteXSS.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/rajeshmajumdar/BruteXSS " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + BruteXSS.title + "\033[90m")
        Printspace()
        clearscreen()
        printlogo()
        print("\033[91mInstalling ...\033[0m.")
        Printspace()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/BruteXSS""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/BruteXSS""")
        os.system("echo python2 " + installdirc + tool_dir + "/brutexss.py >> /usr/local/bin/BruteXSS")
        os.system("chmod +x /usr/local/bin/BruteXSS")
        print(("You can now use " + "\033[91m" + BruteXSS.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        Printspace()
        oktocont()
        webhack()

def Jdorker():
    Jdorker.title = "J-dorker : a Website List grabber from Bing"
    tool_dir = "/WEB/Tools/Jdorker"
    if os.path.exists('/usr/local/bin/Jdorker'):
        printlogo()
        Printspace()
        os.system("git clone  https://github.com/04x/J-dorker.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + Jdorker.title + "\033[90m")
        Printspace()
        os.system("Jdorker")
        Printspace()
        oktocont()
        webhack()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + Jdorker.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/04x/J-dorker.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + Jdorker.title + "\033[90m")
        Printspace()
        clearscreen()
        printlogo()
        print("\033[91mInstalling ...\033[0m.")
        Printspace()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Jdorker""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Jdorker""")
        os.system("echo python2 " + installdirc + tool_dir + "/priv8dorker.py >> /usr/local/bin/Jdorker")
        os.system("chmod +x /usr/local/bin/Jdorker")
        print(("You can now use " + "\033[91m" + Jdorker.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        Printspace()
        oktocont()
        webhack()

##### WEB HACKING : Manual

def CMSmap():
    CMSmap.title = "CMSmap : A CMS scanner "
    tool_dir = "/WEB/Tools/CMSmap"
    if os.path.exists('/usr/local/bin/CMSmap'):
        printlogo()
        Printspace()
        os.system("git clone  https://github.com/04x/J-dorker.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + CMSmap.title + "\033[90m")
        Printspace()
        os.system("cmsmap")
        Printspace()
        oktocont()
        webhack()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + CMSmap.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/Dionach/CMSmap.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + CMSmap.title + "\033[90m")
        Printspace()
        clearscreen()
        printlogo()
        print("\033[91mInstalling ...\033[0m.")
        os.system("cd " + installdirc + tool_dir + " && python3 setup.py install >/dev/null")
        os.system("ln -s /usr/bin/cmsmap /usr/local/bin/CMSmap")
        clearscreen()
        printlogo()
        Printspace()
        print(("You can now use " + "\033[91m" + CMSmap.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        oktocont()
        webhack()
  
def droopescan():
    droopescan.title = "droopescan : scanner , identify , CMSs , Drupal , Silverstripe."
    tool_dir = "/WEB/Tools/droopescan"
    if os.path.exists('/usr/local/bin/droopescan'):
        printlogo()
        Printspace()
        os.system("git clone  https://github.com/droope/droopescan.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + droopescan.title + "\033[90m")
        Printspace()
        os.system("droopescan")
        Printspace()
        oktocont()
        webhack()
    else:    
        clearscreen()
        printlogo()
        print("\033[92m           " + droopescan.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/droope/droopescan.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + droopescan.title + "\033[90m")
        Printspace()
        clearscreen()
        printlogo()
        print("\033[91mInstalling ...\033[0m.")
        os.system("cd " + installdirc + tool_dir + " && pip install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("cd " + installdirc + tool_dir + " && python setup.py install " + null)
        os.system("ln -s /usr/bin/droopescan /usr/local/bin/droopescan")
        clearscreen()
        printlogo()
        Printspace()
        print(("You can now use " + "\033[91m" + droopescan.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        oktocont()
        webhack()

def Optiva():
    Optiva.title = "Optiva-Framework : a  Web Application Scanner"
    tool_dir = "/WEB/Tools/Optiva"
    if os.path.exists('/usr/local/bin/Optiva'):
        printlogo()
        Printspace()
        os.system("git clone  https://github.com/joker25000/Optiva-Framework " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + Optiva.title + "\033[90m")
        Printspace()
        os.system("optiva")
        Printspace()
        oktocont()
        webhack()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + Optiva.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/joker25000/Optiva-Framework " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + Optiva.title + "\033[90m")
        Printspace()
        clearscreen()
        printlogo()
        print("\033[91mInstalling ...\033[0m.")
        os.system("chmod +x " + installdirc + tool_dir + "/installer.sh")
        os.system("cd " + installdirc + tool_dir + " && sh installer.sh")
        os.system("ln -s /usr/bin/optiva /usr/local/bin/Optiva")
        clearscreen()
        printlogo()
        Printspace()
        print(("You can now use " + "\033[91m" + Optiva.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        oktocont()
        webhack()

def V3n0M():
    V3n0M.title = "V3n0M : Pentesting scanner in Python3.6 for SQLi/XSS/LFI/RFI and other Vulns "
    tool_dir = "/WEB/Tools/V3n0M"
    if os.path.exists('/usr/local/bin/V3n0M'):
        printlogo()
        Printspace()
        os.system("git clone  https://github.com/v3n0m-Scanner/V3n0M-Scanner.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + V3n0M.title + "\033[90m")
        Printspace()
        os.system("V3n0M")
        Printspace()
        oktocont()
        webhack()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + V3n0M.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/v3n0m-Scanner/V3n0M-Scanner.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + V3n0M.title + "\033[90m")
        Printspace()
        clearscreen()
        printlogo()
        print("\033[91mInstalling ...\033[0m.")
        os.system("python3 " + installdirc + tool_dir + "/setup.py install" + null)
        os.system("ln -s /usr/bin/v3n0m /usr/local/bin/V3n0M")
        clearscreen()
        printlogo()
        Printspace()
        print(("You can now use " + "\033[91m" + V3n0M.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        oktocont()
        webhack()

def Atscan():
    Atscan.title = "Atscan : Advanced dork Search & Mass Exploit Scanner "
    tool_dir = "/WEB/Tools/Atscan"
    if os.path.exists('/usr/local/bin/Atscan'):
        printlogo()
        Printspace()
        os.system("git clone  https://github.com/AlisamTechnology/ATSCAN.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + Atscan.title + "\033[90m")
        Printspace()
        os.system("Atscan")
        Printspace()
        oktocont()
        webhack()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + Atscan.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/AlisamTechnology/ATSCAN.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + Atscan.title + "\033[90m")
        Printspace()
        clearscreen()
        printlogo()
        print("\033[91mInstalling ...\033[0m.")
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Atscan""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Atscan""")
        os.system("echo perl " + installdirc + tool_dir + "/atscan.pl >> /usr/local/bin/Atscan")
        os.system("chmod +x /usr/local/bin/Atscan")
        Printspace()
        print("You can now use " + "\033[91m" + Atscan.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        Printspace()
        oktocont()
        webhack()

def WPSeku():
    WPSeku.title = "WPSeku : Advanced dork Search & Mass Exploit Scanner "
    tool_dir = "/WEB/Tools/WPSeku"
    if os.path.exists('/usr/local/bin/WPSeku'):
        printlogo()
        Printspace()
        os.system("git clone  https://github.com/m4ll0k/WPSeku.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + WPSeku.title + "\033[90m")
        Printspace()
        os.system("WPSeku")
        Printspace()
        oktocont()
        webhack()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + WPSeku.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/m4ll0k/WPSeku.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + WPSeku.title + "\033[90m")
        Printspace()
        clearscreen()
        printlogo()
        print("\033[91mInstalling ...\033[0m.")
        os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/WPSeku""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/WPSeku""")
        os.system("echo python3 " + installdirc + tool_dir + "/wpseku.py >> /usr/local/bin/WPSeku")
        os.system("chmod +x /usr/local/bin/WPSeku")
        Printspace()   
        print("You can now use " + "\033[91m" + WPSeku.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        Printspace()
        oktocont()
        webhack()

def XSStrike():
    XSStrike.title = "XSStrike : The Most advanced XSS scanner."
    tool_dir = "/WEB/Tools/XSStrike"
    if os.path.exists('/usr/local/bin/XSStrike'):
        printlogo()
        Printspace()
        os.system("git clone  https://github.com/s0md3v/XSStrike.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + XSStrike.title + "\033[90m")
        Printspace()
        os.system("XSStrike")
        Printspace()
        oktocont()
        webhack()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + XSStrike.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/s0md3v/XSStrike.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + XSStrike.title + "\033[90m")
        Printspace()
        clearscreen()
        printlogo()
        print("\033[91mInstalling ...\033[0m.")
        Printspace()
        os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/XSStrike""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/XSStrike""")
        os.system("echo python3 " + installdirc + tool_dir + "/xsstrike.py >> /usr/local/bin/XSStrike")
        os.system("chmod +x /usr/local/bin/XSStrike")
        Printspace()   
        print("You can now use " + "\033[91m" + XSStrike.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        Printspace()
        oktocont()
        webhack()

def Joomscan():
    Joomscan.title = "Joomscan : Advanced dork Search & Mass Exploit Scanner "
    tool_dir = "/WEB/Tools/Joomscan"
    if os.path.exists('/usr/local/bin/Joomscan'):
        printlogo()
        Printspace()
        os.system("git clone  https://github.com/rezasp/joomscan.git  " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + Joomscan.title + "\033[90m")
        Printspace()
        os.system("Joomscan")
        Printspace()
        oktocont()
        webhack()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + Joomscan.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/rezasp/joomscan.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + Joomscan.title + "\033[90m")
        Printspace()
        clearscreen()
        printlogo()
        print("\033[91mInstalling ...\033[0m.")
        Printspace()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Joomscan""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Joomscan""")
        os.system("echo perl " + installdirc + tool_dir + "/joomscan.pl >> /usr/local/bin/Joomscan")
        os.system("chmod +x /usr/local/bin/Joomscan")
        Printspace()   
        print("You can now use " + "\033[91m" + Joomscan.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        Printspace()
        oktocont()
        webhack()

def WPscan():
    WPscan.title = "WPscan : WordPress Vulnerability Scanner "
    tool_dir = "/WEB/Tools/WPscan"
    if os.path.exists('/usr/bin/wpscan'):
        print("\033[92m           " + WPscan.title + "\033[90m")
        Printspace()
        os.system("wpscan")
        Printspace()
        oktocont()
        webhack()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + WPscan.title + "\033[90m")
        Printspace()
        print("\033[91mInstalling ...\033[0m.")
        Printspace()
        user = input("With which user you want to install wpscan ? : ")
        os.system("gem install wpscan")
        os.system("su -c " + "'gem install wpscan' " + user)
        Printspace()   
        print("You can now use " + "\033[91m" + WPscan.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        Printspace()
        oktocont()
        webhack()

##### WEB HACKING : Framework
def Dzjecter():
    Dzjecter.title = "Dzjecter : a Server checking Tool."
    tool_dir = "/WEB/Tools/Dzjecter"
    if os.path.exists('/usr/local/bin/Dzjecter'):
        printlogo()
        Printspace()
        os.system("git clone https://github.com/joker25000/Dzjecter " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        Printspace()
        print("\033[92m           " + Dzjecter.title + "\033[90m")
        Printspace()
        os.system("Dzjecter")
        Printspace()
        oktocont()
        infogathering()
    else:
        clearscreen()
        printlogo()
        print("\033[92m           " + Dzjecter.title + "\033[90m")
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/joker25000/Dzjecter " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print("\033[92m           " + Dzjecter.title + "\033[90m")
        clearscreen()
        printlogo()
        print("\033[91mInstalling ...\033[0m.")
        Printspace()
        os.system("cd " + installdirc + tool_dir + " && pip install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Dzjecter""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Dzjecter""")
        os.system("echo python2 " + installdirc + tool_dir + "/Dzjecter.py >> /usr/local/bin/Dzjecter")
        os.system("chmod +x /usr/local/bin/Dzjecter")
        print("You can now use " + "\033[91m" + Dzjecter.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        Printspace()
        oktocont()
        webhack()

if __name__ == "__main__":
    try:
            menuprinc()
    
    except (KeyboardInterrupt):
        Printspace()
        ans = input("           \033[91m-[!]- SIGINT or CTRL-C detected. Are You sure to exit \033[92mLockdoor \033[90m\033[91m-[!] ? (Y/N)\033[0m : ")
        if not ans in yes:
            menuprinc()
        else:
            print("                 \033[91m-[!]- LOCKDOOR IS EXITING -[!]-\033[0m")