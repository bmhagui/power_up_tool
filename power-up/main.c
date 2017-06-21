#include "libpower.h"

int main(int argc, char *argv[])
{
  check_config();
  
  int length, i = 0, opt= 0, long_index =0, exists =0;
  char buffer[EVENT_BUF_LEN];
  time_t first_refresh, second_refresh, first_stop, second_stop;

  static struct option long_options[] = {
    {"help",        no_argument, 0,  'h' },
    {"add-refresh", no_argument, 0,  'r' },
    {"add-black",   no_argument, 0,  'b' },
    {"kill",        no_argument, 0,  'k' },
    {"list",        no_argument, 0,  'l' },
    {"toggle-black",no_argument, 0,  't' },
    {    0,             0,       0,   0  }
  };

  while ((opt = getopt_long(argc, argv,"hrbklt", long_options, &long_index )) != -1) {
    switch (opt) {
    case 'h' :
      print_usage();
      exit (0);
    case 'r' :
      system("ps -e | grep `xprop _NET_WM_PID | cut -f3 -d' '` | sed -e 's/[0-9]*//' | sed -e 's/\\ .*\\ //' >> ~/.config/power-up/refresh_list.conf");
      exit(0);
    case 'b' :
      system("ps -e | grep `xprop _NET_WM_PID | cut -f3 -d' '` | sed -e 's/[0-9]*//' | sed -e 's/\\ .*\\ //' >> ~/.config/power-up/black_list.conf");
      exit(0);
    case 'k' :
      system("kill -15 `ps -e | grep latest | cut -f1 -d' '`");
      exit(0);
    case 'l' :
      printf("Below is a list of currently active applications and their respective PIDS.\n");
      system("wmctrl -l -p");
      exit(0);
    case 't' :
      toggle(exists);
    }
  }
  if (( pipe_popen = popen("xprop -root -spy _NET_ACTIVE_WINDOW | mawk -W interactive '{print $5}' > ~/.config/power-up/notif/window_change.conf", "r")) == NULL)
    {
      perror("popen");
      exit(1);
    }
  
  first_refresh = time(NULL);
  printf("\nLaunched power-up.\n");
  system("bash ~/.config/power-up/get_pid.sh");
  Liste *black_list = create_list(path_black_list_pid);
  Liste *refresh_list = create_list(path_refresh_list_pid);

  action.sa_handler = hand;
  sigaction(SIGINT,&action,NULL);
  sigaction(SIGTERM,&action,NULL);
  
  fd = inotify_init();
  if ( fd < 0 ) {
    perror( "inotify_init" );
  }
  
  wd = inotify_add_watch( fd, path_notif, IN_MODIFY); 
  fp = fopen(path_open_windows,"r");
  if(fp==NULL){
    perror("cannot open file open_windows.conf");
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
	  first_stop = time(NULL);
	  system("xdotool getwindowfocus getwindowpid > ~/.config/power-up/open_windows.conf");
	  system("wmctrl -l -p | cut -f4 -d' ' >> ~/.config/power-up/open_windows.conf");
	  system("bash ~/.config/power-up/get_pid.sh");
	  black_list = create_list(path_black_list_pid);
	  refresh_list = create_list(path_refresh_list_pid);
	  
	  fscanf(fp, "%d", &active_pid);
	  while( !feof(fp)) {
	    fscanf(fp, "%d", &pid); 
	    if (pid == active_pid){
	      kill(pid, SIGCONT);
	    }
	    else if ( !member(pid,black_list) ){
	      second_stop=time(NULL);
	      while (second_stop-first_stop < STOP_AFTER_S){
		second_stop=time(NULL);
	      }
	      kill(pid, SIGSTOP);
	    }
	  }
	}
      }
      i += EVENT_SIZE + event->len;
    }
    rewind(fp);
    second_refresh = time(NULL);
    if (second_refresh-first_refresh >= REFRESH_RATE_S){
      //printf("Waking up chosen processes\n");
      activate_list(refresh_list);
      first_refresh = time(NULL);
    }
  }
}
