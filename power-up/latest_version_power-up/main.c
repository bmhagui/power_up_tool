#include "libpower.h"

int main()
{ 
  time_t first, second;
  first = time(NULL);
  
  int length, i = 0;
  char buffer[EVENT_BUF_LEN];
  
  system("bash ~/power_up_tool/power-up/latest_version_power-up/config/get_pid.sh");
  Liste *the_list = create_list("/power_up_tool/power-up/latest_version_power-up/config/conf_pid.txt");
  Liste *refresh_list = create_list("/power_up_tool/power-up/latest_version_power-up/config/refresh_pid.txt");
  
  action.sa_handler = hand;
  sigaction(SIGINT,&action,NULL);
  
  fd = inotify_init();
  if ( fd < 0 ) {
    perror( "inotify_init" );
  }

  char src[100], src2[100], notify[100], open_windows[100];
  strcpy(notify, getenv("HOME"));
  strcpy(open_windows, getenv("HOME"));
  strcpy(src, "/power_up_tool/power-up/latest_version_power-up/config/notif/");
  strcpy(src2, "/power_up_tool/power-up/latest_version_power-up/config/open_windows.txt");
  strcat(notify, src);
  strcat(open_windows, src2);
  
  wd = inotify_add_watch( fd, notify, IN_MODIFY); 
  fp = fopen(open_windows,"r");
  if(fp==NULL){
    perror("cannot open file open_windows.txt");
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
	  //printf( "%s has been modified, changing suspended windows.\n", event->name );

	  system("xdotool getwindowfocus getwindowpid > ~/power_up_tool/power-up/latest_version_power-up/config/open_windows.txt");
	  system("wmctrl -l -p | cut -f4 -d' ' >> ~/power_up_tool/power-up/latest_version_power-up/config/open_windows.txt");
	  system("bash ~/power_up_tool/power-up/latest_version_power-up/config/get_pid.sh");
	  the_list = create_list("/power_up_tool/power-up/latest_version_power-up/config/conf_pid.txt");
	  refresh_list = create_list("/power_up_tool/power-up/latest_version_power-up/config/refresh_pid.txt");
	  
	  fscanf(fp, "%d", &active_pid);
	  while( !feof(fp)) {
	    fscanf(fp, "%d", &pid); 
	    if (pid == active_pid){
	      kill(pid, SIGCONT);
	    }
	    else if ( !member(pid,the_list) ){ 
	      kill(pid, SIGSTOP);
	    }
	  }
	}
      }
      i += EVENT_SIZE + event->len;
    }
    rewind(fp);
    second = time(NULL);
    if (second-first >= REFRESH_RATE_S){
      //printf("Waking up chosen processes\n");
      activate_list(refresh_list);
      first = time(NULL);
    }
  }
}
