import os
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
def okwe():
    spc()
    oktocont()
    menu()
def pop():
    spc()
    oktocont()
    spc()
############
###Cheatsheets
def webhsh():
    clscprilo()
    print("\033[91mHere is the list of the files :\033[90m")
    print("\033[92m")
    os.system("     find " + installdirc + "/WEB/CHEATSHEETS/ -type f")
    print("\033[90m")
    okwe()
#Tools
def Spaghetti():
    Spaghetti.title = "Spaghetti - Web Application Security Scanner"
    tool_dir = "/WEB/Tools/Spaghetti"
    if os.path.exists('/usr/local/bin/Spaghetti'):
        prilogspc()
        os.system("git clone https://github.com/c0ll3cti0n/Spaghetti.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Spaghetti.title + "\033[90m")
        spc()
        target = input("Select a Target: ")
        spc()
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
        prilogspc()
        os.system("cd  " + installdirc + tool_dir + " && python2 spaghetti.py -u " + target + " -s " + scoption + crawler + ranagent + agent + proxy + verbose)
        okwe()
    else:
        prilogspc()
        print("\033[92m           " + Spaghetti.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/c0ll3cti0n/Spaghetti.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Spaghetti.title + "\033[90m")
        spc()
        prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        spc()
        os.system("cd " + installdirc + tool_dir + " && pip2 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Spaghetti""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Spaghetti""")
        os.system("echo python2 " + installdirc + tool_dir + "/spaghetti.py >> /usr/local/bin/Spaghetti")
        os.system("chmod +x /usr/local/bin/Spaghetti")
        print(("You can now use " + "\033[91m" + Spaghetti.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okwe()
def BruteXSS():
    BruteXSS.title = "BruteXSS :  BruteXSS is a tool to find XSS vulnerabilities in web application "
    tool_dir = "/WEB/Tools/BruteXSS"
    if os.path.exists('/usr/local/bin/BruteXSS'):
        prilogspc()
        os.system("git clone https://github.com/rajeshmajumdar/BruteXSS " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + BruteXSS.title + "\033[90m")
        spc()
        print("\033[92mCheatsheets that can help you with this :")
        spc()
        os.system("find " + installdirc + "/WEB/CHEATSHEETS/ -type f | grep xss")
        print("\033[90m")
        spc()
        oktocont()
        os.system("BruteXSS")
        okwe()
    else:
        prilogspc()
        print("\033[92m           " + BruteXSS.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/rajeshmajumdar/BruteXSS " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + BruteXSS.title + "\033[90m")
        spc()
        prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        spc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/BruteXSS""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/BruteXSS""")
        os.system("echo python2 " + installdirc + tool_dir + "/brutexss.py >> /usr/local/bin/BruteXSS")
        os.system("chmod +x /usr/local/bin/BruteXSS")
        print(("You can now use " + "\033[91m" + BruteXSS.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okwe()
def Jdorker():
    Jdorker.title = "J-dorker : a Website List grabber from Bing"
    tool_dir = "/WEB/Tools/Jdorker"
    if os.path.exists('/usr/local/bin/Jdorker'):
        prilogspc()
        os.system("git clone  https://github.com/04x/J-dorker.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Jdorker.title + "\033[90m")
        spc()
        os.system("Jdorker")
        okwe()
    else:
        prilogspc()
        print("\033[92m           " + Jdorker.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/04x/J-dorker.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Jdorker.title + "\033[90m")
        spc()
        prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        spc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Jdorker""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Jdorker""")
        os.system("echo python2 " + installdirc + tool_dir + "/priv8dorker.py >> /usr/local/bin/Jdorker")
        os.system("chmod +x /usr/local/bin/Jdorker")
        print(("You can now use " + "\033[91m" + Jdorker.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okwe()
##### WEB HACKING : Manual
def whatweb():
    whatweb.title = "whatweb : Next generation web scanner "
    tool_dir= "/WEB/Tools/whatweb"
    if os.path.exists('/usr/local/bin/whatweb'):
        printlogo()
        os.system("git clone  https://github.com/urbanadventurer/WhatWeb " + installdirc + tool_dir + null)
        prilogspc()
        spc()
        print("\033[92m           " + whatweb.title + "\033[90m")
        spc()
        target = input("Select a Target : ")
        prilogspc()
        os.system(installdirc + tool_dir + "/whatweb " + target)
        okwe()
    else:
        prilogspc()
        print("\033[92m           " + whatweb.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/urbanadventurer/WhatWeb " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + whatweb.title + "\033[90m")
        spc()
        prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        os.system("cd " + installdirc + tool_dir +" && gem install json && gem install rchardet")
        os.system("""echo "#!/bin/bash" > /usr/local/bin/whatweb""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/whatweb""")
        os.system("echo " + installdirc + tool_dir + "/whatweb >> /usr/local/bin/whatweb")
        os.system("chmod +x /usr/local/bin/whatweb")
        spc()
        print(("You can now use " + "\033[91m" + whatweb.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okwe()
def sqlmap():
    sqlmap.title = "SQLmap : Automatic SQL injection and database takeover tool "
    tool_dir= "/WEB/Tools/sqlmap"
    if os.path.exists('/usr/bin/sqlmap'):
        printlogo()
        os.system("git clone  https://github.com/sqlmapproject/sqlmap " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + sqlmap.title + "\033[90m")
        spc()
        target = input("Select a Target : ")
        spc()
        print("Check the sqli cheatsheets to know more")
        spc()
        os.system("find " + installdirc + "/WEB/CHEATSHEETS/ -type f | grep sqli")
        popp()
        os.system("sqlmap -u " + target)
        okwe()
    else:
        prilogspc()
        print("\033[92m           " + sqlmap.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/sqlmapproject/sqlmap " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + sqlmap.title + "\033[90m")
        spc()
        prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        os.system("""echo "#!/bin/bash" > /usr/local/bin/sqlmap""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/sqlmap""")
        os.system("echo python3 " + installdirc + tool_dir + "/sqlmap.py >> /usr/local/bin/sqlmap")
        os.system("chmod +x /usr/local/bin/sqlmap")
        spc()
        print(("You can now use " + "\033[91m" + sqlmap.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okwe()
def CMSmap():
    CMSmap.title = "CMSmap : A CMS scanner "
    tool_dir = "/WEB/Tools/CMSmap"
    if os.path.exists('/usr/local/bin/CMSmap'):
        prilogspc()
        os.system("git clone  https://github.com/04x/J-dorker.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + CMSmap.title + "\033[90m")
        spc()
        os.system("cmsmap")
        okwe()
    else:
        prilogspc()
        print("\033[92m           " + CMSmap.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/Dionach/CMSmap.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + CMSmap.title + "\033[90m")
        spc()
        prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        os.system("cd " + installdirc + tool_dir + " && python3 setup.py install >/dev/null")
        os.system("ln -s /usr/bin/cmsmap /usr/local/bin/CMSmap")
        prilogspc()
        print(("You can now use " + "\033[91m" + CMSmap.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okwe()
def droopescan():
    droopescan.title = "droopescan : scanner , identify , CMSs , Drupal , Silverstripe."
    tool_dir = "/WEB/Tools/droopescan"
    if os.path.exists('/usr/local/bin/droopescan'):
        prilogspc()
        os.system("git clone  https://github.com/droope/droopescan.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + droopescan.title + "\033[90m")
        spc()
        os.system("droopescan")
        okwe()
    else:
        prilogspc()
        print("\033[92m           " + droopescan.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/droope/droopescan.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + droopescan.title + "\033[90m")
        spc()
        prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        os.system("cd " + installdirc + tool_dir + " && pip install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("cd " + installdirc + tool_dir + " && python setup.py install " + null)
        os.system("ln -s /usr/bin/droopescan /usr/local/bin/droopescan")
        prilogspc()
        print(("You can now use " + "\033[91m" + droopescan.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okwe()
def Optiva():
    Optiva.title = "Optiva-Framework : a  Web Application Scanner"
    tool_dir = "/WEB/Tools/Optiva"
    if os.path.exists('/usr/local/bin/Optiva'):
        prilogspc()
        os.system("git clone  https://github.com/joker25000/Optiva-Framework " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Optiva.title + "\033[90m")
        spc()
        os.system("optiva")
        okwe()
    else:
        prilogspc()
        print("\033[92m           " + Optiva.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/joker25000/Optiva-Framework " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Optiva.title + "\033[90m")
        spc()
        prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        os.system("chmod +x " + installdirc + tool_dir + "/installer.sh")
        os.system("cd " + installdirc + tool_dir + " && sh installer.sh")
        os.system("ln -s /usr/bin/optiva /usr/local/bin/Optiva")
        prilogspc()
        print(("You can now use " + "\033[91m" + Optiva.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okwe()
def V3n0M():
    V3n0M.title = "V3n0M : Pentesting scanner in Python3.6 for SQLi/XSS/LFI/RFI and other Vulns "
    tool_dir = "/WEB/Tools/V3n0M"
    if os.path.exists('/usr/local/bin/V3n0M'):
        prilogspc()
        os.system("git clone  https://github.com/v3n0m-Scanner/V3n0M-Scanner.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + V3n0M.title + "\033[90m")
        spc()
        os.system("V3n0M")
        okwe()
    else:
        prilogspc()
        print("\033[92m           " + V3n0M.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/v3n0m-Scanner/V3n0M-Scanner.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + V3n0M.title + "\033[90m")
        spc()
        prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        os.system("python3 " + installdirc + tool_dir + "/setup.py install" + null)
        os.system("ln -s /usr/bin/v3n0m /usr/local/bin/V3n0M")
        prilogspc()
        print(("You can now use " + "\033[91m" + V3n0M.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okwe()
def Atscan():
    Atscan.title = "Atscan : Advanced dork Search & Mass Exploit Scanner "
    tool_dir = "/WEB/Tools/Atscan"
    if os.path.exists('/usr/local/bin/Atscan'):
        prilogspc()
        os.system("git clone  https://github.com/AlisamTechnology/ATSCAN.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Atscan.title + "\033[90m")
        spc()
        os.system("Atscan")
        okwe()
    else:
        prilogspc()
        print("\033[92m           " + Atscan.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/AlisamTechnology/ATSCAN.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Atscan.title + "\033[90m")
        spc()
        prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Atscan""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Atscan""")
        os.system("echo perl " + installdirc + tool_dir + "/atscan.pl >> /usr/local/bin/Atscan")
        os.system("chmod +x /usr/local/bin/Atscan")
        spc()
        print("You can now use " + "\033[91m" + Atscan.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        okwe()
def WPSeku():
    WPSeku.title = "WPSeku : Advanced dork Search & Mass Exploit Scanner "
    tool_dir = "/WEB/Tools/WPSeku"
    if os.path.exists('/usr/local/bin/WPSeku'):
        prilogspc()
        os.system("git clone  https://github.com/m4ll0k/WPSeku.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + WPSeku.title + "\033[90m")
        spc()
        os.system("WPSeku")
        okwe()
    else:
        prilogspc()
        print("\033[92m           " + WPSeku.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/m4ll0k/WPSeku.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + WPSeku.title + "\033[90m")
        spc()
        prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/WPSeku""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/WPSeku""")
        os.system("echo python3 " + installdirc + tool_dir + "/wpseku.py >> /usr/local/bin/WPSeku")
        os.system("chmod +x /usr/local/bin/WPSeku")
        spc()
        print("You can now use " + "\033[91m" + WPSeku.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        okwe()
def XSStrike():
    XSStrike.title = "XSStrike : The Most advanced XSS scanner."
    tool_dir = "/WEB/Tools/XSStrike"
    if os.path.exists('/usr/local/bin/XSStrike'):
        prilogspc()
        os.system("git clone  https://github.com/s0md3v/XSStrike.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + XSStrike.title + "\033[90m")
        spc()
        spc()
        print("\033[92mCheatsheets that can help you with this :")
        spc()
        os.system("find " + installdirc + "/WEB/CHEATSHEETS/ -type f | grep xss")
        print("\033[90m")
        spc()
        oktocont()
        os.system("XSStrike")
        okwe()
    else:
        prilogspc()
        print("\033[92m           " + XSStrike.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/s0md3v/XSStrike.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + XSStrike.title + "\033[90m")
        spc()
        prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        spc()
        os.system("cd " + installdirc + tool_dir + " && pip3 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/XSStrike""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/XSStrike""")
        os.system("echo python3 " + installdirc + tool_dir + "/xsstrike.py >> /usr/local/bin/XSStrike")
        os.system("chmod +x /usr/local/bin/XSStrike")
        spc()
        print("You can now use " + "\033[91m" + XSStrike.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        okwe()
def Joomscan():
    Joomscan.title = "Joomscan : Advanced dork Search & Mass Exploit Scanner "
    tool_dir = "/WEB/Tools/Joomscan"
    if os.path.exists('/usr/local/bin/Joomscan'):
        prilogspc()
        os.system("git clone  https://github.com/rezasp/joomscan.git  " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Joomscan.title + "\033[90m")
        spc()
        os.system("Joomscan")
        okwe()
    else:
        prilogspc()
        print("\033[92m           " + Joomscan.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/rezasp/joomscan.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Joomscan.title + "\033[90m")
        spc()
        prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        spc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Joomscan""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Joomscan""")
        os.system("echo perl " + installdirc + tool_dir + "/joomscan.pl >> /usr/local/bin/Joomscan")
        os.system("chmod +x /usr/local/bin/Joomscan")
        spc()
        print("You can now use " + "\033[91m" + Joomscan.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        okwe()
def WPscan():
    WPscan.title = "WPscan : WordPress Vulnerability Scanner "
    tool_dir = "/WEB/Tools/WPscan"
    if os.path.exists('/usr/bin/wpscan'):
        print("\033[92m           " + WPscan.title + "\033[90m")
        spc()
        os.system("wpscan")
        okwe()
    else:
        prilogspc()
        print("\033[92m           " + WPscan.title + "\033[90m")
        spc()
        print("\033[91mInstalling ...\033[0m.")
        spc()
        user = input("With which user you want to install wpscan ? : ")
        os.system("gem install wpscan")
        os.system("su -c " + "'gem install wpscan' " + user)
        spc()
        print("You can now use " + "\033[91m" + WPscan.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        okwe()
##### WEB HACKING : Framework
def Dzjecter():
    Dzjecter.title = "Dzjecter : a Server checking Tool."
    tool_dir = "/WEB/Tools/Dzjecter"
    if os.path.exists('/usr/local/bin/Dzjecter'):
        prilogspc()
        os.system("git clone https://github.com/joker25000/Dzjecter " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Dzjecter.title + "\033[90m")
        spc()
        os.system("Dzjecter")
        okwe()
    else:
        prilogspc()
        print("\033[92m           " + Dzjecter.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/joker25000/Dzjecter " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Dzjecter.title + "\033[90m")
        prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        spc()
        os.system("cd " + installdirc + tool_dir + " && pip install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Dzjecter""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Dzjecter""")
        os.system("echo python2 " + installdirc + tool_dir + "/Dzjecter.py >> /usr/local/bin/Dzjecter")
        os.system("chmod +x /usr/local/bin/Dzjecter")
        print("You can now use " + "\033[91m" + Dzjecter.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" )
        okwe()
###Menu
def menu():
    clscprilo()
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
        prilogspc()
        now = datetime.now()
        dt_string = now.strftime("%d/%m/%Y %H:%M:%S")
        print("                 \033[91m-[!]- LOCKDOOR IS EXITING -[!]-\033[0m")
        spc()
        print("                 \033[91m-[!]- EXITING AT " + dt_string + " -[!]-\033[0m")
        sys.exit()
    elif choice == "":
      webhack()
    else:
      webhack()