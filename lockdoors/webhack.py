import os
from lockdoors import main
from lockdoors import shrts
from pathlib import Path
from lockdoors import sanitize
from datetime import datetime
from time import sleep
from os import path
import sys
#VAR
yes = set(['yes', 'y', 'ye', 'Y'])
no = set(['no', 'n', 'nop', 'N'])
cwd = os.getcwd()
null = ""
###Cheatsheets
def webhsh():
    shrts.clscprilo()
    print("\033[91mHere is the list of the files :\033[90m")
    print("\033[92m")
    os.system("     find " + shrts.getinstalldir() + "/WEB/CHEATSHEETS/ -type f")
    print("\033[90m")
    shrts.okwe()
#Tools
def Spaghetti():
    Spaghetti.title = "Spaghetti - Web Application Security Scanner"
    tool_dir = "/WEB/Tools/Spaghetti"
    if os.path.exists('/usr/local/bin/Spaghetti'):
        shrts.prilogspc()
        os.system("git clone https://github.com/c0ll3cti0n/Spaghetti.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Spaghetti.title + "\033[90m")
        shrts.spc()
        target = input("Select a Target: ")
        shrts.spc()
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
        agent = sanitize.bash_escape_restrictor(input("Use a specified Agent ? (Y/N) : "))
        if agent in yes:
            agent = sanitize.bash_escape_restrictor(input("Specify the Agent ! : "))
            agent = " --agent " + agent
        else:
            agent = ""
        proxy = sanitize.bash_escape_restrictor(input("Set a proxy ? (Y/N) : "))
        if proxy in yes:
            proxy = sanitize.bash_escape_restrictor(input("Set the Proxy (host:port) : "))
            proxy = " --proxy " + proxy
        verbose = input("Verbose output ? (Y/N) : ")
        if verbose in yes:
            verbose = "--verbose"
        else:
            verbose = ""
        shrts.prilogspc()
        os.system("cd  " + shrts.getinstalldir() + tool_dir + " && python2 spaghetti.py -u " + target + " -s " + scoption + crawler + ranagent + agent + proxy + verbose)
        shrts.okwe()
    else:
        shrts.prilogspc()
        print("\033[92m           " + Spaghetti.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/c0ll3cti0n/Spaghetti.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Spaghetti.title + "\033[90m")
        shrts.spc()
        shrts.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        shrts.spc()
        os.system("cd " + shrts.getinstalldir() + tool_dir + " && pip2 install -r " + shrts.getinstalldir() + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Spaghetti""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Spaghetti""")
        os.system("echo python2 " + shrts.getinstalldir() + tool_dir + "/spaghetti.py >> /usr/local/bin/Spaghetti")
        os.system("chmod +x /usr/local/bin/Spaghetti")
        print(("You can now use " + "\033[91m" + Spaghetti.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okwe()
def BruteXSS():
    BruteXSS.title = "BruteXSS :  BruteXSS is a tool to find XSS vulnerabilities in web application "
    tool_dir = "/WEB/Tools/BruteXSS"
    if os.path.exists('/usr/local/bin/BruteXSS'):
        shrts.prilogspc()
        os.system("git clone https://github.com/rajeshmajumdar/BruteXSS " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + BruteXSS.title + "\033[90m")
        shrts.spc()
        print("\033[92mCheatsheets that can help you with this :")
        shrts.spc()
        os.system("find " + shrts.getinstalldir() + "/WEB/CHEATSHEETS/ -type f | grep xss")
        print("\033[90m")
        shrts.spc()
        shrts.oktocont()
        os.system("BruteXSS")
        shrts.okwe()
    else:
        shrts.prilogspc()
        print("\033[92m           " + BruteXSS.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/rajeshmajumdar/BruteXSS " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + BruteXSS.title + "\033[90m")
        shrts.spc()
        shrts.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        shrts.spc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/BruteXSS""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/BruteXSS""")
        os.system("echo python2 " + shrts.getinstalldir() + tool_dir + "/brutexss.py >> /usr/local/bin/BruteXSS")
        os.system("chmod +x /usr/local/bin/BruteXSS")
        print(("You can now use " + "\033[91m" + BruteXSS.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okwe()
def Jdorker():
    Jdorker.title = "J-dorker : a Website List grabber from Bing"
    tool_dir = "/WEB/Tools/Jdorker"
    if os.path.exists('/usr/local/bin/Jdorker'):
        shrts.prilogspc()
        os.system("git clone  https://github.com/04x/J-dorker.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Jdorker.title + "\033[90m")
        shrts.spc()
        os.system("Jdorker")
        shrts.okwe()
    else:
        shrts.prilogspc()
        print("\033[92m           " + Jdorker.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/04x/J-dorker.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Jdorker.title + "\033[90m")
        shrts.spc()
        shrts.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        shrts.spc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Jdorker""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Jdorker""")
        os.system("echo python2 " + shrts.getinstalldir() + tool_dir + "/priv8dorker.py >> /usr/local/bin/Jdorker")
        os.system("chmod +x /usr/local/bin/Jdorker")
        print(("You can now use " + "\033[91m" + Jdorker.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okwe()
##### WEB HACKING : Manual
def whatweb():
    whatweb.title = "whatweb : Next generation web scanner "
    tool_dir= "/WEB/Tools/whatweb"
    if os.path.exists('/usr/local/bin/whatweb'):
        shrts.printlogo()
        os.system("git clone  https://github.com/urbanadventurer/WhatWeb " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        shrts.spc()
        print("\033[92m           " + whatweb.title + "\033[90m")
        shrts.spc()
        target = input("Select a Target : ")
        shrts.prilogspc()
        os.system(shrts.getinstalldir() + tool_dir + "/whatweb " + target)
        shrts.okwe()
    else:
        shrts.prilogspc()
        print("\033[92m           " + whatweb.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/urbanadventurer/WhatWeb " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + whatweb.title + "\033[90m")
        shrts.spc()
        shrts.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        os.system("cd " + shrts.getinstalldir() + tool_dir +" && gem install json && gem install rchardet")
        os.system("""echo "#!/bin/bash" > /usr/local/bin/whatweb""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/whatweb""")
        os.system("echo " + shrts.getinstalldir() + tool_dir + "/whatweb >> /usr/local/bin/whatweb")
        os.system("chmod +x /usr/local/bin/whatweb")
        shrts.spc()
        print(("You can now use " + "\033[91m" + whatweb.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okwe()
def sqlmap():
    sqlmap.title = "SQLmap : Automatic SQL injection and database takeover tool "
    tool_dir= "/WEB/Tools/sqlmap"
    if os.path.exists('/usr/bin/sqlmap'):
        shrts.printlogo()
        os.system("git clone  https://github.com/sqlmapproject/sqlmap " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + sqlmap.title + "\033[90m")
        shrts.spc()
        target = input("Select a Target : ")
        shrts.spc()
        print("Check the sqli cheatsheets to know more")
        shrts.spc()
        os.system("find " + shrts.getinstalldir() + "/WEB/CHEATSHEETS/ -type f | grep sqli")
        shrts.popp()
        os.system("sqlmap -u " + target)
        shrts.okwe()
    else:
        shrts.prilogspc()
        print("\033[92m           " + sqlmap.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/sqlmapproject/sqlmap " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + sqlmap.title + "\033[90m")
        shrts.spc()
        shrts.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        os.system("""echo "#!/bin/bash" > /usr/local/bin/sqlmap""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/sqlmap""")
        os.system("echo python3 " + shrts.getinstalldir() + tool_dir + "/sqlmap.py >> /usr/local/bin/sqlmap")
        os.system("chmod +x /usr/local/bin/sqlmap")
        shrts.spc()
        print(("You can now use " + "\033[91m" + sqlmap.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okwe()
def CMSmap():
    CMSmap.title = "CMSmap : A CMS scanner "
    tool_dir = "/WEB/Tools/CMSmap"
    if os.path.exists('/usr/local/bin/CMSmap'):
        shrts.prilogspc()
        os.system("git clone  https://github.com/04x/J-dorker.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + CMSmap.title + "\033[90m")
        shrts.spc()
        os.system("cmsmap")
        shrts.okwe()
    else:
        shrts.prilogspc()
        print("\033[92m           " + CMSmap.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/Dionach/CMSmap.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + CMSmap.title + "\033[90m")
        shrts.spc()
        shrts.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        os.system("cd " + shrts.getinstalldir() + tool_dir + " && python3 setup.py install >/dev/null")
        os.system("ln -s /usr/bin/cmsmap /usr/local/bin/CMSmap")
        shrts.prilogspc()
        print(("You can now use " + "\033[91m" + CMSmap.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okwe()
def droopescan():
    droopescan.title = "droopescan : scanner , identify , CMSs , Drupal , Silverstripe."
    tool_dir = "/WEB/Tools/droopescan"
    if os.path.exists('/usr/local/bin/droopescan'):
        shrts.prilogspc()
        os.system("git clone  https://github.com/droope/droopescan.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + droopescan.title + "\033[90m")
        shrts.spc()
        os.system("droopescan")
        shrts.okwe()
    else:
        shrts.prilogspc()
        print("\033[92m           " + droopescan.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/droope/droopescan.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + droopescan.title + "\033[90m")
        shrts.spc()
        shrts.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        os.system("cd " + shrts.getinstalldir() + tool_dir + " && pip install -r " + shrts.getinstalldir() + tool_dir + "/requirements.txt" + null)
        os.system("cd " + shrts.getinstalldir() + tool_dir + " && python setup.py install " + null)
        os.system("ln -s /usr/bin/droopescan /usr/local/bin/droopescan")
        shrts.prilogspc()
        print(("You can now use " + "\033[91m" + droopescan.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okwe()
def Optiva():
    Optiva.title = "Optiva-Framework : a  Web Application Scanner"
    tool_dir = "/WEB/Tools/Optiva"
    if os.path.exists('/usr/local/bin/Optiva'):
        shrts.prilogspc()
        os.system("git clone  https://github.com/joker25000/Optiva-Framework " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Optiva.title + "\033[90m")
        shrts.spc()
        os.system("optiva")
        shrts.okwe()
    else:
        shrts.prilogspc()
        print("\033[92m           " + Optiva.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/joker25000/Optiva-Framework " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Optiva.title + "\033[90m")
        shrts.spc()
        shrts.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        os.system("chmod +x " + shrts.getinstalldir() + tool_dir + "/installer.sh")
        os.system("cd " + shrts.getinstalldir() + tool_dir + " && sh installer.sh")
        os.system("ln -s /usr/bin/optiva /usr/local/bin/Optiva")
        shrts.prilogspc()
        print(("You can now use " + "\033[91m" + Optiva.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okwe()
def V3n0M():
    V3n0M.title = "V3n0M : Pentesting scanner in Python3.6 for SQLi/XSS/LFI/RFI and other Vulns "
    tool_dir = "/WEB/Tools/V3n0M"
    if os.path.exists('/usr/local/bin/V3n0M'):
        shrts.prilogspc()
        os.system("git clone  https://github.com/v3n0m-Scanner/V3n0M-Scanner.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + V3n0M.title + "\033[90m")
        shrts.spc()
        os.system("V3n0M")
        shrts.okwe()
    else:
        shrts.prilogspc()
        print("\033[92m           " + V3n0M.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/v3n0m-Scanner/V3n0M-Scanner.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + V3n0M.title + "\033[90m")
        shrts.spc()
        shrts.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        os.system("python3 " + shrts.getinstalldir() + tool_dir + "/setup.py install" + null)
        os.system("ln -s /usr/bin/v3n0m /usr/local/bin/V3n0M")
        shrts.prilogspc()
        print(("You can now use " + "\033[91m" + V3n0M.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okwe()
def Atscan():
    Atscan.title = "Atscan : Advanced dork Search & Mass Exploit Scanner "
    tool_dir = "/WEB/Tools/Atscan"
    if os.path.exists('/usr/local/bin/Atscan'):
        shrts.prilogspc()
        os.system("git clone  https://github.com/AlisamTechnology/ATSCAN.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Atscan.title + "\033[90m")
        shrts.spc()
        os.system("Atscan")
        shrts.okwe()
    else:
        shrts.prilogspc()
        print("\033[92m           " + Atscan.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/AlisamTechnology/ATSCAN.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Atscan.title + "\033[90m")
        shrts.spc()
        shrts.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Atscan""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Atscan""")
        os.system("echo perl " + shrts.getinstalldir() + tool_dir + "/atscan.pl >> /usr/local/bin/Atscan")
        os.system("chmod +x /usr/local/bin/Atscan")
        shrts.spc()
        print("You can now use " + "\033[91m" + Atscan.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        shrts.okwe()
def WPSeku():
    WPSeku.title = "WPSeku : Advanced dork Search & Mass Exploit Scanner "
    tool_dir = "/WEB/Tools/WPSeku"
    if os.path.exists('/usr/local/bin/WPSeku'):
        shrts.prilogspc()
        os.system("git clone  https://github.com/m4ll0k/WPSeku.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + WPSeku.title + "\033[90m")
        shrts.spc()
        os.system("WPSeku")
        shrts.okwe()
    else:
        shrts.prilogspc()
        print("\033[92m           " + WPSeku.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/m4ll0k/WPSeku.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + WPSeku.title + "\033[90m")
        shrts.spc()
        shrts.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        os.system("cd " + shrts.getinstalldir() + tool_dir + " && pip3 install -r " + shrts.getinstalldir() + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/WPSeku""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/WPSeku""")
        os.system("echo python3 " + shrts.getinstalldir() + tool_dir + "/wpseku.py >> /usr/local/bin/WPSeku")
        os.system("chmod +x /usr/local/bin/WPSeku")
        shrts.spc()
        print("You can now use " + "\033[91m" + WPSeku.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        shrts.okwe()
def XSStrike():
    XSStrike.title = "XSStrike : The Most advanced XSS scanner."
    tool_dir = "/WEB/Tools/XSStrike"
    if os.path.exists('/usr/local/bin/XSStrike'):
        shrts.prilogspc()
        os.system("git clone  https://github.com/s0md3v/XSStrike.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + XSStrike.title + "\033[90m")
        shrts.spc()
        shrts.spc()
        print("\033[92mCheatsheets that can help you with this :")
        shrts.spc()
        os.system("find " + shrts.getinstalldir() + "/WEB/CHEATSHEETS/ -type f | grep xss")
        print("\033[90m")
        shrts.spc()
        shrts.oktocont()
        os.system("XSStrike")
        shrts.okwe()
    else:
        shrts.prilogspc()
        print("\033[92m           " + XSStrike.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/s0md3v/XSStrike.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + XSStrike.title + "\033[90m")
        shrts.spc()
        shrts.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        shrts.spc()
        os.system("cd " + shrts.getinstalldir() + tool_dir + " && pip3 install -r " + shrts.getinstalldir() + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/XSStrike""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/XSStrike""")
        os.system("echo python3 " + shrts.getinstalldir() + tool_dir + "/xsstrike.py >> /usr/local/bin/XSStrike")
        os.system("chmod +x /usr/local/bin/XSStrike")
        shrts.spc()
        print("You can now use " + "\033[91m" + XSStrike.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        shrts.okwe()
def Joomscan():
    Joomscan.title = "Joomscan : Advanced dork Search & Mass Exploit Scanner "
    tool_dir = "/WEB/Tools/Joomscan"
    if os.path.exists('/usr/local/bin/Joomscan'):
        shrts.prilogspc()
        os.system("git clone  https://github.com/rezasp/joomscan.git  " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Joomscan.title + "\033[90m")
        shrts.spc()
        os.system("Joomscan")
        shrts.okwe()
    else:
        shrts.prilogspc()
        print("\033[92m           " + Joomscan.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/rezasp/joomscan.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Joomscan.title + "\033[90m")
        shrts.spc()
        shrts.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        shrts.spc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Joomscan""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Joomscan""")
        os.system("echo perl " + shrts.getinstalldir() + tool_dir + "/joomscan.pl >> /usr/local/bin/Joomscan")
        os.system("chmod +x /usr/local/bin/Joomscan")
        shrts.spc()
        print("You can now use " + "\033[91m" + Joomscan.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        shrts.okwe()
def WPscan():
    WPscan.title = "WPscan : WordPress Vulnerability Scanner "
    tool_dir = "/WEB/Tools/WPscan"
    if os.path.exists('/usr/bin/wpscan'):
        print("\033[92m           " + WPscan.title + "\033[90m")
        shrts.spc()
        os.system("wpscan")
        shrts.okwe()
    else:
        shrts.prilogspc()
        print("\033[92m           " + WPscan.title + "\033[90m")
        shrts.spc()
        print("\033[91mInstalling ...\033[0m.")
        shrts.spc()
        user = input("With which user you want to install wpscan ? : ")
        os.system("gem install wpscan")
        os.system("su -c " + "'gem install wpscan' " + user)
        shrts.spc()
        print("You can now use " + "\033[91m" + WPscan.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        shrts.okwe()
##### WEB HACKING : Framework
def Dzjecter():
    Dzjecter.title = "Dzjecter : a Server checking Tool."
    tool_dir = "/WEB/Tools/Dzjecter"
    if os.path.exists('/usr/local/bin/Dzjecter'):
        shrts.prilogspc()
        os.system("git clone https://github.com/joker25000/Dzjecter " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Dzjecter.title + "\033[90m")
        shrts.spc()
        os.system("Dzjecter")
        shrts.okwe()
    else:
        shrts.prilogspc()
        print("\033[92m           " + Dzjecter.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/joker25000/Dzjecter " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Dzjecter.title + "\033[90m")
        shrts.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        shrts.spc()
        os.system("cd " + shrts.getinstalldir() + tool_dir + " && pip install -r " + shrts.getinstalldir() + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Dzjecter""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Dzjecter""")
        os.system("echo python2 " + shrts.getinstalldir() + tool_dir + "/Dzjecter.py >> /usr/local/bin/Dzjecter")
        os.system("chmod +x /usr/local/bin/Dzjecter")
        print("You can now use " + "\033[91m" + Dzjecter.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        shrts.okwe()
###Menu
def menu():
    shrts.clscprilo()
    print("""\033[94m
        [ WEB HACKING  ]

        Make A Choice :\033[90m
    \033[91m -[!]----- Tools ------[!]-\033[90m

        \033[93m-[!]----- Auto ------[!]-

            1)  Spaghetti
            2)  BruteXSS
            3)  Jdorker

        -[!]----- Manual ------[!]-

            4)  CMSmap
            5)  droopescan
            6)  Optiva
            7)  V3n0M
            8)  Atscan
            9)  WPSeku
            10) XSStrike
            11) Joomscan
            12) WPscan
            13) Sqlmap
            14) Whatweb
        -[!]----- FrameWorks ------[!]-

            15) Dzjecter\033[90m

    \033[91m-[!]----- Cheatsheets ------[!]-\033[90m

            \033[93m16) Web Hacking Cheatsheets\033[90m
    ------------------------
    \033[94mb)    Back to ROOT MENU
    q)    Leave  Lockdoor\033[90m
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
      V3n0M()
    elif choice == "8":
      Atscan()
    elif choice == "9":
      WPSeku()
    elif choice == "10":
      XSStrike()
    elif choice == "11":
      Joomscan()
    elif choice == "12":
      WPscan()
    elif choice == "13":
      sqlmap()
    elif choice == "14":
      whatweb()
    elif choice == "15":
      Dzjecter()
    elif choice == "16":
        webhsh()
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
