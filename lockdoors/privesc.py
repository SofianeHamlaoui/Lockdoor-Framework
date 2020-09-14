import os
import sys
from lockdoors import main
from lockdoors import shrts
from pathlib import Path
from datetime import datetime
from time import sleep
#VAR
yes = set(['yes', 'y', 'ye', 'Y'])
no = set(['no', 'n', 'nop', 'N'])
cwd = os.getcwd()
null = ""
###Cheatsheets
def privesh():
    shrts.clscprilo()
    print("\033[91mHere is the list of the files :\033[90m")
    print("\033[92m")
    os.system("     find " + shrts.getinstalldir() + "/PrivEsc/CHEATSHEETS -type f")
    print("\033[90m")
    shrts.okpr()
#Tools
def priivesc():
    priivesc.title=("A collection of Windows, Linux and MySQL privilege escalation scripts and exploits")
    shrts.printlogo()
    print("\033[92m" + priivesc.title + "\033[90m")
    print()
    print("\033[91mDownloading ...\033[0m")
    shrts.spc()
    os.system("git clone https://github.com/1N3/PrivEsc.git " + shrts.getinstalldir() + "/PrivEsc/Scripts")
    shrts.spc()
    print("\033[91m" + """\033[91mThe collection of Windows, Linux and MySQL privilege
    escalation scripts and exploits is downloaded successfully , you can check
    it here : """ + "\033[90m" + shrts.getinstalldir() + "/PrivEsc/Scripts")
    shrts.okpr()
#Menu
def menu():
    shrts.clscprilo()
    print("""\033[94m
       [ PRIVILEGE ESCALATION ]

         Make A Choice :\033[90m
    \033[91m -[!]----- Tools ------[!]-\033[90m

        \033[93m1)  All you need as Privilege Escalation scripts and exploits\033[90m

    \033[91m-[!]----- Cheatsheets ------[!]-\033[90m

    \033[93m    2) Privilege Escalation Cheatsheets\033[90m
    ------------------------
    \033[94mb)    Back to ROOT MENU
    q)    Leave  Lockdoor\033[94m
       """)
    choice = input("\033[92mLockdoor@PrivEsc~# \033[0m")
    os.system('clear')
    if choice == "1":
      priivesc()
    elif choice == "2":
        privesh()
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
