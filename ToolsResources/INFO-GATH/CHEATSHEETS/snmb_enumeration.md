# SNMP Enumeration (Simple Network Management Protocol)

## Fix SNMP output values so they are human readable

```ShellSession
apt-get install snmp-mibs-downloader download-mibs
echo "" > /etc/snmp/snmp.conf
```

## Scanning for SNMP

### Using nmap

```ShellSession
root@kali:~# nmap -sU -v --open -p 161 192.168.1.12
   Note: SNMP is using UDP not TCP
```

### Using onesixtyone

```ShellSession
root@kali:~# echo public > /tmp/community.txt
root@kali:~# echo private >> /tmp/community.txt
root@kali:~# echo manager >> /tmp/community.txt
root@kali:~# echo 192.168.1.12 > /tmp/ip.txt
root@kali:~# onesixtyone -c /tmp/community.txt -i /tmp/ip.txt
```

## Windows SNMP Enumeration Example

```ShellSession
root@kali:~# snmpwalk -c puplic -v1 192.168.1.12
```

## SNMP Enumeration Commands

```ShellSession
snmpcheck -t $ip -c public

snmpwalk -c public -v1 $ip 1|

grep hrSWRunName|cut -d\* \* -f

snmpenum -t $ip

onesixtyone -c names -i hosts
```

## SNMPv3 Enumeration

```ShellSession
nmap -sV -p 161 --script=snmp-info $ip/24
```

## Automate the username enumeration process for SNMPv3:

```ShellSession
apt-get install snmp snmp-mibs-downloader
wget https://raw.githubusercontent.com/raesene/TestingScripts/master/snmpv3enum.rb
```

## SNMP Default Credentials

```ShellSession
/usr/share/metasploit-framework/data/wordlists/snmp\_default\_pass.tx
```


### SNMP Enumeration (Simple Network Management Protocol)

- SNMP is based on UDP, a simple, stateless protocol, and is therefore susceptible to IP spoofing, and replay attacks.
- In addition, the commonly used SNMP protocols 1, 2, and 2c offer no traffic encryption, meaning SNMP information and credentials can be easily intercepted over a local network.
- For all these reasons, SNMP is another of our favorite enumeration protocols.

### MIB Tree (SNMP Management Information Base)

- (MIB) is a database containing information usually related to network management.
- The database is organized like a tree, where branches represent different organizations or network functions. The leaves of the tree (final endpoints) correspond to specific variable values that can then be accessed, and probed, by an external user.
- [Read more about the MIB](http://www-01.ibm.com/support/knowledgecenter/ssw_aix_53/com.ibm.aix.progcomm/doc/progcomc/mib.htm%23jkmb0ria)

### Scanning for SNMP

  ```Bash
  > nmap -sU --open -p 161 192.168.11.200-254 -oG mega-snmp.txt
  # -sU :: UDP scan
  ```

- Alternatively, we can use a tool such as __onesixtyone__, which will check for given community strings against an IP list, allowing us to brute force various community strings.

  ```Bash
  > echo public > community
  > echo private >> community
  > echo manager >> community
  > for ip in $(seq 200 254);do echo 192.168.11.$ip;done > ips
  > onesixtyone -c community i ips
  ```

  Once these SNMP services are found, we can start querying them for specific MIB data that might be interesting to us.

### Windows SNMP Enumeration Example

- We can probe and query SNMP values using a tool such as __snmpwalk__ provided we at least know the SNMP read-only community string, which in most cases is “public”.
- Using some of the MIB values provided above, we could attempt to enumerate their corresponding values.
- Try out the following examples against a known machine in the labs, which has a Windows SNMP port exposed with the community string “public”.

  ```Bash
  # Enumerating the Entire MIB Tree
  > snmpwalk  c public -v1 192.168.11.219

  # Enumerating Windows Users:
  > snmpwalk -c public -v1 192.168.11.204 1.3.6.1.4.1.77.1.2.25

  # Enumerating Running Windows Processes:
  > snmpwalk -c public -v1 192.168.11.204 1.3.6.1.2.1.25.4.2.1.2

  # Enumerating Open TCP Ports:
  > snmpwalk -c public -v1 192.168.11.204 1.3.6.1.2.1.6.13.1.3

  # Enumerating Installed Software:
  > snmpwalk -c public v1 192.168.11.204 1.3.6.1.2.1.25.6.3.1.2
  ```

- try to Use __snmpwalk__ and __snmpcheck__ to gather information about the discovered targets.
