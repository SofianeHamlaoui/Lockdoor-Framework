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
def clscprilo():
    clr()
    printlogo()
############
###Cheatsheets
def wtpp():
    clscprilo()
    print("\033[91mHere is the list of the files :\033[90m")
    print("\033[92m")
    os.system("     find " + installdirc + "/WALK -type f")
    print("\033[90m")
    spc()
    oktocont()
    main.menu()