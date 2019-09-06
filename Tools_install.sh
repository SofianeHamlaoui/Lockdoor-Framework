#!/bin/bash
# Author ; Sofiane Hamlaoui
# TEST LOCKDOOR SCRIPT

clear

cat << EOF 
.____                  __       .___                   
|    |    ____   ____ |  | __ __| _/____   ___________ 
|    |   /  _ \_/ ___\|  |/ // __ |/  _ \ /  _ \_  __ \

|    |__(   _  )  \___|    </ /_/ (   _  |   _  )  | \/
|_______ \____/ \___  >__|_ \____ |\____/ \____/|__|   
        \/          \/     \/    \/                                                                                                                                                                                                                     
                      Sofiane Hamlaoui | 2019
  Lockdoor A Penetration Testing framework with Cyber Security Resources
                                                              
EOF

## WORDLIST
git clone https://github.com/crunchsec/crunch.git ToolsResources/PASSWORD/Tools/crunch # a wordlist generator
git clone https://github.com/digininja/CeWL.git ToolsResources/PASSWORD/Tools/CeWL # a Custom Word List Generator
git clone https://github.com/lanjelot/patator.git ToolsResources/PASSWORD/Tools/patator # a multi-purpose brute-forcer, with a modular design and a flexible usage
## Downlaoding rockyou wordlist
wget -o ToolsResources/PASSWORD/wordlists/ https://gitlab.com/kalilinux/packages/wordlists/raw/kali/master/rockyou.txt.gz # The Rockyou.txt wordlist
gunzip ToolsResources/PASSWORD/wordlists/rockyou.txt.gz #  extracting the file
rm rockyou.txt.gz # removing the file

## EXPLOITATION
git clone https://github.com/1N3/Findsploit.git ToolsResources/EXPLOITATION/Tools/Findsploit # Find exploits in local and online databases instantly
git clone https://github.com/jm33-m0/mec.git ToolsResources/EXPLOITATION/Tools/MassExpConsole # concurrent exploiting
git clone https://github.com/rfunix/Pompem.git ToolsResources/EXPLOITATION/Tools/Pompem # Exploit and Vulnerability Finder
git clone https://github.com/joaogmauricio/rfix.git ToolsResources/EXPLOITATION/Tools/rfix #  Python tool that helps RFI exploitation.
git clone https://github.com/googleinurl/SCANNER-INURLBR.git ToolsResources/EXPLOITATION/Tools/InUrlBr # Advanced search in search engines
git clone https://github.com/jondonas/linux-exploit-suggester-2.git ToolsResources/EXPLOITATION/Tools/linux-exploit-suggester2 # Next-Generation Linux Kernel Exploit Suggester

## ENCRYPTION
git clone https://github.com/blackthorne/Codetective.git ToolsResources/ENCRYPTION/Codetective # a tool to determine the crypto/encoding algorithm used
git clone https://github.com/frdmn/findmyhash.git ToolsResources/ENCRYPTION/findmyhash # Python script to crack hashes using online services
git clone https://github.com/psypanda/hashID.git ToolsResources/ENCRYPTION/hashID # Software to identify the different types of hashes

