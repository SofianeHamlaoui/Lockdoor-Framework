#!/usr/bin/env python3.6

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
import sys
import json
import telnetlib
import glob
import signal
import random
import queue
import threading
import atexit
import requests
#requests
import base64
from getpass import getpass
from subprocess import *
from sys import argv
from platform import system
from urllib.parse import urlparse
from xml.dom import minidom
from optparse import OptionParser
from time import sleep
from pathlib import Path
##########################
###### Answers
yes = set(['yes', 'y', 'ye', 'Y'])
no = set(['no', 'n', 'nop', 'N'])
#Adding Lockdoor Logo
lockdoorlogo = """
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

"""
###################################
def exit_handler():
    print ("")
    print("                 \033[91m-[!]- LOCKDOOR IS EXITING -[!]-\033[0m")
atexit.register(exit_handler)
##################################
# Variables
config = str(Path.home()) + "/.config/lockdoor"
################DIRC
f = open('config.conf')
contents = f.read().rstrip('\n')
f.close()
installdirc = contents.replace('Location:', '')
################
cwd = os.getcwd()
Version = "0.3_TEST"
verbose = yes
##############
### REQ Functions
# Print Logo
def printlogo():
    print(lockdoorlogo)
# Press Enter to Continue
def oktocont():
    ans = input("\033[0;36mPress Enter to Continue...\033[0m")
# ClearScreen
def clearscreen():
    os.system('clear')
# Print Space
def Printspace():
    print("")
#Check root
def checkroot():
  if os.geteuid() != 0:
    printlogo()
    exit("              \033[1;33;40m-[!]- This Tool Must Run As ROOT -[!]-\033[0m")
#Show About 
#Show About 
def showabout():
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
# Updating Lockdoor
def updatelockdoor():
    print ("[Confirmation] : You want to update Lockdoor ? (Y / N) ")
    choiceupdate = input()
    if choiceupdate in yes:
        os.system("chmod +x " + cwd + "/update.sh")
        os.system("sh " + cwd + "/update.sh")
        os.system("lockdoor")
        
def checkconnect():
    url='http://www.google.com/'
    try:
        _ = requests.get(url, timeout=2)
        return True
    except requests.ConnectionError:
        exit("          \033[1;33;40m-[!]- YOU NEED INTERNET CONNECTION TO RUN LOCKDOOR -[!]-\033[0m")
    return False
##############################################################################################
################# INIT
clearscreen()
printlogo()
checkroot()
checkconnect()
######VERSION
with urllib.request.urlopen('https://raw.githubusercontent.com/SofianeHamlaoui/Lockdoor-Framework/master/VERSION') as response:
    resp = str(response.read())
    if Version in resp:
       print("                  \033[92m[\u2713]\033[0m \033[94m LOCKDOOR IS UP TO DATE \033[0m \033[92m[\u2713]\033[0m")
    else:
        ans = input("           \033[91m[!]\033[0m   \033[94mLOCKDOOR ISN'T UP TO DATE ! UPDATE IT ? (Y/N) : \033[0m")
        Printspace()
        if not ans in yes:
            print("                 \033[91m[!]\033[0m   \033[94mLOCKDOOR IS OUTDATED \033[0m")
        else:
            updatelockdoor() 
Printspace()
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
    print((lockdoorlogo + """
\033[94m         [ R00T MENU ] \033[0m

        Make A Choice :

    1)  Information Gathering
    2)  Password Attacks
    3)  Wireless Testing
    4)  Exploitation Tools
    5)  Sniffing & Spoofing
    6)  Web Hacking
    7)  Private Web Hacking
    8)  Post Exploitation
    ------------------------
    a)    About  Lockdoor
    u)    Update Lockdoor
    q)    Leave  Lockdoor
       """))
    choice = input("\033[92mLockdoor~# \033[0m")
    os.system('clear')
    if choice == "1":
        infogathering()
    elif choice == "2":
        passwd_attack()
    elif choice == "3":
        wire()
    elif choice == "4":
        exp()
    elif choice == "5":
        snif()
    elif choice == "6":
        webhack()
    elif choice == "7":
        dzz()
    elif choice == "8":
        postexp()
    elif choice == "a":
        showabout()
    elif choice == "u":
        updatelockdoor()
    elif choice == "q":
        clearscreenean(), sys.exit()
    elif choice == "":
        menuprinc()
    else:
        menuprinc()
