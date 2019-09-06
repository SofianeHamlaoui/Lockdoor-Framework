import socket
from urlparse import urlparse
import  time, urllib2, re,  httplib
#Coded By Ph DZ
#Edit By Ph Dz

print 'Com_jce Server Scanner ~~ Edit By Ph Dz'
file2=open('jce.txt','a')
 
 
 
def check(site) :
 try  :
 
  w  = urllib2.urlopen(site).read()
 except urllib2.URLError, (err):
              pass
 except socket.error , v :
            pass
 except urllib2.HTTPError, err:
          pass
 except IOError, e:
             pass
 except httplib.IncompleteRead ,e:
  pass
 else :
   if re.findall('Image Manager',w) :
    ox=  urlparse(site)
    print 'Ph Dz! ! ! Com_jce Found In   =>       ' + ox[1]
    file2.write(ox[1]+'\n')
   
def xlol(site):
        try:
           
            urllib2.urlopen(site)
 
        except urllib2.URLError, (err):
              pass
        except socket.error , v :
            pass
        except urllib2.HTTPError, err:
          pass
        except IOError, e:
             pass
        else:
            check(site)
def bing_it(ip):
 
     page  = 0
     while(page <= 200):
      try :
          bing      = "http://www.bing.com/search?q=ip%3A"+ip+"+index.php?option=com_&first="+str(page)
 
          openbing  = urllib2.urlopen(bing)
 
          readbing = openbing.read()
          findbing = re.findall('<div class="sb_tlst"><h3><a href="(.*?)" h=',readbing)
 
          for i in range(len(findbing)):
              x=findbing[i]
              global o
              o=  urlparse(x)
              y = o[2].replace('/index.php','')
              print 'checking ' + o[1]
              check('http://'+o[1]+y+'/index.php?option=com_jce&task=plugin&plugin=imgmanager&file=imgmanager&version=1576&cid=20')
             
 #Check 1-1
          page = page + 10
      except httplib.IncompleteRead ,e:
        pass  
      except urllib2.URLError, (err):
              pass
      except socket.error , v :
            pass
      except urllib2.HTTPError, err:
          pass
      except IOError, e:
             pass
 
 #Check l1
Xip = raw_input('IP :')
 
bing_it(Xip)