## INFO-GATH
## INFO-GATH Frameworks
git clone https://github.com/s0md3v/ReconDog.git ToolsResources/INFO-GATH/Tools/Framework/ReconDog # Reconnaissance Swiss Army Knife
git clone https://github.com/Tuhinshubhra/RED_HAWK.git ToolsResources/INFO-GATH/Tools/Framework/RED_HAWK # All in one tool for Information Gathering, Vulnerability Scanning and Crawling
git clone https://github.com/0xInfection/TIDoS-Framework.git ToolsResources/INFO-GATH/Tools/Framework/TIDoS # Offensive Manual Web Application Penetration Testing Framework.
git clone https://github.com/Screetsec/Dracnmap.git ToolsResources/INFO-GATH/Tools/Framework/Dracnmap # Info Gathering Framework
########################
git clone https://github.com/ex0dus-0x/brut3k1t.git ToolsResources/INFO-GATH/Tools/brut3k1t # security-oriented bruteforce framework
git clone https://github.com/OJ/gobuster.git ToolsResources/INFO-GATH/Tools/gobuster # DNS and VHost busting tool written in Go 
git clone https://github.com/maurosoria/dirsearch.git ToolsResources/INFO-GATH/Tools/dirsearch # Web path scanner
git clone https://github.com/trickster0/Enyx.git ToolsResources/INFO-GATH/Tools/Enyx # Enyx SNMP IPv6 Enumeration Tool
git clone https://github.com/1N3/Goohak.git ToolsResources/INFO-GATH/Tools/Goohak # Launch Google Hacking Queries Against A Target Domain (Dork)
git clone https://github.com/tcstool/Nasnum.git ToolsResources/INFO-GATH/Tools/Nasnum # The NAS Enumerator
git clone https://github.com/aboul3la/Sublist3r.git ToolsResources/INFO-GATH/Tools/Sublist3r # Fast subdomains enumeration tool for penetration testers
git clone https://github.com/EnableSecurity/wafw00f.git ToolsResources/INFO-GATH/Tools/wafw00f # identify and fingerprint Web Application Firewall 
git clone https://github.com/s0md3v/Photon.git ToolsResources/INFO-GATH/Tools/Photon # ncredibly fast crawler designed for OSINT. 
git clone https://github.com/evyatarmeged/Raccoon.git ToolsResources/INFO-GATH/Tools/Raccoon # offensive security tool for reconnaissance and vulnerability scanning 
git clone https://github.com/0x09AL/raven.git ToolsResources/INFO-GATH/Tools/raven # raven is a Linkedin information gathering tool 
git clone https://github.com/darkoperator/dnsrecon.git ToolsResources/INFO-GATH/Tools/DnsRecon # DNS Enumeration Script
git clone https://github.com/codingo/Reconnoitre.git ToolsResources/INFO-GATH/Tools/Reconnoitre # multithreaded information gathering and service enumeratio tool
git clone https://github.com/sherlock-project/sherlock.git ToolsResources/INFO-GATH/Tools/sherlock # Find usernames across social networks
git clone https://github.com/hatlord/snmpwn.git ToolsResources/INFO-GATH/Tools/snmpwn # An SNMPv3 User Enumerator and Attack tool
git clone https://github.com/s0md3v/Striker.git ToolsResources/INFO-GATH/Tools/Striker #  an offensive information and vulnerability scanner. 
git clone https://github.com/laramies/theHarvester.git ToolsResources/INFO-GATH/Tools/theHarvester # E-mails, subdomains and names Harvester 
git clone https://github.com/eschultze/URLextractor.git ToolsResources/INFO-GATH/Tools/URLextractor # Information gathering & website reconnaissance
wget -O  ToolsResources/INFO-GATH/Tools/denumerator.py https://raw.githubusercontent.com/bl4de/security-tools/master/denumerator/denumerator.py # Enumerates list of subdomains
wget -O ToolsResources/INFO-GATH/CHEATSHEETS/Nmap-Cheet-Sheet.pdf http://arishitz.net/files/cheat_sheets/nmap_cheet_sheet_0.6.pdf # Nmap Cheat Sheet

## WEB
## WEB FrameWork
git clone https://github.com/joker25000/Dzjecter ToolsResources/WEB/Tools/Dzjecter # Server checking Tool
git clone https://github.com/andresriancho/w3af.git ToolsResources/WEB/Tools/W3af # web application attack and audit framework
#################
git clone https://github.com/c0ll3cti0n/Spaghetti.git ToolsResources/WEB/Tools/Spaghetti # Spaghetti - Web Application Security Scanner
git clone https://github.com/1N3/HTTPoxyScan.git ToolsResources/WEB/Tools/HTTPoxyScan # HTTPoxy Exploit Scanner by 1N3
git clone https://github.com/Dionach/CMSmap.git ToolsResources/WEB/Tools/CMSmap # CMS scanner 
git clone https://github.com/rajeshmajumdar/BruteXSS ToolsResources/WEB/Tools/BruteXSS # BruteXSS is a tool to find XSS vulnerabilities in web application
git clone https://github.com/fgeek/pyfiscan.git ToolsResources/WEB/Tools/PyFiScan # web-application vulnerability and version scanner 
git clone https://github.com/04x/J-dorker.git ToolsResources/WEB/Tools/J-dorker # Website List grabber from Bing
git clone https://github.com/droope/droopescan.git ToolsResources/WEB/Tools/droopescan # scanner , identify , CMSs , Drupal , Silverstripe. 
git clone https://github.com/joker25000/Optiva-Framework ToolsResources/WEB/Tools/Optiva-Framework # Web Application Scanner
git clone https://github.com/v3n0m-Scanner/V3n0M-Scanner.git ToolsResources/WEB/Tools/V3n0M # Pentesting scanner in Python3.6 for SQLi/XSS/LFI/RFI and other Vulns 
git clone https://github.com/04x/Priv8SqliTool.git ToolsResources/WEB/Tools/Priv8SqliTool # Find Sqli Targets v
git clone https://github.com/SofianeHamlaoui/sqliv.git ToolsResources/WEB/Tools/SqliV # massive SQL injection vulnerability scanner
git clone https://github.com/AlisamTechnology/ATSCAN.git ToolsResources/WEB/Tools/AtScan # Advanced dork Search & Mass Exploit Scanner 
git clone https://github.com/SofianeHamlaoui/Toolbox.git ToolsResources/WEB/Tools/ToolB0x # penetration testing and information gathering!
git clone https://github.com/m4ll0k/WPSeku.git ToolsResources/WEB/Tools/WPSeku # Wordpress Security Scanner 
git clone https://github.com/s0md3v/XSStrike.git ToolsResources/WEB/Tools/XSStrike # Most advanced XSS scanner.
git clone https://github.com/SofianeHamlaoui/WpBrute-Priv8.git ToolsResources/WEB/Tools/WP/WpBrute # Wordpress BruteForce Tools
git clone https://github.com/swisskyrepo/Wordpresscan.git ToolsResources/WEB/Tools/WP/Wpscan # A simple Wordpress scanner written in python
git clone git://git.kali.org/packages/joomscan.git ToolsResources/WEB/Tools/Joomla/joomscan # Joomla Vulnerability Scanner Project
git clone https://gitlab.com/SofianeHamlaoui/idtoolkit.git ToolsResources/WEB/Tools/WP/B7S-ToolB0x # Wordpress vulnerability scanner

