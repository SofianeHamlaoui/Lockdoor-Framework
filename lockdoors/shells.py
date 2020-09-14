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
############
##Tools
def webshells():
    main.printlogo()
    print("\033[91mDownloading ...\033[0m")
    main.spc()
    os.system("git clone https://github.com/BlackArch/webshells.git " + main.getinstalldir() + "/SHELLS/WebShells")
    main.spc()
    print("BlackArch Webshells collection downloaded successfully , you can use them from " + main.getinstalldir() + "/SHELLS/WebShells")
    main.oksh()
def Shellnum():
    Shellnum.title = "Shellnum : A defense tool thay detect web shells in local directories"
    tool_dir = "/SHELLS/Shellnum"
    if os.path.exists('/usr/local/bin/Shellnum'):
        main.prilogspc()
        os.system("git clone https://github.com/ManhNho/shellsum.git " + main.getinstalldir() + tool_dir + null)
        main.prilogspc()
        print("\033[92m           " + Shellnum.title + "\033[90m")
        os.system("python2 " + main.getinstalldir() + tool_dir + "/shellsum.py ")
        main.oksh()
    else:
        main.prilogspc()
        print("\033[92m           " + Shellnum.title + "\033[90m")
        main.spc()
        print("\033[91mDownloading ...\033[0m")
        main.spc()
        os.system("git clone https://github.com/ManhNho/shellsum.git " + main.getinstalldir() + tool_dir + null)
        main.prilogspc()
        print("\033[92m           " + Shellnum.title + "\033[90m")
        main.spc()
        main.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        main.spc()
        os.system("cd " + main.getinstalldir() + tool_dir + " && pip2 install -r " + main.getinstalldir() + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Shellnum""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Shellnum""")
        os.system("echo python2 " + main.getinstalldir() + tool_dir + "/shellsum.py >> /usr/local/bin/Shellnum")
        os.system("chmod +x /usr/local/bin/Shellnum")
        print(("You can now use " + "\033[91m" + Shellnum.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        main.oksh()
def Weevely():
        Weevely.title = "Weevely : a multi-purpose brute-forcer, with a modular design and a flexible usage"
        tool_dir = "/SHELLS/Tools/Weevely"
        if os.path.exists('/usr/local/bin/Weevely'):
            main.prilogspc()
            os.system("git clone  https://github.com/epinna/weevely3.git " + main.getinstalldir() + tool_dir + null)
            main.prilogspc()
            print("\033[92m           " + Weevely.title + "\033[90m")
            main.spc()
            os.system("Weevely")
            print("You can always use Weevely from Terminal [ \033[91mWeevely\033[90m ]")
            main.oksh()
        else:
            main.prilogspc()
            print("\033[92m           " + Weevely.title + "\033[90m")
            main.spc()
            print("\033[91mDownloading ...\033[0m")
            main.spc()
            os.system("git clone https://github.com/epinna/weevely3.git " + main.getinstalldir() + tool_dir + null)
            main.prilogspc()
            print("\033[92m           " + Weevely.title + "\033[90m")
            main.spc()
            main.prilogspc()
            print("\033[91mInstalling ...\033[0m.")
            os.system("cd " + main.getinstalldir() + tool_dir + " && pip2 install -r requirements.txt" + null)
            os.system("""echo "#!/bin/bash" > /usr/local/bin/Weevely""")
            os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Weevely""")
            os.system("echo python2 " + main.getinstalldir() + tool_dir + "/weevely.py >> /usr/local/bin/Weevely")
            os.system("chmod +x /usr/local/bin/Weevely")
            main.prilogspc()
            print(("You can now use " + "\033[91m" + Weevely.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
            main.oksh()
def ptyshells():
    main.printlogo()
    print("\033[91mDownloading ...\033[0m")
    main.spc()
    os.system("git clone https://github.com/infodox/python-pty-shells " + main.getinstalldir() + "/SHELLS/python-pty-shells")
    main.spc()
    print("The Python PTY backdoors collection downloaded successfully , you can use them from " + main.getinstalldir() + "/SHELLS/python-pty-shells")
    main.oksh()
#Menu
def menu():
    main.clscprilo()
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
