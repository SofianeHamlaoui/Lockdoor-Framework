########################
 DIRB - URL Bruteforcer
########################

darkraver@open-labs.org (http://dirb.sf.net) 


What is DIRB?
------------

DIRB is a Web Content Scanner. It looks for existing (and/or hidden) Web 
Objects. It basically works by launching a dictionary based attack against 
a web server and analizing the response.

DIRB comes with a set of preconfigured attack wordlists for easy usage but 
you can use your custom wordlists. Also DIRB sometimes can be used as a 
classic CGI scanner, but remember is a content scanner not a vulnerability 
scanner.

DIRB main purpose is to help in professional web application auditing. 
Specially in security related testing. It covers some holes not covered by 
classic web vulnerability scanners. DIRB looks for specific web objects that 
other generic CGI scanners can't look for. It doesn't search vulnerabilities 
nor does it look for web contents that can be vulnerables.

Maybe the last try for an unlucky security analyst... :)


What is NOT?
------------

DIRB is NOT a Web Spider. It doesn't follow HTML links (by now). It searches 
content by rules and dictionary based attacks.

DIRB is NOT a Web Downloader. It doesn't download Web Pages (by now), only 
test they existence for later manual analysis.

DIRB is NOT a Web Vulnerability Scanner. It does not look for bugs. But it's 
designed for helping in web vulnerability assessment.


Installation
------------

DIRB is based on libcurl so you need to install this library where autoconf 
can locate it. Once libcurl is installed properly you must only do:

	$ ./configure
	$ make


Usage
-----

DIRB takes 2 main parameters, the base URL for testing and a list of wordlist 
files used for the attack. Example:

	$ ./dirb.exe http://www.test.org/ common.txt 


The URL must be a valid standard URL and the wordlists are simple text files 
with a word by line. It is also possible to scan subdirectories directly:

	$ ./dirb.exe http://www.test.org/html/ common.txt
	
	
For SSL simply include the HTTPS url:

	$ ./dirb.exe https://www.test.org/ common.txt -i


You can use multiple wordfiles at a time this way (separated by comma):

	$ ./dirb.exe https://www.test.org/ common.txt,spanish.txt,names.txt 
	

You can append different extensions to the probed words, by using the -x or 
the -X option:

	$ ./dirb.exe https://www.test.org/ common.txt -X .html,.asp,.jsp,,

	$ ./dirb.exe https://www.test.org/ common.txt -x extensions.txt
	
	
Examples
--------

+ Scan a webserver for common directories/files: (without using file 
extensions)
	
	$ ./src/dirb.exe http://www.test.org/ wordlists/common.txt	


+ Scan a webserver for common directories/files: (search for PHP and HTML 
files)

	$ ./src/dirb.exe http://www.test.org/ wordlists/common.txt -X .php,.html	


+ When a file is found, try different variations: (~, .old, etc...)

	$ ./src/dirb.exe http://www.test.org/ wordlists/common.txt -X .php,.html -M ~,.tmp,.old,.backup,.test
	


Bugs
----

There are a lot :)

Please notify them to: darkraver@open-labs.org


Credits
-------

Project manager: The Dark Raver

Contributors: Sage, Jfs, Warezzman, The Dark Raver, Sha0, Hubert Seiwert, Pablo Catalina 

Beta-testers, Ideas: Necronoid, Fatuo, IaM, Laramies, Mandingo


