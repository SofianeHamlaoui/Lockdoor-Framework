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
def okpa():
    spc()
    oktocont()
    menu()
def pop():
    spc()
    oktocont()
    spc()
############
###Cheatsheets
def passsh():
    clscprilo()
    print("\033[91mHere is the list of the files :\033[90m")
    print("\033[92m")
    os.system("     find " + installdirc + "/PASSWORD/CHEATSHEETS -type f")
    print("\033[90m")
    okpa()
#Tools
def crunch():
    crunch.title = "Crunch : a wordlist generator"
    tool_dir = "/PASSWORD/Tools/crunch"
    prilogspc()
    os.system("git clone https://github.com/crunchsec/crunch.git " + installdirc + tool_dir + null)
    clr()
    prilogspc()
    print("\033[92m           Crunch Downlaoded successfully \033[90m")
    spc()
    print("\033[92m           Installig..... \033[90m")
    clr()
    spc()
    os.system("cd " + installdirc + tool_dir + " && make -j4 && make install")
    spc()
    print("\033[92m           Crunch Installed successfully \033[90m")
    spc()
    print("\033[92m           use the command 'crunch' on terminal\033[90m")
    print("\033[92m           use 'man crunch to know more about the tool'\033[90m")
    okpa()
def Cewl():
    Cewl.title = "Cewl : a Custom Word List Generator."
    tool_dir = "/PASSWORD/Tools/Cewl"
    if os.path.exists('/usr/local/bin/Cewl'):
        prilogspc()
        prilogspc()
        print("\033[92m           " + Cewl.title + "\033[90m")
        spc()
        os.system("Cewl -h")
        spc()
        print("Use Cewl from terminal [Cewl -h]")
        okpa()
    else:
        prilogspc()
        print("\033[92m           " + Cewl.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/digininja/CeWL.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Cewl.title + "\033[90m")
        spc()
        prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        spc()
        os.system("cd " + installdirc + tool_dir +" && bundle install")
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Cewl""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Cewl""")
        os.system("echo ruby " + installdirc + tool_dir + "/cewl.rb >> /usr/local/bin/Cewl")
        os.system("chmod +x /usr/local/bin/Cewl")
        spc()
        print(("You can now use " + "\033[91m" + Cewl.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okpa()
def patator():
    patator.title = "Patator : a multi-purpose brute-forcer, with a modular design and a flexible usage"
    tool_dir = "/PASSWORD/Tools/patator"
    if os.path.exists('/usr/local/bin/Patator'):
        prilogspc()
        os.system("git clone  https://github.com/lanjelot/patator.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + patator.title + "\033[90m")
        spc()
        os.system("Patator")
        print("You can always use Patator from Terminal [ \033[91mPatator\033[90m ]")
        okpa()
    else:
        prilogspc()
        print("\033[92m           " + patator.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/lanjelot/patator.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + patator.title + "\033[90m")
        spc()
        prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        os.system("cd " + installdirc + tool_dir + " && pip2 install -r requirements.txt" + null)
        os.system("python2 " + installdirc + tool_dir + "/setup.py install" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Patator""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Patator""")
        os.system("echo python2 " + installdirc + tool_dir + "/patator.py >> /usr/local/bin/Patator")
        os.system("chmod +x /usr/local/bin/Patator")
        prilogspc()
        print(("You can now use " + "\033[91m" + patator.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        okpa()
def dlrockyou():
    printlogo()
    print("\033[91mDownloading ...\033[0m")
    spc()
    os.system("wget -O " + installdirc+ "/PASSWORD/wordlists/rockyou.gz https://gitlab.com/kalilinux/packages/wordlists/raw/kali/master/rockyou.txt.gz")
    spc()
    print("\033[91mExtracting ...\033[0m")
    os.system("gunzip " + installdirc+ "/PASSWORD/wordlists/rockyou.gz")
    os.system("rm "+ installdirc + "/PASSWORD/wordlists/rockyou.txt.gz")
    spc()
    print("Rockyou Password list downloaded successfully , you can use it from here " + installdirc + "/PASSWORD/wordlists/")
    okpa()
#Menu
def menu():
    clscprilo()
    print("""\033[94m
    [ PASSWORD ATTACKS ]

        Make A Choice :\033[90m
    \033[91m -[!]----- Tools ------[!]-\033[90m

            \033[93m1)  Crunch
            2)  CeWl
            3)  Patator
            4)  Download Rockyou.txt\033[90m
    \033[91m-[!]----- Cheatsheets ------[!]-\033[90m

        \033[93m    5) Password Attacks Cheatsheets Cheatsheets\033[90m
    ------------------------
    \033[94mb)    Back to ROOT MENU
    q)    Leave  Lockdoor\033[94m
       """)
    choice = input("\033[92mLockdoor@PassAttacks~# \033[0m")
    os.system('clear')
    if choice == "1":
      crunch()
    elif choice == "2":
      Cewl()
    elif choice == "3":
      patator()
    elif choice == "4":
      dlrockyou()
    elif choice == "5":
        passsh()
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
