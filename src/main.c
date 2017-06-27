#include "libpower.h"

int main(int argc, char *argv[])
{
  action.sa_handler = hand;
  sigaction(SIGINT,&action,NULL);
  sigaction(SIGTERM,&action,NULL);
  check_paths();
  int length, i = 0, opt= 0, long_index =0, exists =0, verbose=0;
  char buffer[EVENT_BUF_LEN];
  time_t first_refresh, second_refresh, first_stop, second_stop;

  static struct option long_options[] = {
    {"help",                no_argument, 0,  'h' },
    {"refresh-list",        optional_argument, 0,  'r' },
    {"black-list",          optional_argument, 0,  'b' },
    {"kill-power-up",       no_argument, 0,  'k' },
    {"list-apps",           no_argument, 0,  'l' },
    {"toggle-active-window",no_argument, 0,  't' },
    {"wait-for",      required_argument, 0,  'w' },
    {"configure-pause-and-refresh-rates",      no_argument, 0,  'c' },
    {"verbose",             no_argument, 0,  'v' },
    {    0,                     0,       0,   0  }
  };

  while ((opt = getopt_long(argc, argv,"hrbkltwcv", long_options, &long_index )) != -1) {
    switch (opt) {
    case 'h' :
      print_usage();
      exit (0);
    case 'r' :
      if (argc > 2){
	check=fopen(path_refresh_list,"a+");
	for (i=2;i<argc;i++){
	  add_to_list(argv[i],check,exists);
	}
      }
      else{
	if (( pipe_popen = popen("ps -e | grep `xprop _NET_WM_PID | cut -f3 -d' '` | sed -e 's/[0-9]*//' | sed -e 's/\\ .*\\ //'", "r")) == NULL)
	  {
	    perror("popen");
	    exit(1);
	  }
	fscanf(pipe_popen,"%s",app_name);
	pclose(pipe_popen);
	check=fopen(path_refresh_list,"a+");
	add_to_list(app_name,check,exists);
      }
      exit(0);
    case 'b' :
      if (argc > 2){
	check=fopen(path_black_list,"a+");
	for (i=2;i<argc;i++){
	  add_to_list(argv[i],check,exists);
	}
      }
      else{
	if (( pipe_popen = popen("ps -e | grep `xprop _NET_WM_PID | cut -f3 -d' '` | sed -e 's/[0-9]*//' | sed -e 's/\\ .*\\ //'", "r")) == NULL)
	  {
	    perror("popen");
	    exit(1);
	  }
	fscanf(pipe_popen,"%s",app_name);
	pclose(pipe_popen);
	check=fopen(path_black_list,"a+");
	add_to_list(app_name,check,exists);
      }
      exit(0);
    case 'k' :
      //system("kill `pgrep power_up`");
      system("pkill -SIGINT power_up");
      exit(0);
    case 'l' :
      printf("Below is a list of currently active applications and their respective PIDS.\n\nPID\tName\n");
      system("wmctrl -l -p | cut -f4 -d' '> $XDG_RUNTIME_DIR/open_windows.conf");
      system("ps -e | grep -f $XDG_RUNTIME_DIR/open_windows.conf | awk '{print $1,$4}'");      
      exit(0);
    case 't' :
      toggle(exists);
    case 'w' :
      i = strtol(optarg, NULL, 10);
      system("pkill -SIGINT power_up");
      sleep(i);
      break;
    case 'c' :
      if (( pipe_popen = popen("sudo find ~ -name \"libpower.h\" | grep \"/src/libpower.h\"", "r")) == NULL)
	{
	  perror("popen");
	  exit(1);
	}
      fscanf(pipe_popen,"%s",path_libpowerh);
      printf("%s\n",path_libpowerh);
      
      if (( fp = fopen(path_libpowerh, "r+")) == NULL){
	perror("fopen");
	exit(1);
      }
      
      fscanf(fp,"%s",read_name);
      rewind(fp);
      printf("After how many seconds would you like to pause your applications?\n");
      scanf("%d",&i);
      while(i>9 || i<0){
	printf("Answer MUST be between 1 and 10 seconds.\n");
	scanf("%d",&i);
      }
      
      fprintf(fp,"#define STOP_AFTER_S %d\n",i);
      printf("At what frequencey would you like to refresh your refresh-list applications?\n");
      scanf("%d",&i);
      while(i>9 || i<0){
	printf("Answer MUST be between 1 and 10 seconds.\n");
	scanf("%d",&i);
      }
      fprintf(fp,"#define REFRESH_RATE_S %d\n",i);
      exit(0);
    case 'v' :
      verbose=1;
      break;
    default:
      printf("\nInvalid argument. Refer to --help or -h for valid options.\n");
      exit(0);
    }
  }

  running_check();
  
  if (( pipe_popen = popen("xprop -root -spy _NET_ACTIVE_WINDOW | mawk -W interactive '{print $5}' > ~/.config/power_up/notif/window_change.conf", "w")) == NULL)
    {
      perror("popen");
      exit(1);
    }
  
  first_refresh = time(NULL);
  printf("\nLaunched power_up.\n");
  system("bash ~/.config/power_up/get_pid.sh");
  Liste *black_list = create_list(path_black_list_pid);
  Liste *refresh_list = create_list(path_refresh_list_pid);
  
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
    
    while ( i < length ) {     struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];     if ( event->len ) {
	if ( event->mask & IN_MODIFY ) {
	  first_stop = time(NULL);
	  system("xdotool getwindowfocus getwindowpid > $XDG_RUNTIME_DIR/open_windows.conf");
	  system("wmctrl -l -p | cut -f4 -d' ' >> $XDG_RUNTIME_DIR/open_windows.conf"); 
	  system("bash ~/.config/power_up/get_pid.sh");
	  black_list = create_list(path_black_list_pid);
	  refresh_list = create_list(path_refresh_list_pid);
	  
	  fscanf(fp, "%d", &active_pid);
	  while( !feof(fp)) {
	    fscanf(fp, "%d", &pid); 
	    if (pid == active_pid){
	      kill(pid, SIGCONT);
	      if(verbose ==1){
		printf("%d Continued\n",pid);
	      }
	    }
	    else if ( !member(pid,black_list) ){
	      second_stop=time(NULL);
	      while (second_stop-first_stop < STOP_AFTER_S){
		second_stop=time(NULL);
	      }
	      kill(pid, SIGSTOP);
	      if(verbose ==1){
		printf("%d Paused\n",pid);
	      }
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
