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
def revsh():
    main.clscprilo()
    print("\033[91mHere is the list of the files :\033[90m")
    print("\033[92m")
    os.system("     find " + main.getinstalldir() + "/REVERSE/CHEATSHEETS/ -type f")
    print("\033[90m")
    main.okrev()
#Tools
def radar2():
    radar2.title = "Radar 2 : unix-like reverse engineering framework"
    tool_dir = "/REVERSE/Tools/radar2"
    main.prilogspc()
    os.system("git clone https://github.com/radare/radare2.git " + main.getinstalldir() + tool_dir + null)
    main.clscprilo()
    print("\033[92m           Radar2 Downlaoded successfully \033[90m")
    main.spc()
    print("\033[92m           Check " + main.getinstalldir() + tool_dir +" Folder\033[90m")
    main.okrev()
def virustotal():
    virustotal.title = "VirusTotal tools"
    tool_dir = "/REVERSE/Tools/virustotal"
    if os.path.exists('/usr/local/bin/virustotal'):
        main.prilogspc()
        os.system("git clone https://github.com/botherder/virustotal.git " + main.getinstalldir() + tool_dir + null)
        main.prilogspc()
        print("\033[92m           " + virustotal.title + "\033[90m")
        main.spc()
        key = input("\033[92mEnter the Virtustoal Api  ? : \033[90m")
        outp = input("\033[92mEnter directory containing files to scan ? : \033[90m")
        os.system("python2 " + main.getinstalldir() + tool_dir + "/vt.py --key "+key+" " +outp)
        main.okrev()
    else:
        main.prilogspc()
        print("\033[92m           " + virustotal.title + "\033[90m")
        main.spc()
        print("\033[91mDownloading ...\033[0m")
        main.spc()
        os.system("git clone  https://github.com/botherder/virustotal.git  " + main.getinstalldir() + tool_dir + null)
        main.prilogspc()
        print("\033[92m           " + virustotal.title + "\033[90m")
        main.spc()
        main.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        main.spc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/virustotal""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/virustotal""")
        os.system("echo python2 " + main.getinstalldir() + tool_dir + "/vt.py >> /usr/local/bin/virustotal")
        os.system("chmod +x /usr/local/bin/virustotal")
        print(("You can now use " + "\033[91m" + virustotal.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        main.okrev()
def miasm():
    miasm.title = "miasm : Reverse engineering framework"
    tool_dir = "/REVERSE/Tools/miasm"
    main.prilogspc()
    os.system("git clone https://github.com/cea-sec/miasm.git " + main.getinstalldir() + tool_dir + null)
    main.prilogspc()
    os.system("cd " +main.getinstalldir() + tool_dir + " && python2 setup.py build")
    os.system("cd " +main.getinstalldir() + tool_dir + " && python2 setup.py install")
    main.spc()
    print("\033[92m           Miasm Downlaoded successfully \033[90m")
    main.spc()
    print("\033[92m           Check " + main.getinstalldir() + tool_dir +" Folder\033[90m")
    main.okrev()
def mirror():
    mirror.title = "mirror : reverses the bytes of a file"
    tool_dir = "/REVERSE/Tools/mirror"
    main.prilogspc()
    os.system("git clone https://github.com/guelfoweb/mirror.git " + main.getinstalldir() + tool_dir + null)
    main.clr()
    main.prilogspc()
    print("\033[92m           Mirror Downlaoded successfully \033[90m")
    main.spc()
    print("\033[92m           Check " + main.getinstalldir() + tool_dir +" Folder\033[90m")
    main.okrev()
def Dnspy():
    Dnspy.title = "Dnspy : reverses the bytes of a file"
    tool_dir = "/REVERSE/Tools/Dnspy"
    main.prilogspc()
    os.system("git clone https://github.com/0xd4d/dnSpy.git " + main.getinstalldir() + tool_dir + null)
    main.clr()
    main.prilogspc()
    print("\033[92m           Dnspy Downlaoded successfully \033[90m")
    main.spc()
    print("\033[92m           Check " + main.getinstalldir() + tool_dir +" Folder\033[90m")
    main.okrev()
def angrio():
    angrio.title = "angrio : a python framework for analyzing binaries"
    tool_dir = "/REVERSE/Tools/angrio"
    main.prilogspc()
    print("\033[92m           Installing \033[90m")
    main.spc()
    os.system("pip install angr ")
    main.clr()
    main.prilogspc()
    print("\033[92m           Dnspy Downlaoded successfully \033[90m")
    main.spc()
    print("\033[92m           Check Angr.io docs to learn more about the tool \033[90m")
    print("\033[92m           https://github.com/angr/angr-doc \033[90m")
    main.okrev()
def dllrunner():
    dllrunner.title = "Dllrunner : a smart DLL execution script for malware analysis"
    tool_dir = "/REVERSE/Tools/dllrunner"
    main.prilogspc()
    os.system("git clone https://github.com/Neo23x0/DLLRunner " + main.getinstalldir() + tool_dir + null)
    main.clr()
    main.prilogspc()
    print("\033[92m           Dllrunner Downlaoded successfully \033[90m")
    main.spc()
    print("\033[92m           Check "+ main.getinstalldir() + tool_dir + " Folder\033[90m")
    main.okrev()
def yara():
    yara.title = "YARA :  a tool to identify and classify malwares "
    tool_dir = "/REVERSE/Tools/yara"
    main.prilogspc()
    print("\033[92m           Installing \033[90m")
    main.spc()
    os.system("pip install yara-python")
    main.clr()
    main.prilogspc()
    print("\033[92m           YARA Downlaoded successfully \033[90m")
    main.spc()
    print("\033[92m           Check YARA Docs to learn more about the tool\033[90m")
    print("\033[92m           https://yara.readthedocs.io/en/latest/\033[90m")
    main.okrev()
#Menu
def menu():
    main.clscprilo()
    print("""\033[94m
    [ REVERSE ENGINEERING  ]

        Make A Choice :\033[90m
    \033[91m -[!]----- Tools ------[!]-\033[90m

            \033[93m1)  Radar2
            2)  Virustotal
            3)  Miasm
            4)  Mirror
            5)  Dnspy
            6)  Angrio
            7)  DLLRunner
            8)  Yara\033[90m
    \033[91m-[!]----- Cheatsheets ------[!]-\033[90m

        \033[93m    9) Reverse Engineering Cheatsheets\033[90m
    ------------------------
    \033[94mb)    Back to ROOT MENU
    q)    Leave  Lockdoor\033[94m
       """)
    choice = input("\033[92mLockdoor@ReverseEngineering~# \033[0m")
    os.system('clear')
    if choice == "1":
      radar2()
    elif choice == "2":
      virustotal()
    elif choice == "3":
      miasm()
    elif choice == "4":
      mirror()
    elif choice == "5":
      Dnspy()
    elif choice == "6":
      angrio()
    elif choice == "7":
      dllrunner()
    elif choice == "8":
      yara()
    elif choice == "9":
      revsh()
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
