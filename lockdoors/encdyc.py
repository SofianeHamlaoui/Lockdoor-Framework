import os
import sys
from lockdoors import main
from lockdoors import sanitize
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
def encsh():
    shrts.clscprilo()
    print("\033[91mHere is the list of the files :\033[90m")
    print("\033[92m")
    os.system("     find " + shrts.getinstalldir() + "/ENCRYPTION/CHEATSHEETS -type f")
    print("\033[90m")
    shrts.okenc()
#Tools
def Codetective():
    Codetective.title = "Codetective : a tool to determine the crypto/encoding algorithm used"
    tool_dir = "/ENCRYPTION/Tools/Codetective"
    if os.path.exists('/usr/local/bin/Codetective'):
        shrts.prilogspc()
        os.system("git clone https://github.com/blackthorne/Codetective.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Codetective.title + "\033[90m")
        shrts.spc()
        os.system("Codetective")
        shrts.okenc()
    else:
        shrts.prilogspc()
        print("\033[92m           " + Codetective.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/blackthorne/Codetective.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + Codetective.title + "\033[90m")
        shrts.spc()
        shrts.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        shrts.spc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Codetective""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Codetective""")
        os.system("echo python2 " + shrts.getinstalldir() + tool_dir + "/codetective.py >> /usr/local/bin/Codetective")
        os.system("chmod +x /usr/local/bin/Codetective")
        print(("You can now use " + "\033[91m" + Codetective.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        print("Type Codetective to run the tool from Terminal")
        shrts.okenc()
def findmyhash():
    findmyhash.title = "findmyhash : Python script to crack hashes using online services"
    tool_dir = "/ENCRYPTION/Tools/Findmyhash"
    if os.path.exists('/usr/local/bin/Findmyhash'):
        shrts.prilogspc()
        os.system("git clone https://github.com/frdmn/findmyhash.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + findmyhash.title + "\033[90m")
        shrts.spc()
        print("""
              MD4       - RFC 1320
              MD5       - RFC 1321
              SHA1      - RFC 3174 (FIPS 180-3)
              SHA224    - RFC 3874 (FIPS 180-3)
              SHA256    - FIPS 180-3
              SHA384    - FIPS 180-3
              SHA512    - FIPS 180-3
              RMD160    - RFC 2857
              GOST      - RFC 5831
              WHIRLPOOL - ISO/IEC 10118-3:2004
              LM        - Microsoft Windows hash
              NTLM      - Microsoft Windows hash
              MYSQL     - MySQL 3, 4, 5 hash
              CISCO7    - Cisco IOS type 7 encrypted passwords
              JUNIPER   - Juniper Networks $9$ encrypted passwords
              LDAP_MD5  - MD5 Base64 encoded
              LDAP_SHA1 - SHA1 Base64 encoded
        """)
        algo = sanitize.bash_escape_restrictor(input("What Algo you want to use ? : "))
        hash = sanitize.bash_escape_restrictor(input("Enter the hash : "))
        os.system("python2 " + shrts.getinstalldir() + tool_dir + "/findmyhash.py " +algo+ " -h " +hash)
        shrts.okenc()
    else:
        shrts.prilogspc()
        print("\033[92m           " + findmyhash.title + "\033[90m")
        shrts.spc()
        print("\033[91mDownloading ...\033[0m")
        shrts.spc()
        os.system("git clone https://github.com/frdmn/findmyhash.git " + shrts.getinstalldir() + tool_dir + null)
        shrts.prilogspc()
        print("\033[92m           " + findmyhash.title + "\033[90m")
        shrts.spc()
        shrts.prilogspc()
        print("\033[91mInstalling ...\033[0m.")
        shrts.spc()
        os.system("""echo "#!/bin/bash" > /usr/local/bin/Findmyhash""")
        os.system("""echo "#Dev : Sofiane Hamlaoui" >> /usr/local/bin/Findmyhash""")
        os.system("echo python2 " + shrts.getinstalldir() + tool_dir + "/findmyhash.py >> /usr/local/bin/Findmyhash")
        os.system("chmod +x /usr/local/bin/Findmyhash")
        print(("You can now use " + "\033[91m" + findmyhash.title + "\033[90m" + " from Lockdoor [\033[92m Lockdoor \033[90m ]" ))
        shrts.okenc()
#Menu
def menu():
    shrts.clscprilo()
    print("""\033[94m
    [ ENCRYPTION/DECRYPTION ]

        Make A Choice :\033[90m
    \033[91m -[!]----- Tools ------[!]-\033[90m

            \033[93m1)  Codetective
            2)  findmyhash\033[90m
    \033[91m-[!]----- Cheatsheets ------[!]-\033[90m

        \033[93m    3) Crypto Cheatsheets\033[90m
    ------------------------
    \033[94mb)    Back to ROOT MENU
    q)    Leave  Lockdoor\033[94m
       """)
    choice = input("\033[92mLockdoor@EncDec~# \033[0m")
    os.system('clear')
    if choice == "1":
      Codetective()
    elif choice == "2":
      findmyhash()
    elif choice == "3":
        encsh()
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
