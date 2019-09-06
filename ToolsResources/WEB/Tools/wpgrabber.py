#!/usr/bin/python

# wordpress grabber

'''


Coded by Ph Dz
Use my code as you want :D    

'''

import re , urllib2 , sys , os
from platform import system

if system() == 'Linux':
    os.system('clear')
if system() == 'Windows':
    os.system('cls')

logo = '''

 _       ______
| |     / / __ \  | ----| Wordpress Grabber |----
| | /| / / /_/ /  | Author : Ph Dz
| |/ |/ / ____/   | FB : www.fb.com/red.DZ.hacker
|__/|__/_/        | Email : dzph@bk.ru
                
[*] Usage : python '''+sys.argv[0]+''' 127.0.0.1
'''

# found this code on stackoverflow.com/questions/19278877
def unique(seq):
    seen = set()
    return [seen.add(x) or x for x in seq if x not in seen]

print(logo)
try:
	lista = []
	s = sys.argv[1]
	page = 1
	print('\n')
	while page <= 21:
		bing = "http://www.bing.com/search?q=ip%3A"+s+"+?page_id=&count=50&first="+str(page)
		openbing  = urllib2.urlopen(bing)
		readbing = openbing.read()
		findwebs = re.findall('<h2><a href="(.*?)"' , readbing)
		for i in range(len(findwebs)):
			wpnoclean = findwebs[i]
			findwp = re.findall('(.*?)\?page_id=', wpnoclean)
			lista.extend(findwp)

		page = page + 10

	final =  unique(lista)
	for wp in final:
		print(wp)

	try:
		for i , l in enumerate(final):
			pass
		print '\nSites Found : ' , i + 1
	except:
		pass

except IndexError:
	pass
