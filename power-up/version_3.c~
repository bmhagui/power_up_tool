/*#include "/home/bhagui/stage_52/power-up/xdotool-master/xdo.h"
  #include "/home/bhagui/stage_52/power-up/xdotool-master/xdotool.h"*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

int main( )
{

  int length, i = 0;
  int fd;
  int wd;
  char buffer[EVENT_BUF_LEN];
  pid_t pid, active_pid;
  
  fd = inotify_init();
  if ( fd < 0 ) {
    perror( "inotify_init" );
  }
  
  wd = inotify_add_watch( fd, "/home/bhagui/stage_52/power-up/notif/", IN_MODIFY);
  
  FILE *fp = fopen("/home/bhagui/stage_52/power-up/open_windows.txt","r");
  if(fp==NULL){
    printf("cannot open file.\n");
  }
  
  while(1){
    i=0;
    length = read( fd, buffer, EVENT_BUF_LEN );
    if ( length < 0 ) {
      perror( "read" );
    }  
    
    /*read the change event one by one and process it accordingly.*/
    while ( i < length ) {     struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];     if ( event->len ) {
	if ( event->mask & IN_MODIFY ) {
	  printf( "%s has been modified, changing suspended windows.\n", event->name );

	  system("xdotool getwindowfocus getwindowpid > open_windows.txt");
	  system("wmctrl -l -p | cut -f4 -d' ' >> open_windows.txt");
	  
	  fscanf(fp, "%d", &active_pid);
	  
	  while( !feof(fp)) {
	    fscanf(fp, "%d", &pid); 
	    if (pid == active_pid){
	      kill(pid, SIGCONT);
	    }
	    else{
	      kill(pid, SIGSTOP);
	    }
	  }
	}
      }
      i += EVENT_SIZE + event->len;
    }
    rewind(fp);
  }
  
  inotify_rm_watch( fd, wd );
  close(fd);
  fclose(fp);
}
