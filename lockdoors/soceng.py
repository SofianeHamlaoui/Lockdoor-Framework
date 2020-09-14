import os
import sys
from lockdoors import main
from pathlib import Path
from datetime import datetime
from time import sleep
#VAR
yes = set(['yes', 'y', 'ye', 'Y'])
no = set(['no', 'n', 'nop', 'N'])
cwd = os.getcwd()
null = ""
###Cheatsheets
def socsh():
    main.clscprilo()
    print("\033[91mHere is the list of the files :\033[90m")
    print("\033[92m")
    os.system("     find " + main.getinstalldir() + "/SOCIAL_ENGINEERING/CHEATSHEETS -type f")
    print("\033[90m")
    main.okso()
#Tools
def scythe():
    scythe.title = "Cewl : an accounts enumerator"
    tool_dir = "/SOCIAL_ENGINEERING/Tools/scythe"
    main.prilogspc()
    main.prilogspc()
    print("\033[92m           " + scythe.title + "\033[90m")
    main.spc()
    print("\033[92m" + "Change " + main.getinstalldir() + tool_dir + "/accountfile.txt" + """ with
    your targes""" + "\033[90m")
    main.spc()
    os.system("python2 " + main.getinstalldir() + tool_dir + "/scythe.py")
    main.okso()
#Menu
def menu():
    main.clscprilo()
    print("""\033[94m
       [ SOCIAL ENGINEERING ]

         Make A Choice :\033[90m
    \033[91m -[!]----- Tools ------[!]-\033[90m

            \033[93m1)  Scythe\033[90m

    \033[91m-[!]----- Cheatsheets ------[!]-\033[90m

        \033[93m    2) Social Engineering Cheatsheets\033[90m
    ------------------------
    \033[94mb)    Back to ROOT MENU
    q)    Leave  Lockdoor\033[94m
       """)
    choice = input("\033[92mLockdoor@SocEng~# \033[0m")
    os.system('clear')
    if choice == "1":
      scythe()
    elif choice == "2":
        socsh()
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
