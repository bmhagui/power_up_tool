#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

int main( )
{

  int length, i = 0;
  int fd;
  int wd;
  char buffer[EVENT_BUF_LEN];
  char str1[10];
  
  fd = inotify_init();
  /*checking for error*/
  if ( fd < 0 ) {
    perror( "inotify_init" );
  }
  
  wd = inotify_add_watch( fd, "/home/bhagui/stage_52/power-up/notif/", IN_MODIFY);
  /* FILE *fp = fopen("/home/bhagui/stage_52/power-up/notif/test.txt","r");
  if(fp==NULL){
    printf("cannot open file.\n");
    }*/
  
  while(1){
    i=0;
    length = read( fd, buffer, EVENT_BUF_LEN );
    /*checking for error*/
    if ( length < 0 ) {
      perror( "read" );
    }  
    
    /*read the change event one by one and process it accordingly.*/
    while ( i < length ) {     struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];     if ( event->len ) {
	if ( event->mask & IN_MODIFY ) {
	  printf( "The file  %s has been modified.\n", event->name );
	  /*while( !feof(fp)) { fscanf(fp, "%s", str1); }
	    printf("Read String1 |%s|\n", str1 );
	    int pid = execlp("xdotool","xdotool", "getwindowpid",str1, NULL);
	    printf("is");doesnt print this i have no idea why
	    printf("the pid is: %d",pid);*/
	  char a = system("wmctrl -l -p | cut -f4 -d' ' > active_windows.txt");
	  printf("hghghghghg %s\n",a);
	  system("/home/bhagui/stage_52/power-up/active.sh");
	}
	
      }
      i += EVENT_SIZE + event->len;
    }
  }
  inotify_rm_watch( fd, wd );
  /*closing the INOTIFY instance*/
  close(fd);
  
}
