#!/bin/sh
#just to show you how - you can edit this to your liking
#by using dcedump and ifids or epdump to find a UUID and Version and port.


#defaults
MAX=20
#USERNAME="Administrator"
#PASSWORD="jbone"
#USERNAME=""
#PASSWORD=""
TRIES=3000
NUMOFSTRINGS=10


TARGET=192.168.1.105
STARTFUNCTION=0

#locator UUIDs
#bug is in function 0
PIPE="locator"
VERSIONMAJOR=1
VERSIONMINOR=0
UUID=d3fbb514-0e3b-11cb-8fad-08002b1d29c3
STARTFUNCTION=0
#MAX=8
#UUID=d6d70ef0-0e3b-11cb-acc3-08002b1d29c3
#VULN FUNCTION 0! (1,0)
#UNCOMMENT BELOW TO CRASH LOCATOR IF INITIALIZED
#UUID=e33c0cc4-0482-101a-bc0c-02608c6ba218


#LSA \lsarpc
#PIPE="\\lsarpc"
#UUID=12345778-1234-abcd-ef00-0123456789ab
#VERSIONMAJOR=1
#VERSIONMINOR=0
#Wins \wins
#UUID=45f52c28-7f9f-101a-b52b-08002b2efabe
#?
#UUID=6bffd098-a112-3610-9833-46c3f87e345a


#tapsrv \tapsrv 1,0
#UUID=1ff70682-0a51-30e8-076d-740be8cee98b
#PIPE="\\tapsrv"
#VERSIONMAJOR=1
#VERSIONMINOR=0


#\wkssvc
#UUID=5a7b91f8-ff00-11d0-a9b2-00c04fb6e6fc
#generates funy messages on XP Home when fuzzed in event log
#PIPE="\\wkssvc"
#VERSIONMAJOR=1
#VERSIONMINOR=0


#WINS.EXE
#PIPE="\\winsvc"
#UUID=811109bf-a4e1-11d1-ab54-00a0c91e9b45
#UUID=45f52c28-7f9f-101a-b52b-08002b2efabe
#VERSIONMAJOR=1
#VERSIONMINOR=0


#LSASS.EXE

#PIPE="\\lsarpc"
#UUID=12345778-1234-abcd-ef00-0123456789ab 
#VERSIONMAJOR=0
#VERSIONMINOR=0
#UUID=c681d488-d850-11d0-8c52-00c04fd90f7e v1.0 (no)
#VERSIONMAJOR=0
#VERSIONMINOR=0
#v0.0
#UUID=3919286a-b10c-11d0-9ba8-00c04fd92ef5 
#UUID=12345778-1234-abcd-ef00-0123456789ac v1.0
#UUID=ecec0d70-a603-11d0-96b1-00a0c91ece30 v1.0
#UUID=16e0cf3a-a604-11d0-96b1-00a0c91ece30 v1.0
#UUID=e3514235-4b06-11d1-ab04-00c04fc2dcd2 v4.0
#UUID=f5cc5a7c-4264-101a-8c59-08002b2f8426 v21.0
#UUID=f5cc5a18-4264-101a-8c59-08002b2f8426 v56.0
#UUID=12345678-1234-abcd-ef00-01234567cffb v1.0
#UUID=d335b8f6-cb31-11d0-b0f9-006097ba4e54 v1.5
#UUID=98fe2c90-a542-11d0-a4ef-00a0c9062910 v1.0


#llsscv.exe
#PIPE="\\lssrpc"
#UUID=57674cd0-5200-11ce-a897-08002b2e9c6d
#VERSIONMAJOR=1
#VERSIONMINOR=0

#UUID=342cfd40-3c6c-11ce-a893-08002b2e9c6d
#VERSIONMAJOR=0
#VERSIONMINOR=0
#MAX=86


#VERSIONMAJOR=1
#VERSIONMINOR=0
#PIPE="locator"


#SQL Server 2000
#PIPE="\\.\\pipe\\sql\\query"
#VERSIONMAJOR=1
#VERSIONMINOR=0
#Wrong
#UUID="342cfd40-3c6c-11ce-a893-08002b2e9c6d"

#EPMAPPER - not listening
#PIPE="\\PIPE\\epmapper"
#VERSIONMAJOR=3
#VERSIONMINOR=0
#UUID=e1af8308-5d1f-11c9-91a4-08002b14a0fa


#NNTP
#ACCESS DENIED as ANON
#PIPE="\\PIPE\\NNTPSERVICE"
#VERSIONMAJOR=4
#VERSIONMINOR=0
#UUID=4f82f460-0e21-11cf-909e-00805f48a135

#Messanger Service
#ACCESS DENIED
#PIPE="\\PIPE\\ntsvcs"
#PIPE="scerpc"
#UUID=5a7b91f8-ff00-11d0-a9b2-00c04fb6e6fc
#VERSIONMAJOR=1
#VERSIONMINOR=0

#Inetinfo.exe
#
#PIPE="\\PIPE\\INETINFO"
#UUID=4f82f460-0e21-11cf-909e-00805f48a135
#VERSIONMAJOR=4
#VERSIONMINOR=0


#NtFrs
#PIPE="\\PIPE\\000002b4.000"
#UUID=d049b186-814f-11d1-9a3c-00c04fc9b232
#VERSIONMAJOR=1
#VERSIONMINOR=0

#MSTASK
#some weird error when NTAndX connect
#PIPE="\\PIPE\\atsvc"
#VERSIONMAJOR=1
#VERSIONMINOR=0
#UUID=378e52b0-c0a9-11cf-822d-00aa0051e40f

#DNS.EXE
#ACCESS DENIED
#PIPE="DNSSERVER"
#VERSIONMAJOR=5
#VERSIONMINOR=0
#UUID=50abc2a4-574d-40b3-9d66-ee4fd5fba076

#Bindview's ifids is great for this
#cygwin command:
#  ./ifids -p ncacn_np -e \\pipe\\W32TIME \\\\.
#XP Audit
#PIPE="\\trkwks"
#VERSIONMAJOR=1
#VERSIONMINOR=0
#UUID=5a7b91f8-ff00-11d0-a9b2-00c04fb6e6fc

#PIPE="\\Ctx_WinStation_API_service"
#UUID=5a7b91f8-ff00-11d0-a9b2-00c04fb6e6fc
#VERSIONMAJOR=1
#VERSIONMINOR=0

#PIPE="\\PIPE\\atsvc"
#UUID=1ff70682-0a51-30e8-076d-740be8cee98b
#VERSIONMAJOR=1
#VERSIONMINOR=0

PIPE="\\startup\\"

i=$STARTFUNCTION
while [ "$i" -lt "$MAX" ]; do
    echo "*************************************************************************************************"
    echo "Doing function $i try $j"
    echo "./dceoversmb $TARGET $PIPE $UUID $VERSIONMAJOR $VERSIONMINOR $i  $TRIES $NUMOFSTRINGS $USERNAME $PASSWORD 2>> /crypt/out_smbfuzz"
START=`date`
    ./dceoversmb $TARGET $PIPE $UUID $VERSIONMAJOR $VERSIONMINOR $i $TRIES $NUMOFSTRINGS $USERNAME $PASSWORD 2>> /dev/null
echo "START=$START"
END=`date`
echo "END=$END"

    i=`expr $i + 1`
done

echo "DONE!"
