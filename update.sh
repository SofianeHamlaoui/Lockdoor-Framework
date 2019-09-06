#!/bin/bash
#Let's Begin ::

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
function checkroot { # Checking root
  showlogo && sleep 1
  if [[ $(id -u) = 0 ]]; then
    echo -e " Checking For ROOT: ${g}PASSED${endc}"
  else
    echo -e " Checking For ROOT: ${r}FAILED${endc}
 ${y}[!] You Must Run The Update As ROOT [!]${y}"
    echo -e " ${b}Lockdoor_update${enda} Will Now Exit"
    echo
    sleep 1
    exit
  fi
}
function update {
    showlogo && checkroot && sleep 1
    source install.sh
    rm -rf "$INSTALL_DIR"
    echo -e "${y}[!] Updating ...${endc}";
    echo "";
    git clone https://github.com/SofianeHamlaoui/Lockdoor-Framework "$INSTALL_DIR" 2>/dev/null;
    if [ -d "$INSTALL_DIR" ] ; then
    echo -e "${g}[✔] Lockdoor was Updated successfully ! Enjoy${endc}" ;
    else
    echo -e "${r}[✘] Update Failed !!! [✘]${endc}";
    exit
    fi
}
# calling
update

#HAMDOULLILAH <3 
# End