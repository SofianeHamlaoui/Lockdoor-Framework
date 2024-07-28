import os
import sys
from pathlib import Path
from lockdoors import infogathering
from lockdoors import webhack
from lockdoors import exploitation
from lockdoors import reverse
from lockdoors import encdyc
from lockdoors import passattack
from lockdoors import shells
from lockdoors import privesc
from lockdoors import soceng
from lockdoors import psafrt
from lockdoors import wtpp
from lockdoors import about
from lockdoors import update
# VARS
config = str(Path.home()) + "/.config/lockdoor/"
# Functions
def getinstalldir():
    f = open(config + 'lockdoor.conf')
    contents = f.read().rstrip('\n')
    f.close()
    installdirc = contents.replace('Location:', '')
    return installdirc

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
\033[94m ';;..,;;;;;,..,,..';;;;;,..,;' \033[0m\033[92m           © Sofiane Hamlaoui | 2024       \033[0m
\033[94m.';;..,;;;;,. .... .,;;;;,..;;,.\033[0m\033[92m Lockdoor : A Penetration Testing framework\033[0m
\033[94m ';;..,;;;;'  ....  .;;;;,..;;,. \033[0m\033[92m                 v2.3             \033[0m
\033[94m .,;'.';;;;'.  ..  .';;;;,.';,.  \033[0m
\033[94m   ....;;;;;,'''''',;;;;;'...    \033[0m
\033[94m       ..................\033[0m""")

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
def okenc():
    spc()
    oktocont()
    encdyc.menu()
def pop():
    spc()
    oktocont()
    spc()
def okex():
    spc()
    oktocont()
    exploitation.menu()
def okinf():
    spc()
    oktocont()
    infogathering.menu()
def okpa():
    spc()
    oktocont()
    passattack.menu()
def okpr():
    spc()
    oktocont()
    privesc.menu()
def okrev():
    spc()
    oktocont()
    reverse.menu()
def oksh():
    spc()
    oktocont()
    shells.menu()
def okso():
    spc()
    oktocont()
    soceng.menu()
def okwe():
    spc()
    oktocont()
    webhack.menu()