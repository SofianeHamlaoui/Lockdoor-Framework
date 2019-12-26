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
def okpr():
    spc()
    oktocont()
    menu()
def pop():
    spc()
    oktocont()
    spc()
############
###Cheatsheets
def privesh():
    clscprilo()
    print("\033[91mHere is the list of the files :\033[90m")
    print("\033[92m")
    os.system("     find " + installdirc + "/PrivEsc/CHEATSHEETS -type f")
    print("\033[90m")
    okpr()
#Tools
def priivesc():
    priivesc.title=("A collection of Windows, Linux and MySQL privilege escalation scripts and exploits")
    printlogo()
    print("\033[92m" + priivesc.title + "\033[90m")
    print()
    print("\033[91mDownloading ...\033[0m")
    spc()
    os.system("git clone https://github.com/1N3/PrivEsc.git " + installdirc + "/PrivEsc/Scripts")
    spc()
    print("\033[91m" + """\033[91mThe collection of Windows, Linux and MySQL privilege
    escalation scripts and exploits is downloaded successfully , you can check
    it here : """ + "\033[90m" + installdirc + "/PrivEsc/Scripts")
    okpr()
#Menu
def menu():
    clscprilo()
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