def infogathering():
    print((lockdoorlogo + """
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
       """))
    choice = input("\033[92mLockdoor~# \033[0m")
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
        print(("           " + dirsearch.title))
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/maurosoria/dirsearch.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print(("           " + dirsearch.title))
        Printspace()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/DirSearch""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/DirSearch""")
        os.system("echo python3 " + installdirc + tool_dir + "/dirsearch.py >> /usr/local/bin/DirSearch")
        os.system("chmod +x /usr/local/bin/DirSearch")
        print(("You can now use " + "\033[91m" + dirsearch.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
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
        wordlist = input("Select the wordlist file : ")
        address = input("Enter an address : ")
        printlogo()
        Printspace()
        os.system("brut3k1t -s"  + service + " -a " + address + " -u " + user + " -w " + wordlist)
        oktocont()
        infogathering()
    else:
        
        tool_dir = "/INFO-GATH/Tools/brut3k1t"
        clearscreen()
        printlogo()
        Printspace()
        print(("           " + brut3k1t.title))
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/ex0dus-0x/brut3k1t.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print(("           " + brut3k1t.title))
        Printspace()
        print("\033[91m\033[91mInstalling ...\033[0m\033[0m")
        Printspace()
        os.system("cd " + installdirc + tool_dir + " && python2 setup.py install" + null)
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
        print(("           " + gobuster.title))
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/OJ/gobuster.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print(("           " + gobuster.title))
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
        os.system("enyx" + " " + snmpversion + " " + communitystring + " " + ipad)
        Printspace()
        oktocont()
        infogathering()
    else:
        clearscreen()
        printlogo()
        print(("           " + enyx.title))
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/trickster0/Enyx.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print(("           " + enyx.title))
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
    clearscreen()
    printlogo()
    print(("           " + Goohak.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/1N3/Goohak.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print(("           " + Goohak.title))
    Printspace()
    print(("You can now use " + "\033[91m" + Goohak.title + "\033[90m" + " from Lockdoor [\033[92m lockdoor \033[90m ]" ))
    print("[ \033[92mDon't forget to be root !\033[90m]" )
    oktocont()
    os.system("""echo "#!/bin/bash" > /usr/local/bin/Goohak""")
    os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Goohak""")
    os.system("echo " + installdirc + tool_dir + "/goohak >> /usr/local/bin/Goohak")
    os.system("chmod +x " + installdirc + tool_dir + "/goohack")
    os.system("chmod +x /usr/local/bin/Goohak")
    infogathering()

