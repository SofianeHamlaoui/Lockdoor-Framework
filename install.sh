#!/bin/bash
#Let's Begin ::
# Colors
#v0.6
b='\033[1m'
u='\033[4m'
bl='\E[30m'
r='\E[31m'
g='\E[32m'
y='\E[33m'
bu='\E[34m'
m='\E[35m'
c='\E[36m'
w='\E[37m'
endc='\E[0m'
enda='\033[0m'

function showlogo {
    clear
echo """
.____                  __           .__  .__                
|    |    ____   ____ |  | _______  |  | |  |   ___________ 
|    |   /  _ \_/ ___\|  |/ /\__  \ |  | |  | _/ __ \_  __ \

|    |__(  <_> )  \___|    <  / __ \|  |_|  |_\  ___/|  | \/
|_______ \____/ \___  >__|_ \(____  /____/____/\___  >__|   
        \/          \/     \/     \/               \/                                                                                                                                           
        Sofiane Hamlaoui | 2019
""";
    echo
}
function checkroot {
  showlogo
  if [[ $(id -u) = 0 ]]; then
    echo -e " Checking For ROOT: ${g}PASSED${endc}"
    echo ""
  else
    echo -e " Checking For ROOT: ${r}FAILED${endc}
 ${y}This Script Needs To Run As ROOT${endc}"
    echo ""
    echo -e " ${g}Lockdoor Installer${enda} Will Now Exit"
    echo
    sleep 1
    exit
  fi
}

function chkapt {
    which apt > /dev/null 2>&1
    if [ "$?" -eq "0" ]; then
        echo -e "   ${g} [-] Installing the Packages${endc}"
        echo ""
        apt install python python-pip python3 python3-requests python3-pip gcc ruby php git wget bc curl
    else
        echo -e "   ${g} [-] Skipping apt${endc}"
    fi

}

function chkpacman {
    which pacman > /dev/null 2>&1
    if [ "$?" -eq "0" ]; then
        echo -e "   ${g} [-] Installing the Packages${endc}"
        echo ""
        pacman -S python python-pip python-requests python2 python2-pip gcc ruby php git wget bc curl 
    else
        echo -e "   ${g} [-] Skipping pacman${endc}"
    fi
}

function chkzypper {
    which zypper > /dev/null 2>&1
    if [ "$?" -eq "0" ]; then
        echo -e "   ${g} [-] Installing the Packages${endc}"
        echo ""
        zypper install python python-pip python-requests python2 python2-pip gcc ruby php git wget bc curl
    else
        echo -e "   ${g} [-] Skipping zypper${endc}"
    fi
    sleep 1
}

function chkdnf {
    which dnf > /dev/null 2>&1
    if [ "$?" -eq "0" ]; then
        echo -e "   ${g} [-] Installing the Packages${endc}"
        echo ""
        dnf install python python-pip python-requests python2 python2-pip gcc ruby php git wget bc curl
    else
        echo -e "   ${g} [-] Skipping dnf${endc}"
    fi
    sleep 1
}

function chkyum {
    which yum > /dev/null 2>&1
    if [ "$?" -eq "0" ]; then
        echo -e "   ${g} [-] Installing the Packages${endc}"
        echo ""
        yum install python python-pip python-requests python2 python2-pip gcc ruby php git wget bc curl
    else
        echo -e "   ${g} [-] Skipping yum${endc}"
    fi
    sleep 1
}

function install {
    showlogo && checkroot && chkapt && chkzypper && chkdnf && chkyum && chkpacman
    clear 
    showlogo
    echo ""
    echo -e "\e[32m[-] : Where do you want to install the script [/opt/Pentest] !\e[0m"
    read installdir
    mkdir $installdir
    echo "Location:"$installdir > $HOME"/.config/lockdoor/lockdoor.conf"
    mv ToolsResources/* $installdir
    pip3 install lockdoor
    # Cleaning
    rm -rf build dist lockdoor.egg* pictures ToolsResources
    rm lockdoor README.rst install.sh 
    # RUN
    lockdoor
}

#main
install