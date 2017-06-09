#include "libpower.h"

int main(int argc, char *argv[])
{
  system("> ~/.config/powerup_config/conf_pid.txt");
  time_t first, second;
  first = time(NULL);
  
  int length, i = 0, opt= 0, long_index =0;
  char buffer[EVENT_BUF_LEN];
  
  static struct option long_options[] = {
    {"help",      no_argument, 0,  'h' },
    {"add-white", no_argument, 0,  'w' },
    {"add-black", no_argument, 0,  'b' },
    {"kill",      no_argument, 0,  'k' },
    {    0,           0,       0,   0  }
  };
  
  
  system("bash ~/.config/powerup_config/get_pid.sh");
  Liste *the_list = create_list("/.config/powerup_config/conf_pid.txt");
  Liste *refresh_list = create_list("/.config/powerup_config/refresh_pid.txt");

  while ((opt = getopt_long(argc, argv,"hwbk", long_options, &long_index )) != -1) {
    switch (opt) {
    case 'h' :
      print_usage();
      exit (0);
    case 'w' :
      printf("Add to white list\n");
      break;
    case 'b' :
      printf("Add to black list\n");
      break;
    case 'k' :
      system("kill `ps -e | grep latest | cut -f1 -d' '`");
      exit(0);
    }
  }
  
  action.sa_handler = hand;
  sigaction(SIGINT,&action,NULL);
  
  fd = inotify_init();
  if ( fd < 0 ) {
    perror( "inotify_init" );
  }

  char src[100], src2[100], notify[100], open_windows[100];
  strcpy(notify, getenv("HOME"));
  strcpy(open_windows, getenv("HOME"));
  strcpy(src, "/.config/powerup_config/notif/");
  strcpy(src2, "/.config/powerup_config/open_windows.txt");
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

	  system("xdotool getwindowfocus getwindowpid > ~/.config/powerup_config/open_windows.txt");
	  system("wmctrl -l -p | cut -f4 -d' ' >> ~/.config/powerup_config/open_windows.txt");
	  system("bash ~/.config/powerup_config/get_pid.sh");
	  the_list = create_list("/.config/powerup_config/conf_pid.txt");
	  refresh_list = create_list("/.config/powerup_config/refresh_pid.txt");
	  
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
