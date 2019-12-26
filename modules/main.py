import os
import sys
import infogathering
import webhack
import exploitation
import reverse
import encdyc
import passattack
import shells
import privesc
import soceng
import psafrt
import wtpp
import about
import update
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

def menu():
    os.system('clear')
    printlogo()
    print("""
    \033[94m         [ R00T MENU ]

            Make A Choice :\033[0m

        \033[93m1)  Information Gathering
        2)  Web Hacking
        3)  Exploitation
        4)  Reverse Engineering
        5)  Encryption/Decryption
        6)  Password Attacks
        7)  Shells
        8)  Privilege Escalation
        9)  Social Engineering
        10) Pentesting & Security Assessment Findings Report Templates
        11) Help with Walk Throughs & Pentest Processing\033[0m
        ------------------------
        \033[94ma)    About  Lockdoor
        u)    Update Lockdoor
        q)    Leave  Lockdoor\033[90m
        """)
    choice = input("\033[92mLockdoor~# \033[0m")
    os.system('clear')
    if choice == "1":
        infogathering.menu()
    elif choice == "2":
        webhack.menu()
    elif choice == "3":
        exploitation.menu()
    elif choice == "4":
        reverse.menu()
    elif choice == "5":
        encdyc.menu()
    elif choice == "6":
        passattack.menu()
    elif choice == "7":
        shells.menu()
    elif choice == "8":
        privesc.menu()
    elif choice == "9":
        soceng.menu()
    elif choice == "10":
        psafrt.psafrt()
    elif choice == "11":
        wtpp.wtpp()
    elif choice == "a":
        about.show()
    elif choice == "u":
        update.lockdoor()
    elif choice == "q":
        printlogo()
        print("")
        now = datetime.now()
        date = now.strftime("%d/%m/%Y %H:%M:%S")
        print("                 \033[91m-[!]- LOCKDOOR IS EXITING -[!]-\033[0m")
        print("")
        print("                 \033[91m-[!]- EXITING AT " + date + " -[!]-\033[0m")
        sys.exit()
    elif choice == "":
        menu()
    else:
        menu()