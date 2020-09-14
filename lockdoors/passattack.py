import os
import sys
from lockdoors import main
from pathlib import Path
from datetime import datetime
from time import sleep
#VAR
config = str(Path.home()) + "/.config/lockdoor/"
yes = set(['yes', 'y', 'ye', 'Y'])
no = set(['no', 'n', 'nop', 'N'])
cwd = os.getcwd()
null = ""
###Cheatsheets
def passsh():
    main.clscprilo()
    print("\033[91mHere is the list of the files :\033[90m")
    print("\033[92m")
    os.system("     find " + main.getinstalldir() + "/PASSWORD/CHEATSHEETS -type f")
    print("\033[90m")
    main.okpa()
#Tools
def crunch():
    crunch.title = "Crunch : a wordlist generator"
    tool_dir = "/PASSWORD/Tools/crunch"
    main.prilogspc()
    os.system("git clone https://github.com/crunchsec/crunch.git " + main.getinstalldir() + tool_dir + null)
    main.clr()
    main.prilogspc()
    print("\033[92m           Crunch Downlaoded successfully \033[90m")
    main.spc()
    print("\033[92m           Installig..... \033[90m")
    main.clr()
    main.spc()
    os.system("cd " + main.getinstalldir() + tool_dir + " && make -j4 && make install")
    main.spc()
    print("\033[92m           Crunch Installed successfully \033[90m")
    main.spc()
    print("\033[92m           use the command 'crunch' on terminal\033[90m")
    print("\033[92m           use 'man crunch to know more about the tool'\033[90m")
    main.okpa()
def Cewl():
    Cewl.title = "Cewl : a Custom Word List Generator."
    tool_dir = "/PASSWORD/Tools/Cewl"
    if os.path.exists('/usr/local/bin/Cewl'):
        main.prilogspc()
        main.prilogspc()
        print("\033[92m           " + Cewl.title + "\033[90m")
        main.spc()
        os.system("Cewl -h")
        main.spc()
        print("Use Cewl from terminal [Cewl -h]")
        main.okpa()
    else:
        main.prilogspc()
        print("\033[92m           " + Cewl.title + "\033[90m")
        main.spc()
        print("\033[91mDownloading ...\033[0m")
        main.spc()
        os.system("git clone https://github.com/digininja/CeWL.git " + main.getinstalldir() + tool_dir + null)
        main.prilogspc()
        print("\033[92m           " + Cewl.title + "\033[90m")
        main.spc()
        main.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        main.spc()
        os.system("cd " + main.getinstalldir() + tool_dir +" && bundle install")
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Cewl""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Cewl""")
        os.system("echo ruby " + main.getinstalldir() + tool_dir + "/cewl.rb >> /usr/local/bin/Cewl")
        os.system("chmod +x /usr/local/bin/Cewl")
        main.spc()
        print(("You can now use " + "\033[91m" + Cewl.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        main.okpa()
def patator():
    patator.title = "Patator : a multi-purpose brute-forcer, with a modular design and a flexible usage"
    tool_dir = "/PASSWORD/Tools/patator"
    if os.path.exists('/usr/local/bin/Patator'):
        main.prilogspc()
        os.system("git clone  https://github.com/lanjelot/patator.git " + main.getinstalldir() + tool_dir + null)
        main.prilogspc()
        print("\033[92m           " + patator.title + "\033[90m")
        main.spc()
        os.system("Patator")
        print("You can always use Patator from Terminal [ \033[91mPatator\033[90m ]")
        main.okpa()
    else:
        main.prilogspc()
        print("\033[92m           " + patator.title + "\033[90m")
        main.spc()
        print("\033[91mDownloading ...\033[0m")
        main.spc()
        os.system("git clone https://github.com/lanjelot/patator.git " + main.getinstalldir() + tool_dir + null)
        main.prilogspc()
        print("\033[92m           " + patator.title + "\033[90m")
        main.spc()
        main.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        os.system("cd " + main.getinstalldir() + tool_dir + " && pip2 install -r requirements.txt" + null)
        os.system("python2 " + main.getinstalldir() + tool_dir + "/setup.py install" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Patator""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Patator""")
        os.system("echo python2 " + main.getinstalldir() + tool_dir + "/patator.py >> /usr/local/bin/Patator")
        os.system("chmod +x /usr/local/bin/Patator")
        main.prilogspc()
        print(("You can now use " + "\033[91m" + patator.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        main.okpa()
def dlrockyou():
    main.printlogo()
    print("\033[91mDownloading ...\033[0m")
    main.spc()
    os.system("wget -O " + main.getinstalldir()+ "/PASSWORD/wordlists/rockyou.gz https://gitlab.com/kalilinux/packages/wordlists/raw/kali/master/rockyou.txt.gz")
    main.spc()
    print("\033[91mExtracting ...\033[0m")
    os.system("gunzip " + main.getinstalldir()+ "/PASSWORD/wordlists/rockyou.gz")
    os.system("rm "+ main.getinstalldir() + "/PASSWORD/wordlists/rockyou.txt.gz")
    main.spc()
    print("Rockyou Password list downloaded successfully , you can use it from here " + main.getinstalldir() + "/PASSWORD/wordlists/")
    main.okpa()
#Menu
def menu():
    main.clscprilo()
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
        main.prilogspc()
        now = datetime.now()
        dt_string = now.strftime("%d/%m/%Y %H:%M:%S")
        print("                 \033[91m-[!]- LOCKDOOR IS EXITING -[!]-\033[0m")
        main.spc()
        print("                 \033[91m-[!]- EXITING AT " + dt_string + " -[!]-\033[0m")
        sys.exit()
    elif choice == "":
      menu()
    else:
      menu()
