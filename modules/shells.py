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
def oksh():
    spc()
    oktocont()
    menu()
def pop():
    spc()
    oktocont()
    spc()
############
##Tools
def webshells():
    printlogo()
    print("\033[91mDownloading ...\033[0m")
    spc()
    os.system("git clone https://github.com/BlackArch/webshells.git " + installdirc + "/SHELLS/WebShells")
    spc()
    print("BlackArch Webshells collection downloaded successfully , you can use them from " + installdirc + "/SHELLS/WebShells")
    oksh()
def Shellnum():
    Shellnum.title = "Shellnum : A defense tool thay detect web shells in local directories"
    tool_dir = "/SHELLS/Shellnum"
    if os.path.exists('/usr/local/bin/Shellnum'):
        prilogspc()
        os.system("git clone https://github.com/ManhNho/shellsum.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Shellnum.title + "\033[90m")
        os.system("python2 " + installdirc + tool_dir + "/shellsum.py ")
        oksh()
    else:
        prilogspc()
        print("\033[92m           " + Shellnum.title + "\033[90m")
        spc()
        print("\033[91mDownloading ...\033[0m")
        spc()
        os.system("git clone https://github.com/ManhNho/shellsum.git " + installdirc + tool_dir + null)
        prilogspc()
        print("\033[92m           " + Shellnum.title + "\033[90m")
        spc()
        prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        spc()
        os.system("cd " + installdirc + tool_dir + " && pip2 install -r " + installdirc + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Shellnum""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Shellnum""")
        os.system("echo python2 " + installdirc + tool_dir + "/shellsum.py >> /usr/local/bin/Shellnum")
        os.system("chmod +x /usr/local/bin/Shellnum")
        print(("You can now use " + "\033[91m" + Shellnum.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        oksh()
def Weevely():
        Weevely.title = "Weevely : a multi-purpose brute-forcer, with a modular design and a flexible usage"
        tool_dir = "/SHELLS/Tools/Weevely"
        if os.path.exists('/usr/local/bin/Weevely'):
            prilogspc()
            os.system("git clone  https://github.com/epinna/weevely3.git " + installdirc + tool_dir + null)
            prilogspc()
            print("\033[92m           " + Weevely.title + "\033[90m")
            spc()
            os.system("Weevely")
            print("You can always use Weevely from Terminal [ \033[91mWeevely\033[90m ]")
            oksh()
        else:
            prilogspc()
            print("\033[92m           " + Weevely.title + "\033[90m")
            spc()
            print("\033[91mDownloading ...\033[0m")
            spc()
            os.system("git clone https://github.com/epinna/weevely3.git " + installdirc + tool_dir + null)
            prilogspc()
            print("\033[92m           " + Weevely.title + "\033[90m")
            spc()
            prilogspc()
            print("\033[91mInstalling ...\033[0m.")
            os.system("cd " + installdirc + tool_dir + " && pip2 install -r requirements.txt" + null)
            os.system("""echo "#!/bin/bash" > /usr/local/bin/Weevely""")
            os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Weevely""")
            os.system("echo python2 " + installdirc + tool_dir + "/weevely.py >> /usr/local/bin/Weevely")
            os.system("chmod +x /usr/local/bin/Weevely")
            prilogspc()
            print(("You can now use " + "\033[91m" + Weevely.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
            oksh()
def ptyshells():
    printlogo()
    print("\033[91mDownloading ...\033[0m")
    spc()
    os.system("git clone https://github.com/infodox/python-pty-shells " + installdirc + "/SHELLS/python-pty-shells")
    spc()
    print("The Python PTY backdoors collection downloaded successfully , you can use them from " + installdirc + "/SHELLS/python-pty-shells")
    oksh()
#Menu
def menu():
    clscprilo()
    print("""\033[94m
           [ SHELLS ]

         Make A Choice :\033[90m
    \033[91m -[!]----- Tools ------[!]-\033[90m

            \033[93m1)  BlackArch's Webshells Collection
            2)  Shellnum
            3)  Weevely
            4)  Python PTY Shells\033[90m
    ------------------------
    \033[94mb)    Back to ROOT MENU
    q)    Leave  Lockdoor\033[94m
       """)
    choice = input("\033[92mLockdoor@Shells~# \033[0m")
    os.system('clear')
    if choice == "1":
      webshells()
    elif choice == "2":
      Shellnum()
    elif choice == "3":
      Weevely()
    elif choice == "4":
      ptyshells()
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
