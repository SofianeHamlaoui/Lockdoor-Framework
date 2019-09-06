


if (spike_send_tcp(target,port)==0)
    {
      printf("Couldn't connect to host or send data!\r\n");
      /*exit(-1);*/
    }


 /*see, the thing is that the spike is not guaranteed to be
            null terminated, so just a plain printf on the
            s_get_databuf() is ill-advised.*/
	  memset(buffer,0x00,sizeof(buffer));
	  if (s_get_size()>1500)
	    memcpy(buffer,s_get_databuf(),1500);
	  else
	    memcpy(buffer,s_get_databuf(),s_get_size());

/*here we print out our request*/
	  printf("Request:\n%.1500s\nEndRequest\n",buffer);
first=1;
notfin=1;
retval=1;
  while(retval && notfin) 
  {
    memset(buffer,0x00,sizeof(buffer));
    notfin=s_fd_wait();
    if (!notfin)
      break;
    retval=read(our_spike->fd,buffer,2500);
    if (first && (retval==-1 || retval==0))
     {
	printf("***Server closed connection!\n");
     }
    first=0;
    if (retval)
      {
	printf("%.500s",buffer);
      }
  }/*end while read loop*/
s_incrementfuzzstring();
spike_close_tcp();
/*Use this for testing against netcat*/
  /*
    sleep(1);
  */
}/*end for each fuzz string*/
s_incrementfuzzvariable();
}/*end for each variable*/
printf("Done.\n");
  return 0;
} /*end program*/