def Nasnum():
    Nasnum.title = "Nasnum : The NAS Enumerator"
    tool_dir = "/INFO-GATH/Tools/Nasnum"
    clearscreen()
    printlogo()
    print(("           " + Nasnum.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/tcstool/Nasnum.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print(("           " + Nasnum.title))
    Printspace()
    smb = input("Want to grab Grab SMB info ? (Y/N) : ")
    nsf = input("Want to grab Grab NFS info ? (Y/N) : ")
    csnmp = input("Want to Check SNMP for connection data ? (Y/N) : ")
    anonftp = input("Want to Check for anonymous FTP and enumerate dirs/files ? (Y/N) : ")
    target = input("Select A Target : ")
    if smb in yes:
        smb = "-s"
    else:
        smb = ""
    if nsf in yes:
        nsf = "-n"
    else:
        nsf = ""
    if csnmp in yes:
        csnmp = "-b"
    else:
        csnmp = ""
    if anonftp in yes:
        anonftp = "-f"
    else:
        anonftp = ""

    os.system("cd " + installdirc + tool_dir + " && chmod +x ./nasnum.sh")
    clearscreen()
    printlogo()
    Printspace()
    os.system("sh " + installdirc + tool_dir + "/nasnum.sh " + target + " " + smb + " " + nsf + " " + csnmp + " " + anonftp)

def Sublist3r():
    Sublist3r.title = "Sublist3r : Fast subdomains enumeration tool for penetration testers"
    tool_dir = "/INFO-GATH/Tools/Sublist3r"
    clearscreen()
    printlogo()
    print(("           " + Sublist3r.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/aboul3la/Sublist3r.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print(("           " + Sublist3r.title))
    Printspace()
    domaine = input("Select a Target: ")
    bruteforce = input("Enable the subbrute bruteforce module ? (Y/N) : ")
    verbose = input("Enable the verbose mode and display results in realtime ? (Y/N) : ")
    ports = input("Specify tcp ports for subdomains finding : ")
    threads = input("Number of threads to use for subbrute bruteforce ?  : ")
    engines = input("Specify a comma-separated list of search engines : ")

    if bruteforce in yes:
        bruteforce = "-b"
    else:
        bruteforce = ""
    if verbose in yes:
        verbose = "-v"
    else:
        verbose = ""
    clearscreen()
    printlogo()
    Printspace()
    print("\033[91mInstalling ...\033[0m")
    Printspace()
    os.system("pip install -r " + installdirc + tool_dir + "/requirements.txt" + null)
    clearscreen()
    printlogo()
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && python sublist3r.py -d " + domaine + " " + bruteforce + " " + verbose + " -p " + ports + " -e " + engines + " -t " + threads)

def wafw00f():
    wafw00f.title = "wafw00f : identify and fingerprint Web Application Firewall "
    tool_dir = "/INFO-GATH/Tools/wafw00f"
    clearscreen()
    printlogo()
    print(("           " + wafw00f.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/EnableSecurity/wafw00f.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print(("           " + wafw00f.title))
    Printspace()
    domaine = input("Select a Target: ")
    clearscreen()
    printlogo()
    Printspace()
    print("\033[91mInstalling ...\033[0m")
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && python setup.py install" + null)
    clearscreen()
    printlogo()
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && wafw00f " + domaine)

def sherlock():

    sherlock.title = "sherlock : Find usernames across social networks"
    tool_dir = "/INFO-GATH/Tools/sherlock"
    clearscreen()
    printlogo()
    print(("           " + sherlock.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/sherlock-project/sherlock.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print(("           " + sherlock.title))
    Printspace()
    clearscreen()
    printlogo()
    print("\033[91mInstalling ...\033[0m.")
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && pip3 install --upgrade setuptools && pip3 install -r requirements.txt" + null)
    clearscreen()
    printlogo()
    Printspace()
    target = input("Enter the target username : ")
    clearscreen()
    printlogo()
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && python3 sherlock.py" + " " + target)

def snmpwn():
    snmpwn.title = "snmpwn : An SNMPv3 User Enumerator and Attack tool"
    tool_dir = "/INFO-GATH/Tools/snmpwn"
    clearscreen()
    printlogo()
    print(("           " + snmpwn.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/hatlord/snmpwn.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print(("           " + snmpwn.title))
    Printspace()
    clearscreen()
    printlogo()
    print("\033[91mInstalling ...\033[0m.")
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && gem install bundler && bundle install" + null)
    clearscreen()
    printlogo()
    Printspace()
    hosts = input("Enter the targets hosts file : ")
    users = input("List of users you want to try : ")
    passlist = input("Password list for attacks : ")
    enclist = input("Encryption Password List for AuthPriv types : ")
    hosts = "--hosts " + hosts
    users = "--users " + users
    passlist = "--passlist " + passlist
    enclist = "--enclist " + enclist
    clearscreen()
    printlogo()
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && ruby snmpwn.rb" + " " +  hosts + " " + users + " " + passlist + " " + enclist)

def Striker():
    Striker.title = "Striker : an offensive information and vulnerability scanner."
    tool_dir = "/INFO-GATH/Tools/Striker"
    clearscreen()
    printlogo()
    print(("           " + Striker.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/s0md3v/Striker.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print(("           " + Striker.title))
    Printspace()
    clearscreen()
    printlogo()
    print("\033[91mInstalling ...\033[0m.")
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + tool_dir + "/requirements.txt" + null)
    clearscreen()
    printlogo()
    Printspace()
    target = input("Select a Target : ")
    clearscreen()
    printlogo()
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && python striker.py " + target)

def theHarvester():
    theHarvester.title = "theHarvester : E-mails, subdomains and names Harvester "
    tool_dir = "/INFO-GATH/Tools/theHarvester"
    clearscreen()
    printlogo()
    print(("           " + theHarvester.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/laramies/theHarvester.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print(("           " + theHarvester.title))
    Printspace()
    clearscreen()
    printlogo()
    print("\033[91mInstalling ...\033[0m.")
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
    clearscreen()
    printlogo()
    Printspace()
    target = input("Select a Target : ")
    Printspace()
    print(""" Sources : 
        baidu, bing, bingapi, censys, crtsh, dnsdumpster,
        dogpile, duckduckgo, exalead, github-code, google,
        hunter, intelx, linkedin, netcraft, securityTrails,
        threatcrowd, trello, twitter, vhost, virustotal, yahoo
    """)
    Printspace()
    source = input("Select a Soruce (Ex : bing,google,baidu) : ")
    clearscreen()
    printlogo()
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && python3 theHarvester.py -d " + target + " -l 500 -b " + source)

def URLextractor():
    URLextractor.title = "URLextractor : an  Information gathering & website reconnaissance tool"
    tool_dir = "/INFO-GATH/Tools/URLextractor"
    clearscreen()
    printlogo()
    print(("           " + URLextractor.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/eschultze/URLextractor.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print(("           " + URLextractor.title))
    Printspace()
    target = input("Select a Target : ")
    clearscreen()
    printlogo()
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && ./extractor.sh " + target)

def denumerator():
    denumerator.title = "denumerator.py : Enumerates list of subdomains"
    tool_dir = "/INFO-GATH/Tools/"
    clearscreen()
    printlogo()
    print(("           " + denumerator.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("wget -O " + installdirc + tool_dir + "denumerator.py https://raw.githubusercontent.com/bl4de/security-tools/master/denumerator/denumerator.py" + null)
    clearscreen()
    printlogo()
    print(("           " + denumerator.title))
    Printspace()
    targetlist = input("Enter the Targets list: ")
    clearscreen()
    printlogo()
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && python2 ./denumerator.py -f " + targetlist + " -t 10")

##### INFO-GATHERING : Manual

def Photon():
    Photon.title = "Photon : an incredibly fast crawler designed for OSINT."
    tool_dir = "/INFO-GATH/Tools/Photon"
    clearscreen()
    printlogo()
    print(("           " + Photon.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/s0md3v/Photon.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print(("           " + Photon.title))
    Printspace()
    clearscreen()
    printlogo()
    print("\033[91mInstalling ...\033[0m.")
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
    clearscreen()
    printlogo()
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && python3 photon.py ")
    clearscreen()
    printlogo()
    print(("           " + Photon.title))
    Printspace()

    os.system("""echo "#!/bin/bash" > /usr/local/bin/photon""")
    os.system("echo python " + installdirc + tool_dir + "/photon.py >> /usr/local/bin/photon")
    os.system("chmod +x /usr/local/bin/photon")
    print("Use : photon [command]")
    Printspace()

def Raccoon():
    Raccoon.title = "Raccoon : an offensive security tool for reconnaissance and vulnerability scanning."
    tool_dir = "/INFO-GATH/Tools/Raccoon"
    clearscreen()
    printlogo()
    print(("           " + Raccoon.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/evyatarmeged/Raccoon.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print(("           " + Raccoon.title))
    Printspace()
    clearscreen()
    printlogo()
    print("\033[91mInstalling ...\033[0m.")
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
    os.system("cd " + installdirc + tool_dir + " && python3 setup.py install" + null)
    clearscreen()
    printlogo()
    print(("           " + Raccoon.title))
    Printspace()
    target = input("Select a Target (use 'raccoon --help' for more options) :  ")
    Printspace()
    os.system("raccoon " + target)
    print("For more options, Use raccoon --help")
    Printspace()
    
def dnsrecon():
    dnsrecon.title = "DnsRecon : A DNS Enumeration Script"
    tool_dir = "/INFO-GATH/Tools/DnsRecon"
    clearscreen()
    printlogo()
    print(("           " + dnsrecon.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/darkoperator/dnsrecon.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print(("           " + dnsrecon.title))
    Printspace()
    clearscreen()
    printlogo()
    print("\033[91mInstalling ...\033[0m.")
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
    clearscreen()
    printlogo()
    print(("           " + dnsrecon.title))
    Printspace()
    os.system("""echo "#!/bin/bash" > /usr/local/bin/dnsrecon""")
    os.system("echo python " + installdirc + tool_dir + "/dnsrecon.py >> /usr/local/bin/dnsrecon")
    os.system("chmod +x /usr/local/bin/dnsrecon")
    target = input("Select a Target (use 'dnsrecon --help' for more options) :  ")
    Printspace()
    os.system("raccoon " + target)
    print("For more options, Use dnsrecon --help")
    Printspace()

##### INFO-GATHERING : FrameWorks

def ReconDog():
    ReconDog.title = "ReconDog # Reconnaissance Swiss Army Knife"
    tool_dir = "/INFO-GATH/Tools/ReconDog"
    clearscreen()
    printlogo()
    print(("           " + ReconDog.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/s0md3v/ReconDog.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print(("           " + ReconDog.title))
    Printspace()
    clearscreen()
    printlogo()
    print("\033[91mInstalling ...\033[0m.")
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
    clearscreen()
    printlogo()
    print(("           " + ReconDog.title))
    Printspace()
    os.system("chmod +x " + installdirc + tool_dir + "/dog")
    os.system("""echo "#!/bin/bash" > /usr/local/bin/recondog""")
    os.system("echo " + installdirc + tool_dir + "/dog >> /usr/local/bin/recondog")
    os.system("chmod +x /usr/local/bin/recondog")
    print("For more options, Use recondog --help")
    Printspace()
    os.system("recondog")

def RED_HAWK():
    tool_dir = "/INFO-GATH/Tools/RED_HAWK"
    if os.path.exists('/usr/local/bin/RED_HAWK'):
        printlogo()
        Printspace()
        os.system("git clone https://github.com/Tuhinshubhra/RED_HAWK.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        Printspace()
        os.system("RED_HAWK")
        Printspace()
        oktocont()
        infogathering()
    else:
        RED_HAWK.title = "RED_HAWK : All in one tool for Information Gathering, Vulnerability Scanning and Crawling"
        tool_dir = "/INFO-GATH/Tools/RED_HAWK"
        clearscreen()
        printlogo()
        print(("           " + RED_HAWK.title))
        Printspace()
        print("\033[91mDownloading ...\033[0m")
        Printspace()
        os.system("git clone https://github.com/Tuhinshubhra/RED_HAWK.git " + installdirc + tool_dir + null)
        clearscreen()
        printlogo()
        print(("           " + RED_HAWK.title))
        Printspace()
        clearscreen()
        printlogo()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/RED_HAWK""")
        os.system("echo php " + installdirc + tool_dir + "/rhawk.php >> /usr/local/bin/RED_HAWK")
        os.system("chmod +x /usr/local/bin/RED_HAWK")
        print(("You can now use " + "\033[91m" + dirsearch.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        oktocont()
        infogathering()
        
def Dracnmap():
    Dracnmap.title = "Dracnmap : Info Gathering Framework"
    tool_dir = "/INFO-GATH/Tools/Dracnmap"
    clearscreen()
    printlogo()
    print(("           " + Dracnmap.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/Screetsec/Dracnmap.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print(("           " + Dracnmap.title))
    Printspace()
    os.system("chmod +x " + installdirc + tool_dir + "/dracnmap-v2.2.sh")
    os.system("""echo "#!/bin/bash" > /usr/local/bin/Dracnmap""")
    os.system("echo " + installdirc + tool_dir + "/dracnmap-v2.2.sh >> /usr/local/bin/Dracnmap")
    os.system("chmod +x /usr/local/bin/Dracnmap")
    print("For more options, Use Dracnmap --help")
    Printspace()
    os.system("Dracnmap")


##### WEB HACKING : Auto

def Spaghetti():
    Spaghetti.title = "Spaghetti - Web Application Security Scanner"
    tool_dir = "/WEB/Tools/Spaghetti"
    clearscreen()
    printlogo()
    print(("           " + Spaghetti.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/c0ll3cti0n/Spaghetti.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print(("           " + Spaghetti.title))
    Printspace()
    clearscreen()
    printlogo()
    print("\033[91mInstalling ...\033[0m.")
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && pip2 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
    clearscreen()
    printlogo()
    print(("           " + Spaghetti.title))
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
    os.system("cd " + installdirc + tool_dir +  " && python2 spaghetti.py -u " + target + " -s " + scoption + crawler + ranagent + agent + proxy + verbose)
    Printspace()

def BruteXSS():
    BruteXSS.title = "BruteXSS :  BruteXSS is a tool to find XSS vulnerabilities in web application "
    tool_dir = "/WEB/Tools/BruteXSS"
    clearscreen()
    printlogo()
    print(("           " + BruteXSS.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/rajeshmajumdar/BruteXSS " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print(("           " + BruteXSS.title))
    Printspace()
    clearscreen()
    os.system("python2 " + installdirc + tool_dir + "/brutexss.py" + null)

def Jdorker():
    Jdorker.title = "J-dorker : a Website List grabber from Bing"
    tool_dir = "/WEB/Tools/Jdorker"
    clearscreen()
    printlogo()
    print(("           " + Jdorker.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/04x/J-dorker.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print(("           " + Jdorker.title))
    Printspace()
    clearscreen()
    os.system("python2 " + installdirc + tool_dir + "/priv8dorker.py")

def SqliV():
    SqliV.title = "SqliV : massive SQL injection vulnerability scanner"
    tool_dir = "/WEB/Tools/SqliV"
    clearscreen()
    printlogo()
    print(("           " + SqliV.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/SofianeHamlaoui/sqliv.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print("\033[91mInstalling ...\033[0m.")
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && pip2 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
    os.system("python2 " + installdirc + tool_dir + "setup.py -i" + null)
    clearscreen()
    Printspace()
    os.system("""echo "#!/bin/bash" > /usr/local/bin/SqliV""")
    os.system("echo python2 " + installdirc + tool_dir + "/sqliv.py >> /usr/local/bin/SqliV")
    os.system("chmod +x /usr/local/bin/SqliV")
    Printspace()
    print(("           " + SqliV.title))
    Printspace()
    mode = input("""Select A Mode : 
        1) Multiple domain scanning with SQLi dork
        2) Targetted scanning
        3) Reverse domain and scanning
        
        Choice : """)
    if mode == "1":
        dork = input("Enter a Dork : ")
        engine = input("Enter a Searcg Engine : ")
        os.system("SqliV -d " + dork + " -e " + engine)
    elif mode == "2":
        target = ("Select a Target : ")
        os.system("SqliV -t " + target)
    elif mode == "3":
        target = ("Select a Target : ")
        rev = " -r "
        os.system("SqliV -t " + target + rev)
    else:
        print("Non valide Mode")
        os.system("SqliV")

##### WEB HACKING : Manual
def HTTPoxyScan():
    HTTPoxyScan.title = "HTTPoxyScan : HTTPoxy Exploit Scanner by 1N3"
    tool_dir = "/WEB/Tools/HTTPoxyScan"
    clearscreen()
    printlogo()
    print(("           " + HTTPoxyScan.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/1N3/HTTPoxyScan.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print(("           " + HTTPoxyScan.title))
    Printspace()
    os.system("python2 " + installdirc + tool_dir + "/httpoxyscan.py")

def CMSmap():
    CMSmap.title = "CMSmap : A CMS scanner "
    tool_dir = "/WEB/Tools/CMSmap"
    clearscreen()
    printlogo()
    print(("           " + CMSmap.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/Dionach/CMSmap.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print(("           " + CMSmap.title))
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && python3 setup.py install >/dev/null")
    clearscreen()
    os.system("cmsmap -h")

def droopescan():
    droopescan.title = "droopescan : scanner , identify , CMSs , Drupal , Silverstripe."
    tool_dir = "/WEB/Tools/droopescan"
    clearscreen()
    printlogo()
    print(("           " + droopescan.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/droope/droopescan.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print("\033[91mInstalling ...\033[0m.")
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && pip install -r " + installdirc + tool_dir + "/requirements.txt" + null)
    os.system("cd " + installdirc + tool_dir + " && python setup.py install " + null)
    clearscreen()
    Printspace()
    Printspace()
    print(("           " + droopescan.title))
    Printspace()
    os.system("droopescan")

def Optiva():
    Optiva.title = "Optiva-Framework : a  Web Application Scanner"
    tool_dir = "/WEB/Tools/Optiva"
    clearscreen()
    printlogo()
    print(("           " + Optiva.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/joker25000/Optiva-Framework " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print("\033[91mInstalling ...\033[0m.")
    Printspace()
    os.system("chmod +x " + installdirc + tool_dir + "/installer.sh")
    os.system("sh " + installdirc + tool_dir + "/installer.sh")
    clearscreen()
    Printspace()
    Printspace()
    print(("           " + Optiva.title))
    Printspace()
    os.system("optiva")

def V3n0M():
    V3n0M.title = "V3n0M : Pentesting scanner in Python3.6 for SQLi/XSS/LFI/RFI and other Vulns "
    tool_dir = "/WEB/Tools/V3n0M"
    clearscreen()
    printlogo()
    print(("           " + V3n0M.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/v3n0m-Scanner/V3n0M-Scanner.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print("\033[91mInstalling ...\033[0m.")
    Printspace()
    os.system("python3 " + installdirc + tool_dir + "/setup.py install" + null)
    clearscreen()
    Printspace()
    Printspace()
    print(("           " + V3n0M.title))
    Printspace()
    os.system("v3n0m")

def Atscan():
    Atscan.title = "Atscan : Advanced dork Search & Mass Exploit Scanner "
    tool_dir = "/WEB/Tools/Atscan"
    clearscreen()
    printlogo()
    print(("           " + Atscan.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/AlisamTechnology/ATSCAN.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print("\033[91mInstalling ...\033[0m.")
    Printspace()
    os.system("""echo "#!/bin/bash" > /usr/local/bin/ATScan""")
    os.system("echo perl " + installdirc + tool_dir + "/atscan.pl >> /usr/local/bin/ATScan")
    os.system("chmod +x /usr/local/bin/ATScan")
    clearscreen()
    Printspace()
    Printspace()
    print(("           " + Atscan.title))
    Printspace()
    Printspace()
    os.system("ATScan")

def WPSeku():
    WPSeku.title = "WPSeku : Advanced dork Search & Mass Exploit Scanner "
    tool_dir = "/WEB/Tools/WPSeku"
    clearscreen()
    printlogo()
    print(("           " + WPSeku.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/m4ll0k/WPSeku.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print("\033[91mInstalling ...\033[0m.")
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
    os.system("""echo "#!/bin/bash" > /usr/local/bin/WPSeku""")
    os.system("echo python3 " + installdirc + tool_dir + "/wpseku.py >> /usr/local/bin/WPSeku")
    os.system("chmod +x /usr/local/bin/WPSeku")
    clearscreen()
    Printspace()
    Printspace()
    print(("           " + WPSeku.title))
    Printspace()
    Printspace()
    os.system("WPSeku")

def XSStrike():
    XSStrike.title = "XSStrike : The Most advanced XSS scanner."
    tool_dir = "/WEB/Tools/XSStrike"
    clearscreen()
    printlogo()
    print(("           " + XSStrike.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/s0md3v/XSStrike.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print("\033[91mInstalling ...\033[0m.")
    Printspace()
    os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
    os.system("""echo "#!/bin/bash" > /usr/local/bin/XSStrike""")
    os.system("echo python3 " + installdirc + tool_dir + "/xsstrike.py >> /usr/local/bin/XSStrike")
    os.system("chmod +x /usr/local/bin/XSStrike")
    clearscreen()
    Printspace()
    Printspace()
    print(("           " + XSStrike.title))
    Printspace()
    Printspace()
    os.system("XSStrike")

def Joomscan():
    Joomscan.title = "Joomscan : Advanced dork Search & Mass Exploit Scanner "
    tool_dir = "/WEB/Tools/Atscan"
    clearscreen()
    printlogo()
    print(("           " + Joomscan.title))
    Printspace()
    print("\033[91mDownloading ...\033[0m")
    Printspace()
    os.system("git clone https://github.com/rezasp/joomscan.git " + installdirc + tool_dir + null)
    clearscreen()
    printlogo()
    print("\033[91mInstalling ...\033[0m.")
    Printspace()
    os.system("""echo "#!/bin/bash" > /usr/local/bin/Joomscan""")
    os.system("echo perl " + installdirc + tool_dir + "/joomscan.pl >> /usr/local/bin/Joomscan")
    os.system("chmod +x /usr/local/bin/Joomscan")
    clearscreen()
    Printspace()
    Printspace()
    print(("           " + Joomscan.title))
    Printspace()
    Printspace()
    os.system("Joomscan")

if __name__ == "__main__":
    
    menuprinc()