## HARDWARE
git clone https://github.com/arismelachroinos/lscript.git ToolsResources/HRD/lscript # The Lazy Script 
git clone https://github.com/LionSec/xerosploit.git ToolsResources/HRD/xerosploit # Efficient and advanced man in the middle framework
git clone https://github.com/un1x00/ZIPCracker.git ToolsResources/HRD/ZIPCracker # cracking zip files using a brute force method
git clone https://github.com/NytroRST/NetRipper.git ToolsResources/HRD/NetRipper # NetRipper - Smart traffic sniffing for penetration testers
git clone https://github.com/g0tmi1k/msfpc.git ToolsResources/HRD/MsfPC # MSFvenom Payload Creator 

## POST-EXPL
git clone https://github.com/Screetsec/TheFatRat.git ToolsResources/POST-EXPL/Tools/TheFatRat # massive exploiting tool

# Priv Esc
git clone https://github.com/1N3/PrivEsc.git ToolsResources/PrivEsc/Tools # A collection of Windows, Linux and MySQL privilege escalation scripts and exploits

## REPORT
git clone https://github.com/dradis/dradis-ce.git ToolsResources/REPORT/Dradis # Dradis Framework: Colllaboration and reporting for IT Security
git clone https://github.com/maaaaz/nmaptocsv.git ToolsResources/REPORT/nmaptocsv # Converting Nmap output to CSV 
git clone https://github.com/phw/peek ToolsResources/REPORT/Peek-Gif-ScreenRecorder # Gif screen record (For Poc)

## SHELLS
git clone https://github.com/BlackArch/webshells.git ToolsResources/SHELLS/WebShells # Webshells Collection
git clone https://github.com/ManhNho/shellsum.git ToolsResources/SHELLS/ShellSum # A defense tool - detect web shells in local directories
git clone https://github.com/epinna/weevely3.git ToolsResources/SHELLS/Weevely # Weaponized web shell
git clone https://github.com/infodox/python-pty-shells ToolsResources/SHELLS/python-pty-shells # Python PTY backdoors

## Reverse 
git clone https://github.com/radare/radare2.git ToolsResources/REVERSE/Tools/Radare2 # unix-like reverse engineering framework
git clone https://github.com/botherder/virustotal.git ToolsResources/REVERSE/Tools/VirtusTotal # VirusTotal tools
git clone https://github.com/cea-sec/miasm.git ToolsResources/REVERSE/Tools/Miasm # Reverse engineering framework 
git clone https://github.com/slacknux/mirror.git ToolsResources/REVERSE/Tools/Mirror # reverses the bytes of a file
git clone https://github.com/0xd4d/dnSpy.git ToolsResources/REVERSE/Tools/DnSpy # .NET debugger and assembly

## Other
#Security
git clone https://github.com/SofianeHamlaoui/Security-Cheatsheets.git ToolsResources/SECURITY/CHEATSHEETS/ # Security Cheatsheets Download
# Peda python for gdb Installation
git clone https://github.com/longld/peda.git ~/peda
echo "source ~/peda/peda.py" >> ~/.gdbinit
clear
echo "DONE! debug your program with gdb and enjoy"
sleep 2
clear
# Clean
clear
ls