import os
from pathlib import Path
from lockdoors import shrts
from lockdoors import main
yes = set(['yes', 'y', 'ye', 'Y'])
no = set(['no', 'n', 'nop', 'N'])

def lockdoor():
    shrts.clr()
    shrts.printlogo()
    shrts.spc()
    choiceupdate = input("        \033[92m[Confirmation]\033[90m\033[94m You want to update Lockdoor and all tools ? (Y / N) : \033[90m")
    if not choiceupdate in no:
        print("\033[92mUpdating Lockdoor...\033[90m")
        shrts.spc()
        os.system('pip install --upgrade lockdoor')
        shrts.spc()
        shrts.oktocont()
        print("\033[92mUpdating The Tools...\033[90m")
        shrts.spc()
        os.system('sudo find ' + shrts.getinstalldir() + ' -type d -name .git -exec sh -c "cd \"{}\"/../ && git pull --ff-only origin master" \;')
        shrts.clr()
        main.menu()