import os
import sys
from lockdoors import main
from lockdoors import sanitize
from lockdoors import shrts
from lockdoors import sanitize
from pathlib import Path
from datetime import datetime
#VAR
yes = set(['yes', 'y', 'ye', 'Y'])
no = set(['no', 'n', 'nop', 'N'])
cwd = os.getcwd()
null = ""
###Cheatsheets
def infgathsh():
    shrts.clscprilo()
    print("\033[91mHere is the list of the files :\033[90m")
    print("\033[92m")
    os.system("     find " + shrts.getinstalldir() + "/INFO-GATH/CHEATSHEETS/ -type f")
    print("\033[90m")
    shrts.okinf()
###TOOLS :
def dirsearch():
    tool_dir = "/INFO-GATH/Tools/dirsearch"
    dirsearch.title = "DirSearch : Web path scanner "
    if os.path.exists('/usr/local/bin/DirSearch'):
        shrts.prilogspc()
        os.system("git clone https://github.com/maurosoria/dirsearch.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + dirsearch.title + "\033[90m")
        shrts.spc()
        choice = sanitize.bash_escape_restrictor(input("Select a Target : "))
        extchoice = sanitize.bash_escape_restrictor(input("Select an extention (php,asp,aspx ..) : "))
        shrts.spc()
        print("Cheatsheets that can help you with this :")
        shrts.spc()
        print("\033[92m" + shrts.getinstalldir() + '/INFO-GATH/CHEATSHEETS/file_enumeration.md \033[92m')
        shrts.popp()
        os.system("python3 " + shrts.getinstalldir() + tool_dir + "/dirsearch.py" + " -u " + choice + " -e " + extchoice)
        shrts.okinf()
    else:
        shrts.prilogspc()
        print("\033[92m           " + dirsearch.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/maurosoria/dirsearch.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + dirsearch.title + "\033[90m")
        shrts.spc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/DirSearch""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/DirSearch""")
        os.system("echo python3 " + shrts.getinstalldir() + tool_dir + "/dirsearch.py >> /usr/local/bin/DirSearch")
        os.system("chmod +x /usr/local/bin/DirSearch")
        print("You can now use " + "\033[91m" + dirsearch.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        shrts.okinf()
def brut3k1t():
    tool_dir = "/INFO-GATH/Tools/brut3k1t"
    brut3k1t.title = "brut3k1t : A security-oriented bruteforce framework"
    if os.path.exists('/usr/bin/brut3k1t'):
        shrts.prilogspc()
        os.system("git clone https://github.com/ex0dus-0x/brut3k1t.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + brut3k1t.title + "\033[90m")
        shrts.spc()
        service = sanitize.bash_escape_restrictor(input("Select a service (ssh,smtp,xmpp,facebook,instagram,twitter,md5):"))
        user = sanitize.bash_escape_restrictor(input("Choose a user : "))
        address = sanitize.bash_escape_restrictor(input("Enter an address : "))
        shrts.spc()
        print("      \033[92m    Check files on " + shrts.getinstalldir() + "/PASSWORD/wordlists/\033[90m")
        shrts.spc()
        wordlist = sanitize.bash_escape_restrictor(input("Select the wordlist file : "))
        shrts.prilogspc()
        os.system("brut3k1t -s"  + service + " -a " + address + " -u " + user + " -w " + wordlist)
        shrts.okinf()
    else:
        shrts.prilogspc()
        print("\033[92m           " + brut3k1t.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/ex0dus-0x/brut3k1t.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + brut3k1t.title + "\033[90m")
        shrts.spc()
        print("\033[91m\033[91mInstalling ...\033[0m\033[0m")
        shrts.spc()
        os.system("cd " + shrts.getinstalldir() + tool_dir + " && python2 setup.py install" + null)
        os.system("ln -s /usr/bin/brut3k1t /usr/local/bin/brut3k1t")
        shrts.prilogspc()
        print(("You can now use " + "\033[91m" + brut3k1t.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okinf()
def gobuster():
    tool_dir = "/INFO-GATH/Tools/gobuster"
    gobuster.title = "gobuster : A DNS and VHost busting tool written in Go "
    if os.path.exists('/usr/local/bin/gobuster'):
        shrts.prilogspc()
        os.system("git clone https://github.com/OJ/gobuster.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + gobuster.title + "\033[90m")
        shrts.spc()
        choice = sanitize.bash_escape_restrictor(input("Select a Target : "))
        shrts.spc()
        print("      \033[92m    Check files on " + shrts.getinstalldir() + "/PASSWORD/wordlists/\033[90m")
        shrts.spc()
        wordlist = sanitize.bash_escape_restrictor(input("Select the wordlist file : "))
        shrts.prilogspc()
        os.system(shrts.getinstalldir() + tool_dir + "/gobuster dir -u " + choice + " -w " + wordlist)
        shrts.okinf()
    else:
        shrts.prilogspc()
        print("\033[92m           " + gobuster.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/OJ/gobuster.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + gobuster.title + "\033[90m")
        shrts.spc()
        print("\033[91mInstalling ...\033[0m")
        shrts.spc()
        os.system("cd " + shrts.getinstalldir() + tool_dir + " && go get && go build")
        shrts.prilogspc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/gobuster""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/gobuster""")
        os.system("echo " + shrts.getinstalldir() + tool_dir + "/gobuster >> /usr/local/bin/gobuster")
        os.system("chmod +x /usr/local/bin/gobuster")
        print(("You can now use " + "\033[91m" + gobuster.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okinf()
def enyx():
    enyx.title = "Enyx : An SNMP IPv6 Enumeration Tool"
    tool_dir = "/INFO-GATH/Tools/Enyx"
    if os.path.exists('/usr/local/bin/enyx'):
        shrts.prilogspc()
        os.system("git clone https://github.com/trickster0/Enyx.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + enyx.title + "\033[90m")
        shrts.spc()
        snmpversion = sanitize.bash_escape_restrictor(input("Whats the snmp Version (1 or 2c) : "))
        communitystring = sanitize.bash_escape_restrictor(input("Whats the Community string (public or private) : "))
        ipad = sanitize.bash_escape_restrictor(input("Whats the IP : "))
        shrts.prilogspc()
        os.system("python2 " + shrts.getinstalldir() + tool_dir + "/enyx.py " + snmpversion + " " + communitystring + " " + ipad)
        shrts.okinf()
    else:
        shrts.prilogspc()
        print("\033[92m           " + enyx.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/trickster0/Enyx.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + enyx.title + "\033[90m")
        shrts.spc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/enyx""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/enyx""")
        os.system("echo python2 " + shrts.getinstalldir() + tool_dir + "/enyx.py >> /usr/local/bin/enyx")
        os.system("chmod +x /usr/local/bin/enyx")
        print(("You can now use " + "\033[91m" + enyx.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okinf()
def Nasnum():
    Nasnum.title = "Nasnum : The NAS Enumerator"
    tool_dir = "/INFO-GATH/Tools/Nasnum"
    if os.path.exists('/usr/local/bin/Nasnum'):
        shrts.prilogspc()
        os.system("git clone https://github.com/tcstool/Nasnum.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Nasnum.title + "\033[90m")
        shrts.spc()
        smb = input("Want to grab Grab SMB info ? (Y/N) : ")
        nsf = input("Want to grab Grab NFS info ? (Y/N) : ")
        csnmp = input("Want to Check SNMP for connection data ? (Y/N) : ")
        anonftp = input("Want to Check for anonymous FTP and enumerate dirs/files ? (Y/N) : ")
        shrts.spc()
        print("\033[92mCheatsheets that can help you with this :")
        shrts.spc()
        os.system("find " + shrts.getinstalldir() + "/INFO-GATH/CHEATSHEETS/ -type f | grep -E 'nfs.*enum|nfs.*enum'")
        os.system("find " + shrts.getinstalldir() + "/INFO-GATH/CHEATSHEETS/ -type f | grep -E 'smb.*enum|smb.*enum'")
        os.system("find " + shrts.getinstalldir() + "/INFO-GATH/CHEATSHEETS/ -type f | grep -E 'ftp.*enum|ftp.*enum'")
        print(" \033[90m")
        shrts.pop()
        target = sanitize.bash_escape_restrictor(input("Select A Target : "))
        output = sanitize.bash_escape_restrictor(input("Select an output file : "))
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
        shrts.prilogspc()
        os.system("Nasnum " + output + target + " " + smb + " " + nsf + " " + csnmp + " " + anonftp)
        shrts.okinf()
    else:
        shrts.prilogspc()
        print("\033[92m           " + Nasnum.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/tcstool/Nasnum.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Nasnum.title + "\033[90m")
        shrts.spc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Nasnum""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Nasnum""")
        os.system("echo sh " + shrts.getinstalldir() + tool_dir + "/nasnum.sh >> /usr/local/bin/Nasnum")
        os.system("chmod +x /usr/local/bin/Nasnum")
        print(("You can now use " + "\033[91m" + Nasnum.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okinf()
def Sublist3r():
    Sublist3r.title = "Sublist3r : Fast subdomains enumeration tool for penetration testers"
    tool_dir = "/INFO-GATH/Tools/Sublist3r"
    if os.path.exists('/usr/local/bin/Sublist3r'):
        shrts.prilogspc()
        os.system("git clone https://github.com/aboul3la/Sublist3r.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Sublist3r.title + "\033[90m")
        shrts.spc()
        domaine = sanitize.bash_escape_restrictor(input("Select a Target: "))
        shrts.spc()
        print("\033[92mCheatsheets that can help you with this :")
        shrts.spc()
        os.system("find " + shrts.getinstalldir() + "/PASSWORD/wordlists/ -type f | grep domain")
        os.system("find " + shrts.getinstalldir() + "/IMAGES/ -type f | grep tcp")
        os.system("find " + shrts.getinstalldir() + "/NETWORKING/ -type f ")
        print("\033[90m")
        shrts.pop()
        bruteforce = input("Enable the subbrute bruteforce module ? (Y/N) : ")
        verbose = input("Enable the verbose mode and display results in realtime ? (Y/N) : ")
        ports = sanitize.bash_escape_restrictor(input("Specify tcp ports for subdomains finding : "))
        threads = sanitize.bash_escape_restrictor(input("Number of threads to use for subbrute bruteforce ?  : "))
        engines = sanitize.bash_escape_restrictor(input("Specify a comma-separated list of search engines : "))
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
        shrts.prilogspc()
        os.system("python3 " + shrts.getinstalldir() + tool_dir + "/sublist3r.py" + domaine + " " + bruteforce + " " + verbose + ports + engines + threads)
        shrts.okinf()
    else:
        shrts.prilogspc()
        print("\033[92m           " + Sublist3r.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/aboul3la/Sublist3r.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Sublist3r.title + "\033[90m")
        shrts.spc()
        print("\033[91mInstalling ...\033[0m")
        shrts.spc()
        os.system("pip3 install -r " + shrts.getinstalldir() + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Sublist3r""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Sublist3r""")
        os.system("echo python3 " + shrts.getinstalldir() + tool_dir + "/sublist3r.py >> /usr/local/bin/Sublist3r")
        os.system("chmod +x /usr/local/bin/Sublist3r")
        print(("You can now use " + "\033[91m" + Sublist3r.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okinf()
def wafw00f():
    wafw00f.title = "wafw00f : identify and fingerprint Web Application Firewall "
    tool_dir = "/INFO-GATH/Tools/wafw00f"
    if os.path.exists('/usr/local/bin/wafw00f'):
        shrts.prilogspc()
        os.system("git clone https://github.com/EnableSecurity/wafw00f.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + wafw00f.title + "\033[90m")
        shrts.spc()
        domaine = sanitize.bash_escape_restrictor(input("Select a Target: "))
        shrts.prilogspc()
        os.system("wafw00f "  + domaine)
        shrts.okinf()
    else:
        shrts.prilogspc()
        print("\033[92m           " + wafw00f.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/EnableSecurity/wafw00f.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + wafw00f.title + "\033[90m")
        shrts.prilogspc()
        print("\033[91mInstalling ...\033[0m")
        os.system("cd " + shrts.getinstalldir() + tool_dir + " && python setup.py install" + null)
        os.system("ln -s /usr/bin/wafw00f /usr/local/bin/wafw00f")
        print(("You can now use " + "\033[91m" + wafw00f.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okinf()
def sherlock():

    sherlock.title = "sherlock : Find usernames across social networks"
    tool_dir = "/INFO-GATH/Tools/sherlock"
    if os.path.exists('/usr/local/bin/sherlock'):
        shrts.prilogspc()
        os.system("git clone https://github.com/sherlock-project/sherlock.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + sherlock.title + "\033[90m")
        shrts.spc()
        target = sanitize.bash_escape_restrictor(input("Enter the target username : "))
        shrts.prilogspc()
        os.system("python " + shrts.getinstalldir() + tool_dir + "/sherlock.py " + target)
        shrts.okinf()
    else:
        shrts.prilogspc()
        print("\033[92m           " + sherlock.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/sherlock-project/sherlock.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + sherlock.title + "\033[90m")
        shrts.spc()
        print("\033[91mInstalling ...\033[0m.")
        shrts.spc()
        os.system("cd " + shrts.getinstalldir() + tool_dir + " && pip3 install --upgrade setuptools && pip3 install -r requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/sherlock""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/sherlock""")
        os.system("echo python " + shrts.getinstalldir() + tool_dir + "/sherlock.py >> /usr/local/bin/sherlock")
        os.system("chmod +x /usr/local/bin/sherlock")
        print(("You can now use " + "\033[91m" + sherlock.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okinf()
def snmpwn():
    snmpwn.title = "snmpwn : An SNMPv3 User Enumerator and Attack tool"
    tool_dir = "/INFO-GATH/Tools/snmpwn"
    if os.path.exists('/usr/local/bin/snmpwn'):
        shrts.prilogspc()
        os.system("git clone https://github.com/hatlord/snmpwn.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + snmpwn.title + "\033[90m")
        shrts.spc()
        hosts = sanitize.bash_escape_restrictor(input("Enter the targets hosts file : "))
        users = sanitize.bash_escape_restrictor(input("List of users you want to try : "))
        passlist = sanitize.bash_escape_restrictor(input("Password list for attacks : "))
        enclist = sanitize.bash_escape_restrictor(input("Encryption Password List for AuthPriv types : "))
        hosts = "--hosts " + hosts
        users = "--users " + users
        passlist = "--passlist " + passlist
        enclist = "--enclist " + enclist
        shrts.prilogspc()
        os.system("snmpwn " + hosts + " " + users + " " + passlist + " " + enclist)
        shrts.okinf()
    else:
        shrts.prilogspc()
        print("\033[92m           " + snmpwn.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/hatlord/snmpwn.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + snmpwn.title + "\033[90m")
        shrts.spc()
        shrts.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        print("\033[91m-[!]-\033[90m    \033[94mTo install this tool you need a non-root user, Please choose one !\033[90m \033[91m-[!] \033[90m")
        user = input("\033[91m-[!]-\033[90m        \033[94mEnter the non-root user username\033[90m  : ")
        shrts.spc()
        os.system("su -c " + "'cd " + shrts.getinstalldir() + tool_dir + " && gem install bundler && bundle install' " + user + null)
        shrts.spc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/snmpwn""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/snmpwn""")
        os.system("echo ruby " + shrts.getinstalldir() + tool_dir + "/snmpwn.rb >> /usr/local/bin/snmpwn")
        os.system("chmod +x /usr/local/bin/snmpwn")
        print(("You can now use " + "\033[91m" + snmpwn.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okinf()
def Striker():
    Striker.title = "Striker : an offensive information and vulnerability scanner."
    tool_dir = "/INFO-GATH/Tools/Striker"
    if os.path.exists('/usr/local/bin/Striker'):
        shrts.prilogspc()
        os.system("git clone https://github.com/s0md3v/Striker.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Striker.title + "\033[90m")
        shrts.spc()
        target = sanitize.bash_escape_restrictor(input("Select a Target: "))
        shrts.spc()
        print("\033[92mCheatsheets that can help you with this :")
        shrts.spc()
        os.system("find " + shrts.getinstalldir() + "/INFO-GATH/CHEATSHEETS/ -type f | grep vulnerability")
        print("\033[90m")
        shrts.pop()
        shrts.prilogspc()
        os.system("python3 " + shrts.getinstalldir() + tool_dir + "/striker.py " + target)
        shrts.okinf()
    else:
        shrts.prilogspc()
        print("\033[92m           " + Striker.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/s0md3v/Striker.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Striker.title + "\033[90m")
        shrts.spc()
        shrts.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        shrts.spc()
        os.system("cd " + shrts.getinstalldir() + tool_dir + " && pip3 install -r " + shrts.getinstalldir() + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Striker""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Striker""")
        os.system("echo python3 " + shrts.getinstalldir() + tool_dir + "/striker.py >> /usr/local/bin/Striker")
        os.system("chmod +x /usr/local/bin/Striker")
        print(("You can now use " + "\033[91m" + Striker.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okinf()
def theHarvester():
    theHarvester.title = "theHarvester : E-mails, subdomains and names Harvester "
    tool_dir = "/INFO-GATH/Tools/theHarvester"
    if os.path.exists('/usr/local/bin/theHarvester'):
        shrts.prilogspc()
        os.system("git clone https://github.com/laramies/theHarvester.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + theHarvester.title + "\033[90m")
        shrts.spc()
        target = sanitize.bash_escape_restrictor(input("Select a Target : "))
        shrts.spc()
        print("Cheatsheets that can help you with this :")
        shrts.spc()
        print("\033[92m" + shrts.getinstalldir() + '/PASSWORD/wordlists/others/names.txt \033[90m')
        shrts.spc()
        shrts.oktocont()
        target = " -d " + target
        shrts.spc()
        print(""" Sources :
            baidu, bing, bingapi, censys, crtsh, dnsdumpster,
            dogpile, duckduckgo, exalead, github-code, google,
            hunter, intelx, linkedin, netcraft, securityTrails,
            threatcrowd, trello, twitter, vhost, virustotal, yahoo
            """)
        shrts.spc()
        source = sanitize.bash_escape_restrictor(input("Select a Soruce (Ex : bing,google,baidu) : "))
        shrts.prilogspc()
        os.system("theHarvester" + target + " -l 500 -b " + source)
        shrts.okinf()
    else:
        shrts.prilogspc()
        print("\033[92m           " + theHarvester.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/laramies/theHarvester.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + theHarvester.title + "\033[90m")
        shrts.spc()
        shrts.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        shrts.spc()
        os.system("cd " + shrts.getinstalldir() + tool_dir + " && pip3 install -r " + shrts.getinstalldir() + tool_dir + "/requirements.txt" + null)
        os.system("cd " + shrts.getinstalldir() + tool_dir + " && python3 " + shrts.getinstalldir() + tool_dir + "/setup.py install" + null)
        os.system("ln -s /usr/bin/theHarvester /usr/local/bin/theHarvester")
        print("You can now use " + "\033[91m" + theHarvester.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        shrts.okinf()
def URLextractor():
    URLextractor.title = "URLextractor : an  Information gathering & website reconnaissance tool"
    tool_dir = "/INFO-GATH/Tools/URLextractor"
    if os.path.exists('/usr/local/bin/URLextractor'):
        shrts.prilogspc()
        os.system("git clone https://github.com/SofianeHamlaoui/URLextractor.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + URLextractor.title + "\033[90m")
        shrts.spc()
        target = sanitize.bash_escape_restrictor(input("Select a Target : "))
        shrts.prilogspc()
        os.system(shrts.getinstalldir() + tool_dir + "/extractor.sh "+ target)
        shrts.okinf()
    else:
        shrts.prilogspc()
        print("\033[92m           " + URLextractor.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/SofianeHamlaoui/URLextractor.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + URLextractor.title + "\033[90m")
        shrts.spc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/URLextractor""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/URLextractor""")
        os.system("echo sh " + shrts.getinstalldir() + tool_dir + "/extractor.sh >> /usr/local/bin/URLextractor")
        os.system("chmod +x /usr/local/bin/URLextractor")
        print("You can now use " + "\033[91m" + URLextractor.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        shrts.okinf()
def denumerator():
    denumerator.title = "denumerator.py : Enumerates list of subdomains"
    tool_dir = "/INFO-GATH/Tools/"
    if os.path.exists('/usr/local/bin/denumerator'):
        shrts.prilogspc()
        os.system("wget -O " + shrts.getinstalldir() + tool_dir + "denumerator.py https://raw.githubusercontent.com/bl4de/security-tools/master/denumerator/denumerator.py" + null)
        shrts.prilogspc()
        print("\033[92m           " + denumerator.title + "\033[90m")
        shrts.spc()
        targetlist = sanitize.bash_escape_restrictor(input("Enter the Targets list: "))
        shrts.prilogspc()
        os.system("denumerator -f " + targetlist + " -t 10")
        shrts.okinf()
    else:
        shrts.prilogspc()
        print("\033[92m           " + denumerator.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("wget -O " + shrts.getinstalldir() + tool_dir + "denumerator.py https://raw.githubusercontent.com/bl4de/security-tools/master/denumerator/denumerator.py" + null)
        shrts.prilogspc()
        print("\033[92m           " + denumerator.title + "\033[90m")
        shrts.spc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/denumerator""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/denumerator""")
        os.system("echo python2 " + shrts.getinstalldir() + tool_dir + "/denumerator.py >> /usr/local/bin/denumerator")
        os.system("chmod +x /usr/local/bin/denumerator")
        print("You can now use " + "\033[91m" + denumerator.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        shrts.okinf()
##### INFO-GATHERING : Manual
def nmap():
    nmap.title = "Nmap : the Network Mapper "
    tool_dir = "/INFO-GATH/Tools/nmap"
    if os.path.exists('/usr/bin/nmap'):
        shrts.prilogspc()
        print("\033[92m           " + nmap.title + "\033[90m")
        shrts.spc()
        target = sanitize.bash_escape_restrictor(input("Select a Target : "))
        shrts.spc()
        print("Cheatsheets that can help you with this :")
        shrts.spc()
        print("\033[92m" + shrts.getinstalldir() + '/INFO-GATH/CHEATSHEETS/Nmap-Cheat-Sheet.pdf \033[90m')
        shrts.popp()
        os.system("nmap " + target)
        shrts.okinf()
    else:
        shrts.prilogspc()
        print("\033[92m           " + nmap.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("svn checkout https://svn.nmap.org/nmap/ " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + nmap.title + "\033[90m")
        shrts.spc()
        print("\033[91mInstalling ...\033[0m.")
        os.system("cd " + shrts.getinstalldir() + tool_dir + " && ./configure && make -j4 && make install && cd " + shrts.getinstalldir() + tool_dir + "/ncat && ./configure && make -j4 && make install")
        shrts.spc()
        print("You can now use " + "\033[91m" + nmap.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        shrts.okinf()
def Photon():
    Photon.title = "Photon : an incredibly fast crawler designed for OSINT."
    tool_dir = "/INFO-GATH/Tools/Photon"
    if os.path.exists('/usr/local/bin/Photon'):
        shrts.prilogspc()
        os.system("git clone https://github.com/s0md3v/Photon.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Photon.title + "\033[90m")
        shrts.spc()
        os.system("Photon")
        shrts.spc()
        print("\033[92mFor more options, Use Photon --help\033[0m")
        shrts.okinf()
    else:
        shrts.prilogspc()
        print("\033[92m           " + Photon.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/s0md3v/Photon.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Photon.title + "\033[90m")
        shrts.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        shrts.spc()
        os.system("cd " + shrts.getinstalldir() + tool_dir + " && pip3 install -r " + shrts.getinstalldir() + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Photon""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Photon""")
        os.system("echo python3 " + shrts.getinstalldir() + tool_dir + "/photon.py >> /usr/local/bin/Photon")
        os.system("chmod +x /usr/local/bin/Photon")
        print("You can now use " + "\033[91m" + Photon.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        shrts.okinf()
def Raccoon():
    Raccoon.title = "Raccoon : an offensive security tool for reconnaissance and vulnerability scanning."
    tool_dir = "/INFO-GATH/Tools/Raccoon"
    if os.path.exists('/usr/local/bin/Raccoon'):
        shrts.prilogspc()
        os.system("git clone https://github.com/evyatarmeged/Raccoon.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Raccoon.title + "\033[90m")
        shrts.spc()
        target = sanitize.bash_escape_restrictor(input("Select a Target (use 'raccoon --help' for more options) : ")
        shrts.spc()
        print("\033[92mCheatsheets that can help you with this :")
        shrts.spc()
        os.system("find " + shrts.getinstalldir() + "/INFO-GATH/CHEATSHEETS/ -type f | grep vulnerability")
        print("\033[90m")
        shrts.pop()
        os.system("raccoon " + target)
        shrts.spc()
        print("\033[92mFor more options, Use raccoon --help\033[0m")
        shrts.okinf()
    else:
        shrts.prilogspc()
        print("\033[92m           " + Raccoon.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/evyatarmeged/Raccoon.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Raccoon.title + "\033[90m")
        shrts.spc()
        shrts.printlogo()
        print("\033[91mInstalling ...\033[0m.")
        shrts.spc()
        os.system("cd " + shrts.getinstalldir() + tool_dir + " && pip3 install -r " + shrts.getinstalldir() + tool_dir + "/requirements.txt" + null)
        os.system("cd " + shrts.getinstalldir() + tool_dir + " && python3 setup.py install" + null)
        os.system("ln -s /usr/bin/raccoon /usr/local/bin/Raccoon")
        shrts.prilogspc()
        print(("You can now use " + "\033[91m" + Raccoon.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okinf()
def dnsrecon():
    dnsrecon.title = "DnsRecon : A DNS Enumeration Script"
    tool_dir = "/INFO-GATH/Tools/DnsRecon"
    if os.path.exists('/usr/local/bin/dnsrecon'):
        shrts.prilogspc()
        os.system("git clone https://github.com/darkoperator/dnsrecon.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + dnsrecon.title + "\033[90m")
        shrts.spc()
        target = sanitize.bash_escape_restrictor(input("Select a Target (use 'dnsrecon --help' for more options) : "))
        shrts.spc()
        print("\033[92mCheatsheets that can help you with this :")
        shrts.spc()
        os.system("find  " + shrts.getinstalldir() + " -type f | grep dns_")
        print("\033[90m")
        shrts.pop()
        os.system("python3 " + shrts.getinstalldir() + tool_dir + "/dnsrecon.py -d " + target)
        shrts.spc()
        print("\033[92mFor more options, Use dnsrecon --help\033[0m")
        shrts.okinf()
    else:
        shrts.prilogspc()
        print("\033[92m           " + dnsrecon.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/darkoperator/dnsrecon.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.clr()
        print("\033[92m           " + dnsrecon.title + "\033[90m")
        shrts.spc()
        shrts.printlogo()
        print("\033[91mInstalling ...\033[0m.")
        shrts.spc()
        os.system("cd " + shrts.getinstalldir() + tool_dir + " && pip3 install -r " + shrts.getinstalldir() + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/dnsrecon""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/dnsrecon""")
        os.system("echo python3 " + shrts.getinstalldir() + tool_dir + "/dnsrecon.py >> /usr/local/bin/dnsrecon")
        os.system("chmod +x /usr/local/bin/dnsrecon")
        shrts.prilogspc()
        print(("You can now use " + "\033[91m" + dnsrecon.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okinf()
##### INFO-GATHERING : FrameWorks
def ReconDog():
    ReconDog.title = "ReconDog # Reconnaissance Swiss Army Knife"
    tool_dir = "/INFO-GATH/Tools/ReconDog"
    if os.path.exists('/usr/local/bin/ReconDog'):
        shrts.prilogspc()
        os.system("git clone https://github.com/s0md3v/ReconDog.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + ReconDog.title + "\033[90m")
        shrts.spc()
        os.system("ReconDog")
        shrts.okinf()
    else:
        shrts.prilogspc()
        print("\033[92m           " + ReconDog.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/s0md3v/ReconDog.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + ReconDog.title + "\033[90m")
        shrts.spc()
        print("\033[91mInstalling ...\033[0m.")
        shrts.spc()
        os.system("cd " + shrts.getinstalldir() + tool_dir + " && pip3 install -r " + shrts.getinstalldir() + tool_dir + "/requirements.txt" + null)
        os.system("chmod +x " + shrts.getinstalldir() + tool_dir + "/dog")
        os.system("""echo "#!/bin/bash" > /usr/local/bin/ReconDog""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/ReconDog""")
        os.system("echo " + shrts.getinstalldir() + tool_dir + "/dog >> /usr/local/bin/ReconDog")
        os.system("chmod +x /usr/local/bin/ReconDog")
        shrts.spc()
        print(("You can now use " + "\033[91m" + ReconDog.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okinf()
def RED_HAWK():
    RED_HAWK.title = "RED_HAWK : All in one tool for Information Gathering, Vulnerability Scanning and Crawling"
    tool_dir = "/INFO-GATH/Tools/RED_HAWK"
    if os.path.exists('/usr/local/bin/RED_HAWK'):
        shrts.prilogspc()
        os.system("git clone https://github.com/Tuhinshubhra/RED_HAWK.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + RED_HAWK.title + "\033[90m")
        shrts.spc()
        os.system("RED_HAWK")
        shrts.okinf()
    else:
        shrts.prilogspc()
        print("\033[92m           " + RED_HAWK.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/Tuhinshubhra/RED_HAWK.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + RED_HAWK.title + "\033[90m")
        shrts.spc()
        print("\033[91mInstalling ...\033[0m.")
        shrts.spc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/RED_HAWK""")
        os.system("echo php " + shrts.getinstalldir() + tool_dir + "/rhawk.php >> /usr/local/bin/RED_HAWK")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/RED_HAWK""")
        os.system("chmod +x /usr/local/bin/RED_HAWK")
        shrts.spc()
        print(("You can now use " + "\033[91m" + RED_HAWK.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okinf()
def Dracnmap():
    Dracnmap.title = "Dracnmap : Info Gathering Framework"
    tool_dir = "/INFO-GATH/Tools/Dracnmap"
    if os.path.exists('/usr/local/bin/Dracnmap'):
        shrts.prilogspc()
        os.system("git clone https://github.com/Screetsec/Dracnmap.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Dracnmap.title + "\033[90m")
        shrts.spc()
        os.system("Dracnmap")
        shrts.okinf()
    else:
        shrts.prilogspc()
        print("\033[92m           " + Dracnmap.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/Screetsec/Dracnmap.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Dracnmap.title + "\033[90m")
        shrts.spc()
        print("\033[91mInstalling ...\033[0m.")
        shrts.spc()
        os.system("chmod +x " + shrts.getinstalldir() + tool_dir + "/dracnmap-v2.2.sh")
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Dracnmap""")
        os.system("echo " + shrts.getinstalldir() + tool_dir + "/dracnmap-v2.2.sh >> /usr/local/bin/Dracnmap")
        os.system("chmod +x /usr/local/bin/Dracnmap")
        shrts.spc()
        print(("You can now use " + "\033[91m" + Dracnmap.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okinf()
###Menu
def menu():
    shrts.clscprilo()
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
        shrts.prilogspc()
        now = datetime.now()
        dt_string = now.strftime("%d/%m/%Y %H:%M:%S")
        print("                 \033[91m-[!]- LOCKDOOR IS EXITING -[!]-\033[0m")
        shrts.spc()
        print("                 \033[91m-[!]- EXITING AT " + dt_string + " -[!]-\033[0m")
        sys.exit()
    elif choice == "":
      menu()
    else:
      menu()
