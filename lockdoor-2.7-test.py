#!/usr/bin/env python2.7

import sys
import argparse
import os
import time
import httplib
import subprocess
import re
import urllib2
import socket
import urllib
import sys
import json
import telnetlib
import glob
import random
import Queue
import threading
#requests
import base64
from getpass import getpass
from commands import *
from sys import argv
from platform import system
from urlparse import urlparse
from xml.dom import minidom
from optparse import OptionParser
from time import sleep
##########################
###### Answers
yes = set(['yes', 'y', 'ye', 'Y'])
no = set(['no', 'n', 'nop', 'N'])
#Adding Lockdoor Logo
lockdoorlogo = """\033[0m
.____                  __       .___
|    |    ____   ____ |  | __ __| _/____   ___________
|    |   /  _ \_/ ___\|  |/ // __ |/  _ \ /  _ \_  __ \

|    |__(   _  )  \___|    </ /_/ (   _  |   _  )  | \/
|_______ \____/ \___  >__|_ \____ |\____/ \____/|__|
        \/          \/     \/    \/
                      Sofiane Hamlaoui | 2019
  Lockdoor A Penetration Testing framework with Cyber Security Resources


 \033[91m"""
# Variables
intallationdirc = "/opt/sofiane/lockdoor"
cwd = os.getcwd()
verbose = yes
##############
### REQS
# Print Logo
def printlogo():
    print(lockdoorlogo)
# ClearScreen
def clearscrean():
    if system() == 'Linux':
        os.system('clear')
    if system() == 'Windows':
        os.system('cls')
# Print Space
def Printspace():
    print ""
#Check root
def checkroot():
  if os.geteuid() != 0:
    printlogo()
    exit("\033[1;33;40m[!] This Tool Must Run As ROOT [!] ! Exiting.\033[0m")
################# INIT
clearscrean()
printlogo()
#Selecting The Installation Directory
intallationdirc = raw_input("       \033[94mEnter Lockdoor's installation directory [" + intallationdirc + "]: \033[91m")
if not intallationdirc:
    intallationdirc = "/opt/sofiane/lockdoor"
Printspace()
Printspace()
# Choosing Verbose mode 
verbose = raw_input("       \033[94mDo you want to use Verbose mode (output the installation process of tools) ? (Y/N) : \033[91m")
if verbose in no:
    null = " >/dev/null"
else:
    null = ""

####################################
# ROOT Menu 
def menuprinc():
    print (lockdoorlogo + """
         [ R00T MENU ]

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
       """)
    choice = raw_input("Lockdoor~# ")
    os.system('clear')
    if choice == "1":
        dirsearch()
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
    elif choice == "u":
        updatelockdoor()
    elif choice == "q":
        clearscreanean(), sys.exit()
    elif choice == "":
        menuprinc()
    else:
        menuprinc()


# Updating Lockdoor
def updatelockdoor():
    print ("[Confirmation] : You want to update Lockdoor ? (Y / N) ")
    choiceupdate = raw_input()
    if choiceupdate in yes:
        os.system("chmod +x " + cwd + "/update.sh")
        os.system("sh " + cwd + "/update.sh")
        os.system("lockdoor")


# Fonctions for tools
###### INFO-GATHERING : Auto
def dirsearch():
    title = "DirSearch : Web path scanner "
    tool_dir = "/ToolsResources/INFO-GATH/Tools/dirsearch"
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    print "Downloading ..."
    Printspace()
    os.system("git clone https://github.com/maurosoria/dirsearch.git " + intallationdirc + tool_dir + null)
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    choice = raw_input("Select a Target : ")
    extchoice = raw_input("Select an extention (php,asp,aspx ..) : ")
    clearscrean()
    printlogo()
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && python3 dirsearch.py -u " + choice + " -e " + extchoice)

