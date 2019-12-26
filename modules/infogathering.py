import os
import sys
import main
from pathlib import Path
from datetime import datetime
from time import sleep
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
\033[94m       ..................\033[0m""")
#VAR
config = str(Path.home()) + "/.config/lockdoor/"
yes = set(['yes', 'y', 'ye', 'Y'])
no = set(['no', 'n', 'nop', 'N'])
cwd = os.getcwd()
null = ""
#Config
f = open(config + 'lockdoor.conf')
contents = f.read().rstrip('\n')
f.close()
installdirc = contents.replace('Location:', '')
##########SHRT
def oktocont():
    ans = input("\033[0;36mPress Enter to Continue...\033[0m")
def clr():
    os.system('clear')
def spc():
    print("")
def prilogspc():
    printlogo()
    spc()
def clscprilo():
    clr()
    printlogo()
def popp():
    spc()
    oktocont()
    printlogo()
    spc()
def okinf():
    spc()
    oktocont()
    menu()
def pop():
    spc()
    oktocont()
    spc()
############
###Cheatsheets
def infgathsh():
    clscprilo()
    print("\033[91mHere is the list of the files :\033[90m")
    print("\033[92m")
    os.system("     find " + installdirc + "/INFO-GATH/CHEATSHEETS/ -type f")
    print("\033[90m")
    okinf()
###TOOLS : 
def dirsearch():
    tool_dir = "/INFO-GATH/Tools/dirsearch"
    dirsearch.title = "DirSearch : Web path scanner "
    if os.path.exists('/usr/local/bin/DirSearch'):
        prilogspc()
        os.system("git clone https://github.com/maurosoria/dirsearch.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + dirsearch.title + "\033[90m")
        spc()
        choice = input("Select a Target : ")
        extchoice = input("Select an extention (php,asp,aspx ..) : ")
        spc()
        print("Cheatsheets that can help you with this :")
        spc()
        print("\033[92m" + installdirc + '/INFO-GATH/CHEATSHEETS/file_enumeration.md \033[92m')
        popp()
        os.system("python3 " + installdirc + tool_dir + "/dirsearch.py" + " -u " + choice + " -e " + extchoice)
        okinf()
    else:
        prilogspc()
        print("\033[92m           " + dirsearch.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/maurosoria/dirsearch.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + dirsearch.title + "\033[90m")
        spc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/DirSearch""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/DirSearch""")
        os.system("echo python3 " + installdirc + tool_dir + "/dirsearch.py >> /usr/local/bin/DirSearch")
        os.system("chmod +x /usr/local/bin/DirSearch")
        print("You can now use " + "\033[91m" + dirsearch.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        okinf()
def brut3k1t():
    tool_dir = "/INFO-GATH/Tools/brut3k1t"
    brut3k1t.title = "brut3k1t : A security-oriented bruteforce framework"
    if os.path.exists('/usr/bin/brut3k1t'):
        prilogspc()
        os.system("git clone https://github.com/ex0dus-0x/brut3k1t.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + brut3k1t.title + "\033[90m")
        spc()
        service = input("Select a service (ssh,smtp,xmpp,facebook,instagram,twitter,md5):")
        user = input("Choose a user : ")
        address = input("Enter an address : ")
        spc()
        print("      \033[92m    Check files on " + installdirc + "/PASSWORD/wordlists/\033[90m")
        spc()
        wordlist = input("Select the wordlist file : ")
        prilogspc()
        os.system("brut3k1t -s"  + service + " -a " + address + " -u " + user + " -w " + wordlist)
        okinf()
    else:
        prilogspc()
        print("\033[92m           " + brut3k1t.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/ex0dus-0x/brut3k1t.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + brut3k1t.title + "\033[90m")
        spc()
        print("\033[91m\033[91mInstalling ...\033[0m\033[0m")
        spc()
        os.system("cd " + installdirc + tool_dir + " && python2 setup.py install" + null)
        os.system("ln -s /usr/bin/brut3k1t /usr/local/bin/brut3k1t")
        prilogspc()
        print(("You can now use " + "\033[91m" + brut3k1t.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okinf()
def gobuster():
    tool_dir = "/INFO-GATH/Tools/gobuster"
    gobuster.title = "gobuster : A DNS and VHost busting tool written in Go "
    if os.path.exists('/usr/local/bin/gobuster'):
        prilogspc()
        os.system("git clone https://github.com/OJ/gobuster.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + gobuster.title + "\033[90m")
        spc()
        choice = input("Select a Target : ")
        spc()
        print("      \033[92m    Check files on " + installdirc + "/PASSWORD/wordlists/\033[90m")
        spc()
        wordlist = input("Select the wordlist file : ")
        prilogspc()
        os.system(installdirc + tool_dir + "/gobuster dir -u " + choice + " -w " + wordlist)
        okinf()
    else:
        prilogspc()
        print("\033[92m           " + gobuster.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/OJ/gobuster.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + gobuster.title + "\033[90m")
        spc()
        print("\033[91mInstalling ...\033[0m")
        spc()
        os.system("cd " + installdirc + tool_dir + " && go get && go build")
        prilogspc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/gobuster""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/gobuster""")
        os.system("echo " + installdirc + tool_dir + "/gobuster >> /usr/local/bin/gobuster")
        os.system("chmod +x /usr/local/bin/gobuster")
        print(("You can now use " + "\033[91m" + gobuster.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okinf()
def enyx():
    enyx.title = "Enyx : An SNMP IPv6 Enumeration Tool"
    tool_dir = "/INFO-GATH/Tools/Enyx"
    if os.path.exists('/usr/local/bin/enyx'):
        prilogspc()
        os.system("git clone https://github.com/trickster0/Enyx.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + enyx.title + "\033[90m")
        spc()
        snmpversion = input("Whats the snmp Version (1 or 2c) : ")
        communitystring = input("Whats the Community string (public or private) : ")
        ipad = input("Whats the IP : ")
        prilogspc()
        os.system("python2 " + installdirc + tool_dir + "/enyx.py " + snmpversion + " " + communitystring + " " + ipad)
        okinf()
    else:
        prilogspc()
        print("\033[92m           " + enyx.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/trickster0/Enyx.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + enyx.title + "\033[90m")
        spc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/enyx""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/enyx""")
        os.system("echo python2 " + installdirc + tool_dir + "/enyx.py >> /usr/local/bin/enyx")
        os.system("chmod +x /usr/local/bin/enyx")
        print(("You can now use " + "\033[91m" + enyx.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okinf()
def Nasnum():
    Nasnum.title = "Nasnum : The NAS Enumerator"
    tool_dir = "/INFO-GATH/Tools/Nasnum"
    if os.path.exists('/usr/local/bin/Nasnum'):
        prilogspc()
        os.system("git clone https://github.com/tcstool/Nasnum.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Nasnum.title + "\033[90m")
        spc()
        smb = input("Want to grab Grab SMB info ? (Y/N) : ")
        nsf = input("Want to grab Grab NFS info ? (Y/N) : ")
        csnmp = input("Want to Check SNMP for connection data ? (Y/N) : ")
        anonftp = input("Want to Check for anonymous FTP and enumerate dirs/files ? (Y/N) : ")
        spc()
        print("\033[92mCheatsheets that can help you with this :")
        spc()
        os.system("find " + installdirc + "/INFO-GATH/CHEATSHEETS/ -type f | grep -E 'nfs.*enum|nfs.*enum'")
        os.system("find " + installdirc + "/INFO-GATH/CHEATSHEETS/ -type f | grep -E 'smb.*enum|smb.*enum'")
        os.system("find " + installdirc + "/INFO-GATH/CHEATSHEETS/ -type f | grep -E 'ftp.*enum|ftp.*enum'")
        print(" \033[90m")
        pop()
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
        prilogspc()
        os.system("Nasnum " + output + target + " " + smb + " " + nsf + " " + csnmp + " " + anonftp)
        okinf()
    else:
        prilogspc()
        print("\033[92m           " + Nasnum.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/tcstool/Nasnum.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Nasnum.title + "\033[90m")
        spc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Nasnum""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Nasnum""")
        os.system("echo sh " + installdirc + tool_dir + "/nasnum.sh >> /usr/local/bin/Nasnum")
        os.system("chmod +x /usr/local/bin/Nasnum")
        print(("You can now use " + "\033[91m" + Nasnum.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okinf()
def Sublist3r():
    Sublist3r.title = "Sublist3r : Fast subdomains enumeration tool for penetration testers"
    tool_dir = "/INFO-GATH/Tools/Sublist3r"
    if os.path.exists('/usr/local/bin/Sublist3r'):
        prilogspc()
        os.system("git clone https://github.com/aboul3la/Sublist3r.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Sublist3r.title + "\033[90m")
        spc()
        domaine = input("Select a Target: ")
        spc()
        print("\033[92mCheatsheets that can help you with this :")
        spc()
        os.system("find " + installdirc + "/PASSWORD/wordlists/ -type f | grep domain")
        os.system("find " + installdirc + "/IMAGES/ -type f | grep tcp")
        os.system("find " + installdirc + "/NETWORKING/ -type f ")
        print("\033[90m")
        pop()
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
        prilogspc()
        os.system("python3 " + installdirc + tool_dir + "/sublist3r.py" + domaine + " " + bruteforce + " " + verbose + ports + engines + threads)
        okinf()
    else:
        prilogspc()
        print("\033[92m           " + Sublist3r.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/aboul3la/Sublist3r.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Sublist3r.title + "\033[90m")
        spc()
        print("\033[91mInstalling ...\033[0m")
        spc()
        os.system("pip install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Sublist3r""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Sublist3r""")
        os.system("echo python3 " + installdirc + tool_dir + "/sublist3r.py >> /usr/local/bin/Sublist3r")
        os.system("chmod +x /usr/local/bin/Sublist3r")
        print(("You can now use " + "\033[91m" + Sublist3r.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okinf()
def wafw00f():
    wafw00f.title = "wafw00f : identify and fingerprint Web Application Firewall "
    tool_dir = "/INFO-GATH/Tools/wafw00f"
    if os.path.exists('/usr/local/bin/wafw00f'):
        prilogspc()
        os.system("git clone https://github.com/EnableSecurity/wafw00f.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + wafw00f.title + "\033[90m")
        spc()
        domaine = input("Select a Target: ")
        prilogspc()
        os.system("wafw00f "  + domaine)
        okinf()
    else:
        prilogspc()
        print("\033[92m           " + wafw00f.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/EnableSecurity/wafw00f.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + wafw00f.title + "\033[90m")
        prilogspc()
        print("\033[91mInstalling ...\033[0m")
        os.system("cd " + installdirc + tool_dir + " && python setup.py install" + null)
        os.system("ln -s /usr/bin/wafw00f /usr/local/bin/wafw00f")
        print(("You can now use " + "\033[91m" + wafw00f.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okinf()
def sherlock():

    sherlock.title = "sherlock : Find usernames across social networks"
    tool_dir = "/INFO-GATH/Tools/sherlock"
    if os.path.exists('/usr/local/bin/sherlock'):
        prilogspc()
        os.system("git clone https://github.com/sherlock-project/sherlock.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + sherlock.title + "\033[90m")
        spc()
        target = input("Enter the target username : ")
        prilogspc()
        os.system("python " + installdirc + tool_dir + "/sherlock.py " + target)
        okinf()
    else:
        prilogspc()
        print("\033[92m           " + sherlock.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/sherlock-project/sherlock.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + sherlock.title + "\033[90m")
        spc()
        print("\033[91mInstalling ...\033[0m.")
        spc()
        os.system("cd " + installdirc + tool_dir + " && pip3 install --upgrade setuptools && pip3 install -r requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/sherlock""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/sherlock""")
        os.system("echo python " + installdirc + tool_dir + "/sherlock.py >> /usr/local/bin/sherlock")
        os.system("chmod +x /usr/local/bin/sherlock")
        print(("You can now use " + "\033[91m" + sherlock.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okinf()
def snmpwn():
    snmpwn.title = "snmpwn : An SNMPv3 User Enumerator and Attack tool"
    tool_dir = "/INFO-GATH/Tools/snmpwn"
    if os.path.exists('/usr/local/bin/snmpwn'):
        prilogspc()
        os.system("git clone https://github.com/hatlord/snmpwn.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + snmpwn.title + "\033[90m")
        spc()
        hosts = input("Enter the targets hosts file : ")
        users = input("List of users you want to try : ")
        passlist = input("Password list for attacks : ")
        enclist = input("Encryption Password List for AuthPriv types : ")
        hosts = "--hosts " + hosts
        users = "--users " + users
        passlist = "--passlist " + passlist
        enclist = "--enclist " + enclist
        prilogspc()
        os.system("snmpwn " + hosts + " " + users + " " + passlist + " " + enclist)
        okinf()
    else:
        prilogspc()
        print("\033[92m           " + snmpwn.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/hatlord/snmpwn.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + snmpwn.title + "\033[90m")
        spc()
        prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        print("\033[91m-[!]-\033[90m    \033[94mTo install this tool you need a non-root user, Please choose one !\033[90m \033[91m-[!] \033[90m")
        user = input("\033[91m-[!]-\033[90m        \033[94mEnter the non-root user username\033[90m  : ")
        spc()
        os.system("su -c " + "'cd " + installdirc + tool_dir + " && gem install bundler && bundle install' " + user + null)
        spc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/snmpwn""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/snmpwn""")
        os.system("echo ruby " + installdirc + tool_dir + "/snmpwn.rb >> /usr/local/bin/snmpwn")
        os.system("chmod +x /usr/local/bin/snmpwn")
        print(("You can now use " + "\033[91m" + snmpwn.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okinf()
def Striker():
    Striker.title = "Striker : an offensive information and vulnerability scanner."
    tool_dir = "/INFO-GATH/Tools/Striker"
    if os.path.exists('/usr/local/bin/Striker'):
        prilogspc()
        os.system("git clone https://github.com/s0md3v/Striker.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Striker.title + "\033[90m")
        spc()
        target = input("Select a Target: ")
        spc()
        print("\033[92mCheatsheets that can help you with this :")
        spc()
        os.system("find " + installdirc + "/INFO-GATH/CHEATSHEETS/ -type f | grep vulnerability")
        print("\033[90m")
        pop()
        prilogspc()
        os.system("python3 " + installdirc + tool_dir + "/striker.py " + target)
        okinf()
    else:
        prilogspc()
        print("\033[92m           " + Striker.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/s0md3v/Striker.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Striker.title + "\033[90m")
        spc()
        prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        spc()
        os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Striker""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Striker""")
        os.system("echo python3 " + installdirc + tool_dir + "/striker.py >> /usr/local/bin/Striker")
        os.system("chmod +x /usr/local/bin/Striker")
        print(("You can now use " + "\033[91m" + Striker.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okinf()
def theHarvester():
    theHarvester.title = "theHarvester : E-mails, subdomains and names Harvester "
    tool_dir = "/INFO-GATH/Tools/theHarvester"
    if os.path.exists('/usr/local/bin/theHarvester'):
        prilogspc()
        os.system("git clone https://github.com/laramies/theHarvester.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + theHarvester.title + "\033[90m")
        spc()
        target = input("Select a Target : ")
        spc()
        print("Cheatsheets that can help you with this :")
        spc()
        print("\033[92m" + installdirc + '/PASSWORD/wordlists/others/names.txt \033[90m')
        spc()
        oktocont()
        target = " -d " + target
        spc()
        print(""" Sources :
            baidu, bing, bingapi, censys, crtsh, dnsdumpster,
            dogpile, duckduckgo, exalead, github-code, google,
            hunter, intelx, linkedin, netcraft, securityTrails,
            threatcrowd, trello, twitter, vhost, virustotal, yahoo
            """)
        spc()
        source = input("Select a Soruce (Ex : bing,google,baidu) : ")
        prilogspc()
        os.system("theHarvester" + target + " -l 500 -b " + source)
        okinf()
    else:
        prilogspc()
        print("\033[92m           " + theHarvester.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/laramies/theHarvester.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + theHarvester.title + "\033[90m")
        spc()
        prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        spc()
        os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("cd " + installdirc + tool_dir + " && python3 " + installdirc + tool_dir + "/setup.py install" + null)
        os.system("ln -s /usr/bin/theHarvester /usr/local/bin/theHarvester")
        print("You can now use " + "\033[91m" + theHarvester.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        okinf()
def URLextractor():
    URLextractor.title = "URLextractor : an  Information gathering & website reconnaissance tool"
    tool_dir = "/INFO-GATH/Tools/URLextractor"
    if os.path.exists('/usr/local/bin/URLextractor'):
        prilogspc()
        os.system("git clone https://github.com/SofianeHamlaoui/URLextractor.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + URLextractor.title + "\033[90m")
        spc()
        target = input("Select a Target : ")
        prilogspc()
        os.system(installdirc + tool_dir + "/extractor.sh "+ target)
        okinf()
    else:
        prilogspc()
        print("\033[92m           " + URLextractor.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/SofianeHamlaoui/URLextractor.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + URLextractor.title + "\033[90m")
        spc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/URLextractor""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/URLextractor""")
        os.system("echo sh " + installdirc + tool_dir + "/extractor.sh >> /usr/local/bin/URLextractor")
        os.system("chmod +x /usr/local/bin/URLextractor")
        print("You can now use " + "\033[91m" + URLextractor.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        okinf()
def denumerator():
    denumerator.title = "denumerator.py : Enumerates list of subdomains"
    tool_dir = "/INFO-GATH/Tools/"
    if os.path.exists('/usr/local/bin/denumerator'):
        prilogspc()
        os.system("wget -O " + installdirc + tool_dir + "denumerator.py https://raw.githubusercontent.com/bl4de/security-tools/master/denumerator/denumerator.py" + null)
        prilogspc()
        print("\033[92m           " + denumerator.title + "\033[90m")
        spc()
        targetlist = input("Enter the Targets list: ")
        prilogspc()
        os.system("denumerator -f " + targetlist + " -t 10")
        okinf()
    else:
        prilogspc()
        print("\033[92m           " + denumerator.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("wget -O " + installdirc + tool_dir + "denumerator.py https://raw.githubusercontent.com/bl4de/security-tools/master/denumerator/denumerator.py" + null)
        prilogspc()
        print("\033[92m           " + denumerator.title + "\033[90m")
        spc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/denumerator""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/denumerator""")
        os.system("echo python2 " + installdirc + tool_dir + "/denumerator.py >> /usr/local/bin/denumerator")
        os.system("chmod +x /usr/local/bin/denumerator")
        print("You can now use " + "\033[91m" + denumerator.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        okinf()
##### INFO-GATHERING : Manual
def nmap():
    nmap.title = "Nmap : the Network Mapper "
    tool_dir = "/INFO-GATH/Tools/nmap"
    if os.path.exists('/usr/bin/nmap'):
        prilogspc()
        print("\033[92m           " + nmap.title + "\033[90m")
        spc()
        target = input("Select a Target : ")
        spc()
        print("Cheatsheets that can help you with this :")
        spc()
        print("\033[92m" + installdirc + '/INFO-GATH/CHEATSHEETS/Nmap-Cheat-Sheet.pdf \033[90m')
        popp()
        os.system("nmap " + target)
        okinf()
    else:
        prilogspc()
        print("\033[92m           " + nmap.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("svn checkout https://svn.nmap.org/nmap/ " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + nmap.title + "\033[90m")
        spc()
        print("\033[91mInstalling ...\033[0m.")
        os.system("cd " + installdirc + tool_dir + " && ./configure && make -j4 && make install && cd " + installdirc + tool_dir + "/ncat && ./configure && make -j4 && make install")
        spc()
        print("You can now use " + "\033[91m" + nmap.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        okinf()
def Photon():
    Photon.title = "Photon : an incredibly fast crawler designed for OSINT."
    tool_dir = "/INFO-GATH/Tools/Photon"
    if os.path.exists('/usr/local/bin/Photon'):
        prilogspc()
        os.system("git clone https://github.com/s0md3v/Photon.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Photon.title + "\033[90m")
        spc()
        os.system("Photon")
        spc()
        print("\033[92mFor more options, Use Photon --help\033[0m")
        okinf()
    else:
        prilogspc()
        print("\033[92m           " + Photon.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/s0md3v/Photon.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Photon.title + "\033[90m")
        prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        spc()
        os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Photon""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Photon""")
        os.system("echo python3 " + installdirc + tool_dir + "/photon.py >> /usr/local/bin/Photon")
        os.system("chmod +x /usr/local/bin/Photon")
        print("You can now use " + "\033[91m" + Photon.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        okinf()
def Raccoon():
    Raccoon.title = "Raccoon : an offensive security tool for reconnaissance and vulnerability scanning."
    tool_dir = "/INFO-GATH/Tools/Raccoon"
    if os.path.exists('/usr/local/bin/Raccoon'):
        prilogspc()
        os.system("git clone https://github.com/evyatarmeged/Raccoon.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Raccoon.title + "\033[90m")
        spc()
        target = input("Select a Target (use 'raccoon --help' for more options) :  ")
        spc()
        print("\033[92mCheatsheets that can help you with this :")
        spc()
        os.system("find " + installdirc + "/INFO-GATH/CHEATSHEETS/ -type f | grep vulnerability")
        print("\033[90m")
        pop()
        os.system("raccoon " + target)
        spc()
        print("\033[92mFor more options, Use raccoon --help\033[0m")
        okinf()
    else:
        prilogspc()
        print("\033[92m           " + Raccoon.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/evyatarmeged/Raccoon.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Raccoon.title + "\033[90m")
        spc()
        printlogo()
        print("\033[91mInstalling ...\033[0m.")
        spc()
        os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("cd " + installdirc + tool_dir + " && python3 setup.py install" + null)
        os.system("ln -s /usr/bin/raccoon /usr/local/bin/Raccoon")
        prilogspc()
        print(("You can now use " + "\033[91m" + Raccoon.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okinf()
def dnsrecon():
    dnsrecon.title = "DnsRecon : A DNS Enumeration Script"
    tool_dir = "/INFO-GATH/Tools/DnsRecon"
    if os.path.exists('/usr/local/bin/dnsrecon'):
        prilogspc()
        os.system("git clone https://github.com/darkoperator/dnsrecon.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + dnsrecon.title + "\033[90m")
        spc()
        target = input("Select a Target (use 'dnsrecon --help' for more options) : ")
        spc()
        print("\033[92mCheatsheets that can help you with this :")
        spc()
        os.system("find  " + installdirc + " -type f | grep dns_")
        print("\033[90m")
        pop()
        os.system("python3 " + installdirc + tool_dir + "/dnsrecon.py -d " + target)
        spc()
        print("\033[92mFor more options, Use dnsrecon --help\033[0m")
        okinf()
    else:
        prilogspc()
        print("\033[92m           " + dnsrecon.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/darkoperator/dnsrecon.git " + installdirc + tool_dir + null)
        clr()
        print("\033[92m           " + dnsrecon.title + "\033[90m")
        spc()
        printlogo()
        print("\033[91mInstalling ...\033[0m.")
        spc()
        os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/dnsrecon""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/dnsrecon""")
        os.system("echo python3 " + installdirc + tool_dir + "/dnsrecon.py >> /usr/local/bin/dnsrecon")
        os.system("chmod +x /usr/local/bin/dnsrecon")
        prilogspc()
        print(("You can now use " + "\033[91m" + dnsrecon.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okinf()
##### INFO-GATHERING : FrameWorks
def ReconDog():
    ReconDog.title = "ReconDog # Reconnaissance Swiss Army Knife"
    tool_dir = "/INFO-GATH/Tools/ReconDog"
    if os.path.exists('/usr/local/bin/ReconDog'):
        prilogspc()
        os.system("git clone https://github.com/s0md3v/ReconDog.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + ReconDog.title + "\033[90m")
        spc()
        os.system("ReconDog")
        okinf()
    else:
        prilogspc()
        print("\033[92m           " + ReconDog.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/s0md3v/ReconDog.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + ReconDog.title + "\033[90m")
        spc()
        print("\033[91mInstalling ...\033[0m.")
        spc()
        os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("chmod +x " + installdirc + tool_dir + "/dog")
        os.system("""echo "#!/bin/bash" > /usr/local/bin/ReconDog""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/ReconDog""")
        os.system("echo " + installdirc + tool_dir + "/dog >> /usr/local/bin/ReconDog")
        os.system("chmod +x /usr/local/bin/ReconDog")
        spc()
        print(("You can now use " + "\033[91m" + ReconDog.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okinf()
def RED_HAWK():
    RED_HAWK.title = "RED_HAWK : All in one tool for Information Gathering, Vulnerability Scanning and Crawling"
    tool_dir = "/INFO-GATH/Tools/RED_HAWK"
    if os.path.exists('/usr/local/bin/RED_HAWK'):
        prilogspc()
        os.system("git clone https://github.com/Tuhinshubhra/RED_HAWK.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + RED_HAWK.title + "\033[90m")
        spc()
        os.system("RED_HAWK")
        okinf()
    else:
        prilogspc()
        print("\033[92m           " + RED_HAWK.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/Tuhinshubhra/RED_HAWK.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + RED_HAWK.title + "\033[90m")
        spc()
        print("\033[91mInstalling ...\033[0m.")
        spc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/RED_HAWK""")
        os.system("echo php " + installdirc + tool_dir + "/rhawk.php >> /usr/local/bin/RED_HAWK")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/RED_HAWK""")
        os.system("chmod +x /usr/local/bin/RED_HAWK")
        spc()
        print(("You can now use " + "\033[91m" + RED_HAWK.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okinf()
def Dracnmap():
    Dracnmap.title = "Dracnmap : Info Gathering Framework"
    tool_dir = "/INFO-GATH/Tools/Dracnmap"
    if os.path.exists('/usr/local/bin/Dracnmap'):
        prilogspc()
        os.system("git clone https://github.com/Screetsec/Dracnmap.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Dracnmap.title + "\033[90m")
        spc()
        os.system("Dracnmap")
        okinf()
    else:
        prilogspc()
        print("\033[92m           " + Dracnmap.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/Screetsec/Dracnmap.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Dracnmap.title + "\033[90m")
        spc()
        print("\033[91mInstalling ...\033[0m.")
        spc()
        os.system("chmod +x " + installdirc + tool_dir + "/dracnmap-v2.2.sh")
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Dracnmap""")
        os.system("echo " + installdirc + tool_dir + "/dracnmap-v2.2.sh >> /usr/local/bin/Dracnmap")
        os.system("chmod +x /usr/local/bin/Dracnmap")
        spc()
        print(("You can now use " + "\033[91m" + Dracnmap.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okinf()
###Menu
def menu():
    clscprilo()
    print("""\033[94m
    [ INFORMATION GATHERING ]

        Make A Choice :\033[90m
    \033[91m -[!]----- Tools ------[!]-\033[90m

        \033[93m-[!]----- Auto ------[!]-

            1)  DirSearch
            2)  brut3k1t
            3)  Gobuster
            4)  Enyx
            5)  Nasnum
            6)  Sublist3r
            7)  Wafw00f
            8)  Sherlock
            9)  Snmpwn
            10) Striker
            11) TheHarvester
            12) URLextractor
            13) Denumerator

        -[!]----- Manual ------[!]-

            14) Photon
            15) Raccoon
            16) dnsrecon
            17) Nmap
        -[!]----- FrameWorks ------[!]-

            18) ReconDog
            19) RED_HAWK
            20) Dracnmap\033[90m

    \033[91m-[!]----- Cheatsheets ------[!]-\033[90m

        \033[93m    21) Information Gathering Cheatsheets\033[90m
    ------------------------
    \033[94mb)    Back to ROOT MENU
    q)    Leave  Lockdoor\033[94m
       """)
    choice = input("\033[92mLockdoor@EnumInfoGathering~# \033[0m")
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
      Nasnum()
    elif choice == "6":
      Sublist3r()
    elif choice == "7":
      wafw00f()
    elif choice == "8":
      sherlock()
    elif choice == "9":
      snmpwn()
    elif choice == "10":
      Striker()
    elif choice == "11":
      theHarvester()
    elif choice == "12":
      URLextractor()
    elif choice == "13":
      denumerator()
    elif choice == "14":
      Photon()
    elif choice == "15":
      Raccoon()
    elif choice == "16":
      dnsrecon()
    elif choice == "17":
      nmap()
    elif choice == "18":
      ReconDog()
    elif choice == "19":
      RED_HAWK()
    elif choice == "20":
      Dracnmap()
    elif choice == "21":
      infgathsh()
    elif choice == "b":
      main.menu()
    elif choice == "q":
        prilogspc()
        now = datetime.now()
        dt_string = now.strftime("%d/%m/%Y %H:%M:%S")
        print("                 \033[91m-[!]- LOCKDOOR IS EXITING -[!]-\033[0m")
        spc()
        print("                 \033[91m-[!]- EXITING AT " + dt_string + " -[!]-\033[0m")
        sys.exit()
    elif choice == "":
      menu()
    else:
      menu()