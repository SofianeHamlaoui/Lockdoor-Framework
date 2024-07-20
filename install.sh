#!/bin/bash

# Let's Begin
# Colors
b='\033[1m'
u='\033[4m'
colors=( '\E[30m' '\E[31m' '\E[32m' '\E[33m' '\E[34m' '\E[35m' '\E[36m' '\E[37m' '\E[0m' )
bl=${colors[0]}
r=${colors[1]}
g=${colors[2]}
y=${colors[3]}
bu=${colors[4]}
m=${colors[5]}
c=${colors[6]}
w=${colors[7]}
endc=${colors[8]}
enda='\033[0m'

trap ctrl_c INT

function ctrl_c() {
    echo ""
    echo -e "                   ${r}Do you want to leave the installation?${endc}"
    read -p "                                (Y/N):  " yn
    case $yn in
        [Yy]* ) exit;;
        [Nn]* ) install;;
        * ) install;;
    esac
    echo ""
}

function showlogo {
    clear
    echo """
.____                  __           .__  .__
|    |    ____   ____ |  | _______  |  | |  |   ___________
|    |   /  _ \_/ ___\|  |/ /\__  \ |  | |  | _/ __ \_  __ \\
|    |__(  <_> )  \___|    <  / __ \|  |_|  |_\  ___/|  | \/
|_______ \____/ \___  >__|_ \(____  /____/____/\___  >__|
        \/          \/     \/     \/               \/
        Sofiane Hamlaoui | 2021 | v2.3
    """
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

function install_packages {
    local pkg_manager=$1
    local install_cmd=$2
    which $pkg_manager > /dev/null 2>&1
    if [ "$?" -eq "0" ]; then
        echo -e "   ${g} [-] Installing the Packages with $pkg_manager${endc}"
        $install_cmd > /dev/null 2>&1
        gem install bundler:1.17.2 > /dev/null 2>&1
    else
        echo -e "   ${g} [-] Skipping $pkg_manager${endc}"
    fi
    sleep 1
}

function install_pv {
    which pv > /dev/null 2>&1
    if [ "$?" -ne "0" ]; then
        echo -e "   ${y} [-] 'pv' not found. Installing 'pv'...${endc}"
        install_packages "apt" "apt-get install -y pv"
        install_packages "pacman" "pacman -S --noconfirm pv"
        install_packages "zypper" "zypper install -y pv"
        install_packages "dnf" "dnf install -y pv"
        install_packages "yum" "yum install -y pv"
    fi
}

function show_progress {
    local duration=$1
    local increment=$(($duration / 20))
    local progress=0

    while [ $progress -lt 100 ]; do
        echo -ne "["
        for ((i=0; i<$progress; i+=5)); do echo -ne "#"; done
        for ((i=$progress; i<100; i+=5)); do echo -ne "."; done
        echo -ne "] $progress% \r"
        sleep $increment
        progress=$((progress + 5))
    done
    echo -ne "["
    for ((i=0; i<100; i+=5)); do echo -ne "#"; done
    echo -ne "] 100% \r"
    echo
}

function install {
    showlogo && checkroot

    install_pv

    echo "Starting installation..."
    show_progress 30 &
    progress_pid=$!

    install_packages "apt" "apt-get install -y golang-go python3 python3-pip python3-requests gcc ruby php git wget bc curl netcat-traditional subversion openjdk-11-jre make automake gzip rsync"
    install_packages "pacman" "pacman -S go python python-pip python-requests python2 python2-pip gcc ruby php git wget bc curl netcat subversion jre-openjdk make automake gzip rsync"
    install_packages "zypper" "zypper install -y go python python-pip python-requests python2 python2-pip gcc ruby php git wget bc curl netcat subversion jre-openjdk make automake gzip rsync"
    install_packages "dnf" "dnf install -y go python python-pip python-requests python2 python2-pip gcc ruby php git wget bc curl netcat subversion jre-openjdk make automake gzip rsync"
    install_packages "yum" "yum install -y go python python-pip python-requests python2 python2-pip gcc ruby php git wget bc curl netcat subversion jre-openjdk make automake gzip rsync"
    
    wait $progress_pid

    clear
    showlogo
    echo ""
    echo -e "\e[32m[-] : Where do you want to install the script [/opt/sofiane/Pentest]? \e[0m"
    read installdir
    : ${installdir:=/opt/sofiane/Pentest}
    echo ""
    echo -e "\e[32m[-] Installing .... !\e[0m"
    echo ""

    mkdir -p $installdir
    mkdir -p $HOME"/.config/lockdoor"
    files=$(grep -l Sofiane /usr/local/bin/* 2>/dev/null)
    if [ -n "$files" ]; then
        echo "$files" | xargs rm
    fi
    echo "Location: $installdir" > $HOME"/.config/lockdoor/lockdoor.conf"

    echo "Syncing files..."
    rsync -a --info=progress2 ToolsResources/* $installdir | pv -lep -s $(du -sb ToolsResources | awk '{print $1}') > /dev/null 2>&1

    pip3 install lockdoor > /dev/null 2>&1

    clear
    # RUN
    showlogo
    echo -e "                   ${y}Lockdoor Installed Successfully!${endc}
                ${y}Type ${g}lockdoor${endc}${y} in the ${r}Terminal${endc}${y} to use it${endc}"
}

# main
install
