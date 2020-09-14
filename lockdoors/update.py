import os
from pathlib import Path
from lockdoors import main
yes = set(['yes', 'y', 'ye', 'Y'])
no = set(['no', 'n', 'nop', 'N'])

def lockdoor():
    main.clr()
    main.printlogo()
    main.spc()
    choiceupdate = input("        \033[92m[Confirmation]\033[90m\033[94m You want to update Lockdoor and all tools ? (Y / N) : \033[90m")
    if not choiceupdate in no:
        print("\033[92mUpdating Lockdoor...\033[90m")
        main.spc()
        os.system('pip install --upgrade lockdoor')
        main.spc()
        main.oktocont()
        print("\033[92mUpdating The Tools...\033[90m")
        main.spc()
        os.system('sudo find ' + main.getinstalldir() + ' -type d -name .git -exec sh -c "cd \"{}\"/../ && pwd && git pull origin master" \;')
        main.clr()
        main.menu()