def brut3k1t():
    title = "brut3k1t : A security-oriented bruteforce framework"
    tool_dir = "/ToolsResources/INFO-GATH/Tools/brut3k1t"
    clearscrean()
    printlogo()
    Printspace()
    print("           " + title)
    Printspace()
    print "Downloading ..."
    Printspace()
    os.system("git clone https://github.com/ex0dus-0x/brut3k1t.git " + intallationdirc + tool_dir + null)
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    service = raw_input("Select a service (ssh,smtp,xmpp,facebook,instagram,twitter,md5):")
    user = raw_input("Choose a user : ")
    wordlist = raw_input("Select the wordlist file : ")
    address = raw_input("Enter an address : ")
    Printspace()
    print "Installing ..."
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && python2 setup.py install" + null)
    clearscrean()
    printlogo()
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && " "python2 brut3k1t -s"  + service + " -a " + address + " -u " + user + " -w " + wordlist)

def gobuster():
    title = "gobuster : A DNS and VHost busting tool written in Go "
    tool_dir = "/ToolsResources/INFO-GATH/Tools/gobuster"
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    print "Downloading ..."
    Printspace()
    os.system("git clone https://github.com/OJ/gobuster.git " + intallationdirc + tool_dir + null)
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    choice = raw_input("Select a Target : ")
    wordlist = raw_input("Select the wordlist file : ")
    Printspace()
    print "Installing ..."
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && make")
    clearscrean()
    printlogo()
    Printspace()
    os.system("gobuster -u " + choice + " -w " + wordlist)

def enyx():
    title = "Enyx : An SNMP IPv6 Enumeration Tool"
    tool_dir = "/ToolsResources/INFO-GATH/Tools/Enyx"
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    print "Downloading ..."
    Printspace()
    os.system("git clone https://github.com/trickster0/Enyx.git " + intallationdirc + tool_dir + null)
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    snmpversion = raw_input("Whats the snmp Version (1 or 2c) : ")
    communitystring = raw_input("Whats the Community string (public or private) : ")
    ipad = raw_input("Whats the IP : ")
    clearscrean()
    printlogo()
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && python2 enyx.py " + snmpversion + " " + communitystring + " " + ipad)

def Goohak():
    title = "Goohak : Launch Google Hacking Queries Against A Target Domain (Dork)"
    tool_dir = "/ToolsResources/INFO-GATH/Tools/Goohak"
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    print "Downloading ..."
    Printspace()
    os.system("git clone https://github.com/1N3/Goohak.git " + intallationdirc + tool_dir + null)
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    domaine = raw_input("Select a Target: ")
    clearscrean()
    printlogo()
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && goohak " + domaine)

def Nasnum():
    title = "Nasnum : The NAS Enumerator"
    tool_dir = "/ToolsResources/INFO-GATH/Tools/Nasnum"
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    print "Downloading ..."
    Printspace()
    os.system("git clone https://github.com/tcstool/Nasnum.git " + intallationdirc + tool_dir + null)
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    smb = raw_input("Want to grab Grab SMB info ? (Y/N) : ")
    nsf = raw_input("Want to grab Grab NFS info ? (Y/N) : ")
    csnmp = raw_input("Want to Check SNMP for connection data ? (Y/N) : ")
    anonftp = raw_input("Want to Check for anonymous FTP and enumerate dirs/files ? (Y/N) : ")
    target = raw_input("Select A Target : ")
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

    os.system("cd " + intallationdirc + tool_dir + " && chmod +x ./nasnum.sh")
    clearscrean()
    printlogo()
    Printspace()
    os.system("sh " + intallationdirc + tool_dir + "/nasnum.sh " + target + " " + smb + " " + nsf + " " + csnmp + " " + anonftp)

