#!/bin/bash
#Let's Begin ::
# Colors

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
        apt install python python-pip python3 python3-requests python3-pip gcc ruby php git wget bc curl netcat subversion openjdk-11-jre make automake gcc gzip rsync wget
        gem install bundler:1.17.2
    else
        echo -e "   ${g} [-] Skipping apt${endc}"
    fi

}

function chkpacman {
    which pacman > /dev/null 2>&1
    if [ "$?" -eq "0" ]; then
        echo -e "   ${g} [-] Installing the Packages${endc}"
        echo ""
        pacman -S python python-pip python-requests python2 python2-pip gcc ruby php git wget bc curl netcat subversion jre-openjdk make automake gcc linux-headers gzip rsync wget
        gem install bundler:1.17.2
    else
        echo -e "   ${g} [-] Skipping pacman${endc}"
    fi
}

function chkzypper {
    which zypper > /dev/null 2>&1
    if [ "$?" -eq "0" ]; then
        echo -e "   ${g} [-] Installing the Packages${endc}"
        echo ""
        zypper install python python-pip python-requests python2 python2-pip gcc ruby php git wget bc curl netcat subversion jre-openjdk make automake gcc linux-headers gzip rsync wget
        gem install bundler:1.17.2
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
        dnf install python python-pip python-requests python2 python2-pip gcc ruby php git wget bc curl netcat subversion jre-openjdk make automake gcc linux-headers gzip rsync wget
        gem install bundler:1.17.2
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
        yum install python python-pip python-requests python2 python2-pip gcc ruby php git wget bc curl netcat subversion jre-openjdk make automake gcc linux-headers gzip rsync wget
        gem install bundler:1.17.2
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
    echo -e "\e[32m[-] : Where do you want to install the script [/opt/sofiane/Pentest] !\e[0m"
    read installdir
    : ${installdir:=/opt/sofiane/Pentest}
    echo ""
    echo -e "\e[32m[-] Installing .... !\e[0m"
    echo ""
    mkdir -p $installdir
    mkdir -p $HOME"/.config/lockdoor"
    grep -l Sofiane /usr/local/bin/* | xargs rm
    echo "Location:"$installdir > $HOME"/.config/lockdoor/lockdoor.conf"
    rsync -a ToolsResources/* $installdir
    pip3 install lockdoor
    clear
    echo -e "\e[32m[-] Installing Go !\e[0m"
    echo ""
    # Installing Go
    wget https://dl.google.com/go/go1.13.linux-amd64.tar.gz
    tar -xvf go1.13.linux-amd64.tar.gz
    mv go /usr/local
    export GOROOT=/usr/local/go
    export PATH=$GOPATH/bin:$GOROOT/bin:$PATH
    rm -rf go*
    # RUN
    showlogo
    echo -e "                   ${y}Lockdoor Installed Succesfully !${endc}
                ${y}Type ${g}lockdoor${endc}${y} in ${r} Terminl${endc}${y} to use it${endc}"
}

#main
install
