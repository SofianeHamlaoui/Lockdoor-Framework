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
############
##Tools
def webshells():
    shrts.printlogo()
    print("\033[91mDownloading ...\033[0m")
    shrts.spc()
    os.system("git clone https://github.com/BlackArch/webshells.git " + shrts.getinstalldir() + "/SHELLS/WebShells")
    shrts.spc()
    print("BlackArch Webshells collection downloaded successfully , you can use them from " + shrts.getinstalldir() + "/SHELLS/WebShells")
    shrts.oksh()
def Shellnum():
    Shellnum.title = "Shellnum : A defense tool thay detect web shells in local directories"
    tool_dir = "/SHELLS/Shellnum"
    if os.path.exists('/usr/local/bin/Shellnum'):
        shrts.prilogspc()
        os.system("git clone https://github.com/ManhNho/shellsum.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Shellnum.title + "\033[90m")
        os.system("python2 " + shrts.getinstalldir() + tool_dir + "/shellsum.py ")
        shrts.oksh()
    else:
        shrts.prilogspc()
        print("\033[92m           " + Shellnum.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/ManhNho/shellsum.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Shellnum.title + "\033[90m")
        shrts.spc()
        shrts.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        shrts.spc()
        os.system("cd " + shrts.getinstalldir() + tool_dir + " && pip2 install -r " + shrts.getinstalldir() + tool_dir + "/requirements.txt" + null)
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Shellnum""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Shellnum""")
        os.system("echo python2 " + shrts.getinstalldir() + tool_dir + "/shellsum.py >> /usr/local/bin/Shellnum")
        os.system("chmod +x /usr/local/bin/Shellnum")
        print(("You can now use " + "\033[91m" + Shellnum.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.oksh()
def Weevely():
        Weevely.title = "Weevely : a multi-purpose brute-forcer, with a modular design and a flexible usage"
        tool_dir = "/SHELLS/Tools/Weevely"
        if os.path.exists('/usr/local/bin/Weevely'):
            shrts.prilogspc()
            os.system("git clone  https://github.com/epinna/weevely3.git " + shrts.getinstalldir() + tool_dir + null)
            shrts.prilogspc()
            print("\033[92m           " + Weevely.title + "\033[90m")
            shrts.spc()
            os.system("Weevely")
            print("You can always use Weevely from Terminal [ \033[91mWeevely\033[90m ]")
            shrts.oksh()
        else:
            shrts.prilogspc()
            print("\033[92m           " + Weevely.title + "\033[90m")
            shrts.spc()
            print("\033[91mDownloading ...\033[0m")
            shrts.spc()
            os.system("git clone https://github.com/epinna/weevely3.git " + shrts.getinstalldir() + tool_dir + null)
            shrts.prilogspc()
            print("\033[92m           " + Weevely.title + "\033[90m")
            shrts.spc()
            shrts.prilogspc()
            print("\033[91mInstalling ...\033[0m.")
            os.system("cd " + shrts.getinstalldir() + tool_dir + " && pip2 install -r requirements.txt" + null)
            os.system("""echo "#!/bin/bash" > /usr/local/bin/Weevely""")
            os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Weevely""")
            os.system("echo python2 " + shrts.getinstalldir() + tool_dir + "/weevely.py >> /usr/local/bin/Weevely")
            os.system("chmod +x /usr/local/bin/Weevely")
            shrts.prilogspc()
            print(("You can now use " + "\033[91m" + Weevely.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
            shrts.oksh()
def ptyshells():
    shrts.printlogo()
    print("\033[91mDownloading ...\033[0m")
    shrts.spc()
    os.system("git clone https://github.com/infodox/python-pty-shells " + shrts.getinstalldir() + "/SHELLS/python-pty-shells")
    shrts.spc()
    print("The Python PTY backdoors collection downloaded successfully , you can use them from " + shrts.getinstalldir() + "/SHELLS/python-pty-shells")
    shrts.oksh()
#Menu
def menu():
    shrts.clscprilo()
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