def Sublist3r():
    title = "Sublist3r : Fast subdomains enumeration tool for penetration testers"
    tool_dir = "/ToolsResources/INFO-GATH/Tools/Sublist3r"
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    print "Downloading ..."
    Printspace()
    os.system("git clone https://github.com/aboul3la/Sublist3r.git " + intallationdirc + tool_dir + null)
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    domaine = raw_input("Select a Target: ")
    bruteforce = raw_input("Enable the subbrute bruteforce module ? (Y/N) : ")
    verbose = raw_input("Enable the verbose mode and display results in realtime ? (Y/N) : ")
    ports = raw_input("Specify tcp ports for subdomains finding : ")
    threads = raw_input("Number of threads to use for subbrute bruteforce ?  : ")
    engines = raw_input("Specify a comma-separated list of search engines : ")

    if bruteforce in yes:
        bruteforce = "-b"
    else:
        bruteforce = ""
    if verbose in yes:
        verbose = "-v"
    else:
        verbose = ""
    clearscrean()
    printlogo()
    Printspace()
    print "Installing ..."
    Printspace()
    os.system("pip install -r " + intallationdirc + tool_dir + "/requirements.txt" + null)
    clearscrean()
    printlogo()
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && python sublist3r.py -d " + domaine + " " + bruteforce + " " + verbose + " -p " + ports + " -e " + engines + " -t " + threads)

def wafw00f():
    title = "wafw00f : identify and fingerprint Web Application Firewall "
    tool_dir = "/ToolsResources/INFO-GATH/Tools/wafw00f"
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    print "Downloading ..."
    Printspace()
    os.system("git clone https://github.com/EnableSecurity/wafw00f.git " + intallationdirc + tool_dir + null)
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    domaine = raw_input("Select a Target: ")
    clearscrean()
    printlogo()
    Printspace()
    print "Installing ..."
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && python setup.py install" + null)
    clearscrean()
    printlogo()
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && wafw00f " + domaine)

def sherlock():

    title = "sherlock : Find usernames across social networks"
    tool_dir = "/ToolsResources/INFO-GATH/Tools/sherlock"
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    print "Downloading ..."
    Printspace()
    os.system("git clone https://github.com/sherlock-project/sherlock.git " + intallationdirc + tool_dir + null)
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    clearscrean()
    printlogo()
    print "Installing ...."
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && pip3 install --upgrade setuptools && pip3 install -r requirements.txt" + null)
    clearscrean()
    printlogo()
    Printspace()
    target = raw_input("Enter the target username : ")
    clearscrean()
    printlogo()
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && python3 sherlock.py" + " " + target)

def snmpwn():
    title = "snmpwn : An SNMPv3 User Enumerator and Attack tool"
    tool_dir = "/ToolsResources/INFO-GATH/Tools/snmpwn"
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    print "Downloading ..."
    Printspace()
    os.system("git clone https://github.com/hatlord/snmpwn.git " + intallationdirc + tool_dir + null)
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    clearscrean()
    printlogo()
    print "Installing ...."
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && gem install bundler && bundle install" + null)
    clearscrean()
    printlogo()
    Printspace()
    hosts = raw_input("Enter the targets hosts file : ")
    users = raw_input("List of users you want to try : ")
    passlist = raw_input("Password list for attacks : ")
    enclist = raw_input("Encryption Password List for AuthPriv types : ")
    hosts = "--hosts " + hosts
    users = "--users " + users
    passlist = "--passlist " + passlist
    enclist = "--enclist " + enclist
    clearscrean()
    printlogo()
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && ruby snmpwn.rb" + " " +  hosts + " " + users + " " + passlist + " " + enclist)

def Striker():
    title = "Striker : an offensive information and vulnerability scanner."
    tool_dir = "/ToolsResources/INFO-GATH/Tools/Striker"
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    print "Downloading ..."
    Printspace()
    os.system("git clone https://github.com/s0md3v/Striker.git " + intallationdirc + tool_dir + null)
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    clearscrean()
    printlogo()
    print "Installing ...."
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && pip3 install -r " + tool_dir + "/requirements.txt" + null)
    clearscrean()
    printlogo()
    Printspace()
    target = raw_input("Select a Target : ")
    clearscrean()
    printlogo()
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && python striker.py " + target)

