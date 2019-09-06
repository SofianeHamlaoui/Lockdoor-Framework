# DNS Enumeration

DNS offers a variety of information about public (and sometimes private!) organization servers, such as IP addresses, server names, and server functionality.

### Interacting with a DNS Server

```Bash
> host -t ns megacorpone.com           # -t : type , ns: dns
> host -t mx megacorpone.com           # mx : mail server
```

- Also you can use `nslookup`

```Bash
> nslookup anasboureada.com
```

- `dig` also can be used

```Bash
> dig aboureada.com
```

### Automating lookups

we have some initial data from the megacorpone.com domain, we can continue to use additional DNS queries to discover more host names and IP addresses belonging to megacorpone.com.

```Bash
> host www.megacorpone.com             # we will found that it has an ip
> host idontexist.megacorpone.com      # this is not found
```

### Forward Lookup Brute Force

Taking the previous concept a step further, we can automate the Forward DNS Lookup of common host names using the host command and a Bash script.

```Bash
> echo www > list.txt
> echo ftp >> list.txt
> echo mail >> list.txt
> echo owa >> list.txt
> echo proxy >> list.txt
> echo router >> list.txt
> echo api >> list.txt
> for ip in $(cat list.txt);do host $ip.megacorpone.com;done
```

### Reverse Lookup Brute Force

If the DNS administrator of megacorpone.com configured PTR records for the domain, we might find out some more domain names that were missed during the forward lookup brute-force phase.

```Bash
> for ip in $(seq 155 190);do host 50.7.67.$ip;done | grep -v "not found"
# grep -v :: --invert-match
```

#### DNS Zone Transfers

- A zone transfer is similar to a database replication act between related DNS servers.
- This process includes the copying of the zone file from a master DNS server to a slave server.
- The zone file contains a list of all the DNS names configured for that zone. Zone transfers should usually be limited to authorized slave DNS servers.

```Bash
> host -l megacorpone.com ns1.megacorpone.com   # ns1 refused us our zone transfer request
# -l :: list all hosts in a domain
> host -l megacorpone.com ns2.megacorpone.com
# The result is a full dump of the zone file for the megacorpone.com domain,
# providing us a convenient list of IPs and DNS names for the megacorpone.com domain.
```

```Bash
> host -t axfr zonetransfer.me nsztm1.digi.ninja.
```

```Bash
> dig axfr nsztm1.digi.ninja zonetransfer.me
```

- Now Lets automate the process:

  - To get the name servers for a given domain in a clean format, we can issue the following command.

    ```Bash
    > host -t ns megacorpone.com | cut -d " " -f 4
    # -d :: --delimiter=DELIM ;
    # -f ::  --fields=LIST select only these fields on each line;
    ````

  - Taking this a step further, we could write the following simple Bash script to automate the procedure of discovering and attempting a zone transfer on each DNS server found.

    ```Bash
    # /bin/bash
    # Simple Zone Transfer Bash Script
    # $1 is the first argument given after the bash script
    # Check if argument was given, if not, print usage
    if  [-z "$1" ]; then
    echo "[-] Simple Zone transfer script"
    echo "[-] Usage : $0 <domain name> "
    exit 0
    fi

    # if argument was given, identify the DNS servers for the domain
    for server in $(host ­-t ns $1 | cut ­-d" " ­-f4);do
    # For each of these servers, attempt a zone transfer
    host -l $1 $server | grep "has address"
    done
    ```

    Running this script on megacorpone.com should automatically identify both name servers and attempt a zone transfer on each of them

    ```Bash
    > chmod 755 dns-­-axfr.sh
    > ./dns-­-axfr.sh megacorpone.com
    ```

### Relevant Tools in Kali Linux

#### DNSRecon

  ```Bash
  > dnsrecon -d megacorpone.com -t axfr
  # -d :: domain
  # -t :: type of Enumeration to perform
  # axfr :: test all ns servers for zone transfer
  ```

#### DNSEnum

  ```Bash
  > dnsenum zonetransfer.me
  ```

#### fierce

**NOTE** the one included in the latest version of kali may not work, so try to install the new version from [fierce](https://github.com/mschwager/fierce)

  ```Bash
  > pip3 install fierce
  > fierce --domain zonetransfer.me
  ```

- NMAP DNS Hostnames Lookup

```ShellSession
nmap -F --dns-server
```

- Host Lookup

```ShellSession
host -t ns [megacorpone.com](http://megacorpone.com/)
```

- Reverse Lookup Brute Force - find domains in the same range

```ShellSession
for ip in $(seq 155 190);do host 50.7.67.$ip;done |grep -v "not found"
```

- Perform DNS IP Lookup

```ShellSession
dig a [domain-name-here.com](http://domain-name-here.com/) @nameserver
```

- Perform MX Record Lookup

```ShellSession
dig mx [domain-name-here.com](http://domain-name-here.com/) @nameserver
```

- Perform Zone Transfer with DIG

```ShellSession
dig axfr [domain-name-here.com](http://domain-name-here.com/) @nameserver
```

## DNS Zone Transfers

- Windows DNS zone transfer

```ShellSession
nslookup -> set type=any -> ls -d [blah.com  ](http://blah.com/)
```

- Linux DNS zone transfer

```ShellSession
dig axfr [blah.com](http://blah.com/) @[ns1.blah.com](http://ns1.blah.com/)
```

- Dnsrecon DNS Brute Force

```ShellSession
dnsrecon -d TARGET -D /usr/share/wordlists/dnsmap.txt -t std --xml ouput.xml
```

- Dnsrecon DNS List of megacorp

```ShellSession
dnsrecon -d [megacorpone.com](http://megacorpone.com/) -t axfr
```

- DNSEnum

```ShellSession
dnsenum zonetransfer.m
```
