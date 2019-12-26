import os
from pathlib import Path
import main
yes = set(['yes', 'y', 'ye', 'Y'])
no = set(['no', 'n', 'nop', 'N'])
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
\033[94m       ..................\033[0m
""")
config = str(Path.home()) + "/.config/lockdoor/"
f = open(config + 'lockdoor.conf')
contents = f.read().rstrip('\n')
f.close()
installdirc = contents.replace('Location:', '')
def oktocont():
    ans = input("\033[0;36mPress Enter to Continue...\033[0m")
def clr():
    os.system('clear')
def spc():
    print("")
def lockdoor():
    clr()
    printlogo()
    spc()
    choiceupdate = input("        \033[92m[Confirmation]\033[90m\033[94m You want to update Lockdoor and all tools ? (Y / N) : \033[90m")
    if not choiceupdate in no:
        print("\033[92mUpdating Lockdoor...\033[90m")
        spc()
        os.system('pip install --upgrade lockdoor')
        spc()
        oktocont()
        print("\033[92mUpdating The Tools...\033[90m")
        spc()
        os.system('sudo find ' + installdirc + ' -type d -name .git -exec sh -c "cd \"{}\"/../ && pwd && git pull origin master" \;')
        clr()
        main.menu()