def theHarvester():
    title = "theHarvester : E-mails, subdomains and names Harvester "
    tool_dir = "/ToolsResources/INFO-GATH/Tools/theHarvester"
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    print "Downloading ..."
    Printspace()
    os.system("git clone https://github.com/laramies/theHarvester.git " + intallationdirc + tool_dir + null)
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    clearscrean()
    printlogo()
    print "Installing ...."
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && pip3 install -r " + intallationdirc + tool_dir + "/requirements.txt" + null)
    clearscrean()
    printlogo()
    Printspace()
    target = raw_input("Select a Target : ")
    Printspace()
    print(""" Sources : 
        baidu, bing, bingapi, censys, crtsh, dnsdumpster,
        dogpile, duckduckgo, exalead, github-code, google,
        hunter, intelx, linkedin, netcraft, securityTrails,
        threatcrowd, trello, twitter, vhost, virustotal, yahoo
    """)
    Printspace()
    source = raw_input("Select a Soruce (Ex : bing,google,baidu) : ")
    clearscrean()
    printlogo()
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && python3 theHarvester.py -d " + target + " -l 500 -b " + source)

def URLextractor():
    title = "URLextractor : an  Information gathering & website reconnaissance tool"
    tool_dir = "/ToolsResources/INFO-GATH/Tools/URLextractor"
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    print "Downloading ..."
    Printspace()
    os.system("git clone https://github.com/eschultze/URLextractor.git " + intallationdirc + tool_dir + null)
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    target = raw_input("Select a Target : ")
    clearscrean()
    printlogo()
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && ./extractor.sh " + target)

def denumerator():
    title = "denumerator.py : Enumerates list of subdomains"
    tool_dir = "/ToolsResources/INFO-GATH/Tools/"
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    print "Downloading ..."
    Printspace()
    os.system("wget -O " + intallationdirc + tool_dir + "denumerator.py https://raw.githubusercontent.com/bl4de/security-tools/master/denumerator/denumerator.py" + null)
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    targetlist = raw_input("Enter the Targets list: ")
    clearscrean()
    printlogo()
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && python2 ./denumerator.py -f " + targetlist + " -t 10")

##### INFO-GATHERING : Manual

def Photon():
    title = "Photon : an incredibly fast crawler designed for OSINT."
    tool_dir = "/ToolsResources/INFO-GATH/Tools/Photon"
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    print "Downloading ..."
    Printspace()
    os.system("git clone https://github.com/s0md3v/Photon.git " + intallationdirc + tool_dir + null)
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    clearscrean()
    printlogo()
    print "Installing ...."
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && pip3 install -r " + intallationdirc + tool_dir + "/requirements.txt" + null)
    clearscrean()
    printlogo()
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && python3 photon.py ")
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()

    os.system("""echo "#!/bin/bash" > /usr/local/bin/photon""")
    os.system("echo python " + intallationdirc + tool_dir + "/photon.py >> /usr/local/bin/photon")
    os.system("chmod +x /usr/local/bin/photon")
    print("Use : photon [command]")
    Printspace()

def Raccoon():
    title = "Raccoon : an offensive security tool for reconnaissance and vulnerability scanning."
    tool_dir = "/ToolsResources/INFO-GATH/Tools/Raccoon"
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    print "Downloading ..."
    Printspace()
    os.system("git clone https://github.com/evyatarmeged/Raccoon.git " + intallationdirc + tool_dir + null)
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    clearscrean()
    printlogo()
    print "Installing ...."
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && pip3 install -r " + intallationdirc + tool_dir + "/requirements.txt" + null)
    os.system("cd " + intallationdirc + tool_dir + " && python3 setup.py install" + null)
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    target = raw_input("Select a Target (use 'raccoon --help' for more options) :  ")
    Printspace()
    os.system("raccoon " + target)
    print("For more options, Use raccoon --help")
    Printspace()
    
