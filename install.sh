#!/bin/bash
clear

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

# showing Lockdoor Logo
function showlogo {
  clear
echo -e  """${r}
.____                  __       .___                   
|    |    ____   ____ |  | __ __| _/____   ___________ 
|    |   /  _ \_/ ___\|  |/ // __ |/  _ \ /  _ \_  __ \

|    |__(   _  )  \___|    </ /_/ (   _  |   _  )  | \/
|_______ \____/ \___  >__|_ \____ |\____/ \____/|__|   
        \/          \/     \/    \/                                                                                                                                                                                                                     
                      Sofiane Hamlaoui | 2019
  Lockdoor A Penetration Testing framework with Cyber Security Resources

""";
echo -e "${y} Twitter.com/S0fianeHamlaoui | Github.com/SofianeHamlaoui${endc}"
}

# Variables : 
INSTALL_DIR="/opt/sofiane/lockdoor"
BIN_DIR="/usr/bin/"
# Functions : 
function checkroot { # Checking root
  showlogo && sleep 1
  if [[ $(id -u) = 0 ]]; then
    echo -e " Checking For ROOT: ${g}PASSED${endc}"
  else
    echo -e " Checking For ROOT: ${r}FAILED${endc}
 ${y}[!] This Tool Must Run As ROOT [!]${y}"
    echo -e " ${b}Lockdoor${enda} Will Now Exit"
    echo
    sleep 1
    exit
  fi
}

function dircheck { # Directory Check
    showlogo && sleep 1
    if [ -d "$INSTALL_DIR" ]; then
    echo -e " ${bu}[!] A Directory Lockdoor Was Found.. Do You Want To Replace It ? [y/n]:${endc}" ;
   
    read rep
    if [ "$rep" = "y" ]; then
        rm -R "$INSTALL_DIR"
    else
        exit
    fi
fi
}

function installing { # Installing Lockdoor
    showlogo && checkroot && dircheck && sleep 1
    echo -e "${y}[!] Installing ...${endc}";
    echo "";
    git clone https://github.com/SofianeHamlaoui/Lockdoor-Framework "$INSTALL_DIR" 2>/dev/null;
    if [ -d "$INSTALL_DIR" ] ; then
    echo -e "${g}[✔] Lockdoor was installed successfully ! Enjoy${endc}" ;
    else
    echo -e "${r}[✘] Installation Failed !!! [✘]${endc}";
    exit
    fi
}

installing
