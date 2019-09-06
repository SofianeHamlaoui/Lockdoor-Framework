# SMTP Enumeration (Simple Mail Transfer Protocol)

```ShellSession
root@kali:~# nc -nv 192.168.1.12 25
(UNKNOWN) [192.168.1.12] 25 (smtp) open
220 WIN-3UR24XX66QZ Microsoft ESMTP MAIL Service, Version: 7.0.6001.18000 ready at    Thu, 4 Jan 2018 11:48:35 +0200
```

- mail servers can also be used to gather information about a host or network.
- SMTP supports several important commands, such as VRFY and EXPN.
- A VRFY request asks the server to verify an email address
- while EXPN asks the server for the membership of a mailing list.
- These can often be abused to verify existing users on a mail server, which can later aid the attacker.

```Bash

# This procedure can be used to help guess valid usernames.
> nc -nv 192.168.11.215 25

```

- Examine the following simple Python script that opens a TCP socket, connects to the SMTP server, and issues a VRFY command for a given username.

```python

# !/usr/bin/python
import socket
import sys

if len(sys.argv) != 2:
  print "Usage: vrfy.py <username>"
  sys.exit(0)

# Create a Socket
s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect to the Server
connect=s.connect(('192.168.11.215',25))

# Receive the banner
banner=s.recv(1024)
print banner

# VRFY a user
s.send('VRFY' - sys.argv[1] - '\r\n')
result=s.recv(1024)
print result

# Close the socket
s.close()

```