def dnsrecon():
    title = "DnsRecon : A DNS Enumeration Script"
    tool_dir = "/ToolsResources/INFO-GATH/Tools/DnsRecon"
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    print "Downloading ..."
    Printspace()
    os.system("git clone https://github.com/darkoperator/dnsrecon.git " + intallationdirc + tool_dir + null)
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    clearscrean()
    printlogo()
    print "Installing ...."
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && pip3 install -r " + intallationdirc + tool_dir + "/requirements.txt" + null)
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    os.system("""echo "#!/bin/bash" > /usr/local/bin/dnsrecon""")
    os.system("echo python " + intallationdirc + tool_dir + "/dnsrecon.py >> /usr/local/bin/dnsrecon")
    os.system("chmod +x /usr/local/bin/dnsrecon")
    target = raw_input("Select a Target (use 'dnsrecon --help' for more options) :  ")
    Printspace()
    os.system("raccoon " + target)
    print("For more options, Use dnsrecon --help")
    Printspace()

##### INFO-GATHERING : FrameWorks

def ReconDog():
    title = "ReconDog # Reconnaissance Swiss Army Knife"
    tool_dir = "/ToolsResources/INFO-GATH/Tools/ReconDog"
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    print "Downloading ..."
    Printspace()
    os.system("git clone https://github.com/s0md3v/ReconDog.git " + intallationdirc + tool_dir + null)
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    clearscrean()
    printlogo()
    print "Installing ...."
    Printspace()
    os.system("cd " + intallationdirc + tool_dir + " && pip3 install -r " + intallationdirc + tool_dir + "/requirements.txt" + null)
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    os.system("chmod +x " + intallationdirc + tool_dir + "/dog")
    os.system("""echo "#!/bin/bash" > /usr/local/bin/recondog""")
    os.system("echo " + intallationdirc + tool_dir + "/dog >> /usr/local/bin/recondog")
    os.system("chmod +x /usr/local/bin/recondog")
    print("For more options, Use recondog --help")
    Printspace()
    os.system("recondog")

def RED_HAWK():
    title = "RED_HAWK : All in one tool for Information Gathering, Vulnerability Scanning and Crawling"
    tool_dir = "/ToolsResources/INFO-GATH/Tools/RED_HAWK"
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    print "Downloading ..."
    Printspace()
    os.system("git clone https://github.com/Tuhinshubhra/RED_HAWK.git " + intallationdirc + tool_dir + null)
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    clearscrean()
    printlogo()
    os.system("""echo "#!/bin/bash" > /usr/local/bin/RED_HAWK""")
    os.system("echo php " + intallationdirc + tool_dir + "/rhawk.php >> /usr/local/bin/RED_HAWK")
    os.system("chmod +x /usr/local/bin/RED_HAWK")
    Printspace()
    os.system("RED_HAWK")

def Dracnmap():
    title = "Dracnmap : Info Gathering Framework"
    tool_dir = "/ToolsResources/INFO-GATH/Tools/Dracnmap"
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    print "Downloading ..."
    Printspace()
    os.system("git clone https://github.com/Screetsec/Dracnmap.git " + intallationdirc + tool_dir + null)
    clearscrean()
    printlogo()
    print("           " + title)
    Printspace()
    os.system("chmod +x " + intallationdirc + tool_dir + "/dracnmap-v2.2.sh")
    os.system("""echo "#!/bin/bash" > /usr/local/bin/Dracnmap""")
    os.system("echo " + intallationdirc + tool_dir + "/dracnmap-v2.2.sh >> /usr/local/bin/Dracnmap")
    os.system("chmod +x /usr/local/bin/Dracnmap")
    print("For more options, Use Dracnmap --help")
    Printspace()
    os.system("Dracnmap")





########################
#########PROBLEMS 


###########

#Dracnmap()
#RED_HAWK()
#ReconDog()
#dnsrecon()
#Raccoon()
#Photon()
#denumerator()
#URLextractor()
#theHarvester()
#Striker()
#snmpwn()
#sherlock()
#Reconnoitre()
#wafw00f()
#Sublist3r()
#Nasnum()
#Goohak()
#enyx()
#checkroot()
#gobuster()
#brut3k1t()
#dirsearch()
#################
#menuprinc()
