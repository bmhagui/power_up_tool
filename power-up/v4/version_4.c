#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )
#define REFRESH_RATE_S 5

struct sigaction action;
pid_t pid, active_pid;
int fd;
int wd;
FILE *fp,*fl,*flp;

void activate_all(){
  rewind(fp);
  while( !feof(fp)) {
    fscanf(fp, "%d", &pid);
    kill(pid, SIGCONT);
  }
}

void hand(int sig)
{
  if (sig == SIGINT){
    printf("\nYou have pressed Ctrl+C.\nActivating all processes, cleaning up and exiting.\n");
    activate_all();
    inotify_rm_watch( fd, wd );
    close(fd);
    fclose(fp);
    exit(0);
  }
  else{
    printf("Erreur de gestion de signal SIGINT\n");
    exit(-1);
  }
}

typedef struct Element Element;
struct Element{
    pid_t pid;
    Element *next;
};

typedef struct Liste Liste;
struct Liste{
    Element *first;
};

void activate_list(Liste *liste){
  if (liste == NULL){
    exit(EXIT_FAILURE);
  }
  Element *actuel = liste->first;
  while (actuel != NULL){
    kill(actuel->pid,SIGCONT);
    actuel = actuel->next;
  }
}

Liste *initialisation(){
  Liste *liste = malloc(sizeof(*liste));
  Element *element = malloc(sizeof(*element));
  if (liste == NULL || element == NULL){
    exit(EXIT_FAILURE);
  }
  element->pid = 0;
  element->next = NULL;
  liste->first = element;
  return liste;
}

void insertion(Liste *liste, pid_t new_pid){
  /* Création du nouvel élément */
  Element *nouveau = malloc(sizeof(*nouveau));

  if (liste == NULL || nouveau == NULL){
    exit(EXIT_FAILURE);
  }

  nouveau->pid = new_pid;
  /* Insertion de l'élément au début de la liste */
  nouveau->next = liste->first;
  liste->first = nouveau;
}

bool member(pid_t cpid, Liste *liste){
  bool res = false;
  if (liste == NULL){
    exit(EXIT_FAILURE);
  }
  Element *actuel = liste->first;
  while (actuel != NULL){
    if (actuel->pid == cpid){
      res = true;
    }
    actuel = actuel->next;
  }
  return res;
}

Liste *create_list(char *file_path){
  
  Liste *mylist = initialisation();

  char src[100], dest[100];
  strcpy(dest, getenv("HOME"));
  strcpy(src, file_path);
  strcat(dest, src);
  
  fl = fopen(dest,"r");
  if(fl==NULL){
    perror("cannot open file conf_pid.txt");
  }

  while( !feof(fl)) {
    fscanf(fl, "%d", &pid);
    insertion(mylist, pid);
  }
  return mylist; 
}

int main()
{ 
  time_t first, second;
  first = time(NULL);
  
  int length, i = 0;
  char buffer[EVENT_BUF_LEN];
  
  system("bash ~/power_up_tool/power-up/v4/config/get_pid.sh");
  Liste *the_list = create_list("/power_up_tool/power-up/v4/config/conf_pid.txt");
  Liste *refresh_list = create_list("/power_up_tool/power-up/v4/config/refresh_pid.txt");
  
  action.sa_handler = hand;
  sigaction(SIGINT,&action,NULL);
  
  fd = inotify_init();
  if ( fd < 0 ) {
    perror( "inotify_init" );
  }

  char src[100], src2[100], notify[100], open_windows[100];
  strcpy(notify, getenv("HOME"));
  strcpy(open_windows, getenv("HOME"));
  strcpy(src, "/power_up_tool/power-up/v4/config/notif/");
  strcpy(src2, "/power_up_tool/power-up/v4/config/open_windows.txt");
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
	  printf( "%s has been modified, changing suspended windows.\n", event->name );

	  system("xdotool getwindowfocus getwindowpid > ~/power_up_tool/power-up/v4/config/open_windows.txt");
	  system("wmctrl -l -p | cut -f4 -d' ' >> ~/power_up_tool/power-up/v4/config/open_windows.txt");
	  system("bash ~/power_up_tool/power-up/v4/config/get_pid.sh");
	  the_list = create_list("/power_up_tool/power-up/v4/config/conf_pid.txt");
	  refresh_list = create_list("/power_up_tool/power-up/v4/config/refresh_pid.txt");
	  
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
      printf("Waking up chosen processes\n");
      activate_list(refresh_list);
      first = time(NULL);
    }
  }
}
