#include "libpower.h"

void activate_all(void){
  rewind(fp);
  while( !feof(fp)) {
    fscanf(fp, "%d", &pid);
    kill(pid, SIGCONT);
  }
}

void hand(int sig)
{
  if (sig == SIGINT || sig == SIGTERM){
    printf("\nYou have pressed Ctrl+C.\nActivating all processes, cleaning up and exiting.\n");
    activate_all();
    inotify_rm_watch( fd, wd );
    close(fd);
    fclose(fp);
    delete_stop_list(stop_list);
    delete_list(black_list);
    delete_list(refresh_list);
    free(black_list);
    free(refresh_list);
    
    system("pkill -SIGTERM xprop");
    exit(0);
  }
  else {
    printf("Erreur de gestion de signal SIGINT ou SIGTERM\n");
    exit(-1);
  }
}

void activate_list(Liste *liste, Stop_list *list){
  if (liste == NULL || list == NULL){
    exit(EXIT_FAILURE);
  }
  if (liste->first != NULL){
    Element *actuel = liste->first;
    Proc *process;
    
    while (actuel != NULL){
      kill(actuel->pid,SIGCONT);
      process = list->first;
      while( (process->pid != actuel->pid) && process!=NULL){
	process = process->next;
      }
      if (process->pid == actuel->pid){
	process->paused=false;
      }
      actuel = actuel->next;
    }
  }
}

Liste *initialisation(void){
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

Liste *create_list(char *file_path, Liste *mylist){
    
  fl = fopen(file_path,"r");
  if(fl==NULL){
    perror("create_list error");
  }

  while( !feof(fl)) {
    fscanf(fl, "%d", &pid);
    insertion(mylist, pid);
  }
  return mylist; 
}

void delete_list(Liste *liste){
   if (liste == NULL){
        exit(EXIT_FAILURE);
    }
    while (liste->first != NULL){
        Element *aSupprimer = liste->first;
        liste->first = liste->first->next;
        free(aSupprimer);
    }
}

void print_usage(void) {
    printf("Usage: ./power_up [OPTION]\nWith no arguments the application is launched normally.\n\n");
    printf("[OPTION]:\n");
    printf("--refresh-list or -r\n\tadd the next window you click on's PID to the application's refresh list.\n");
    printf("--black-list or -b\n\tadd the next window you click on's PID to the application's black list.\n");
    printf("--kill-power-up or --k\n\tTerminate the powerup tool currently running.\n");
    printf("--list-apps or -l\n\tdisplay a list of open window applications and their PIDs.\n");
    printf("--toggle-active-window or -t\n\tAdd the currently open application to the blacklist or remove it if it is already on there.\n");
    printf("--wait-for <seconds> or --w\n\tWaits the entered number of seconds before launching the app.\n\tIf the app is alraedy running, this will stop the app and re-run it in the second terminal after <seconds>.\n");
    printf("--configure-pause-and-refresh-rates or -c\n\tChoose after how many seconds to pause the background apps and the frequence of refreshing your refresh-list apps.\n\tAnswers must be between 0 and 10 seconds.\n");
    printf("--verbose or -v\n\tLaunch the app and print out the PID of each application and whether it was paused or continued.\n");
}

void check_paths(void) {
  
  wordexp("~/.config/power_up/", &expansion, 0);
  strcpy(path_config_powerup, *(expansion.we_wordv));
  wordfree(&expansion);

  wordexp("$XDG_RUNTIME_DIR/", &expansion, 0);
  strcpy(path_runtime_dir, *(expansion.we_wordv));
  wordfree(&expansion);

  strcpy(path_time, path_config_powerup);
  strcat(path_time, "time.conf");
  
  strcpy(path_black_list, path_config_powerup);
  strcat(path_black_list, "black_list.conf");
  
  strcpy(path_black_list_pid, path_runtime_dir);
  strcat(path_black_list_pid, "black_list_pid.conf");
  
  strcpy(path_refresh_list, path_config_powerup);
  strcat(path_refresh_list, "refresh_list.conf");
  
  strcpy(path_refresh_list_pid, path_runtime_dir);
  strcat(path_refresh_list_pid, "refresh_list_pid.conf");
  
  strcpy(path_open_windows, path_runtime_dir);
  strcat(path_open_windows, "open_windows.conf");
  
  strcpy(path_window_change, path_config_powerup);
  strcat(path_window_change, "notif/window_change.conf");
  
  strcpy(path_notif, path_config_powerup);
  strcat(path_notif, "notif/");
  
  dir = opendir(path_notif);
  if (dir == NULL){
    /* Error */
    if (ENOENT == errno){
      //printf("Config folder missing, creating now.\n"); 
      if (mkdir(path_config_powerup, 00700) < 0){
	perror("mkdir error config_powerup");
      }
      if (mkdir(path_notif, 00700) < 0){
	perror("mkdir error notif");
      }
    }
    else{ 
      perror("opendir error:");
    }
  }
  else{
    closedir(dir);
  }

  /*sprintf(tmp,"%d",getpid());
  strcpy(get_pid_command,"stat /proc/");
  strcat(get_pid_command,tmp);
  strcat(get_pid_command,"/exe | grep /home/ | sed 's/.*-> //' | sed 's/[a-z]*_[a-z]*$//'");*/
  pid=getpid();
  sprintf(get_pid_command,"stat /proc/%d/exe | grep /home/ | sed 's/.*-> //' | sed 's/[a-z]*_[a-z]*$//'",pid);
  //printf("%s\n",get_pid_command);
  
  if (( pipe_popen = popen(get_pid_command, "r")) == NULL)
    {
      perror("popen");
      exit(1);
    }
  fscanf(pipe_popen,"%s",tmp);
  pclose(pipe_popen);
  /*strcpy(get_pid_command, "cp -u ");
  strcat(get_pid_command,tmp);
  strcat(get_pid_command,"bin/get_pid.sh ~/.config/power_up\n");*/
  sprintf(get_pid_command,"cp -u %sbin/get_pid.sh ~/.config/power_up",tmp);
  //printf("%s\n",get_pid_command);
  system(get_pid_command);
  
  //system cp
  check = fopen(path_time,"a+");
  if(fscanf(check,"%s",tmp)<=0){
    fprintf(check,"STOP_AFTER_S 0\nREFRESH_RATE_S 60\n");
    printf("EMPTY\n");
  }
  if( check != NULL){
    fclose(check);
  }
  check = fopen(path_black_list,"a+");
  if( check != NULL){
    fclose(check);
  }
  check = fopen(path_black_list_pid,"a+");
  if( check != NULL){
    fclose(check);
  }
  check = fopen(path_refresh_list,"a+");
  if( check != NULL){
    fclose(check);
  }
  check = fopen(path_refresh_list_pid,"a+");
  if( check != NULL){
    fclose(check);
  }
  check = fopen(path_open_windows,"a+");
  if( check != NULL){
    fclose(check);
  }
  check = fopen(path_window_change,"a+");
  if( check != NULL){
    fclose(check);
  }
}

Liste_toggle *init_toggle(void){
    Liste_toggle *liste = malloc(sizeof(*liste));
    Element_toggle *element = malloc(sizeof(*element));
    if (liste == NULL || element == NULL)
    {
        exit(EXIT_FAILURE);
    }
    strcpy(element->name, "");
    element->suivant = NULL;
    liste->premier = element;
    return liste;
}

void insertion_toggle(Liste_toggle *liste, char name[]){
    /* Création du nouvel élément */
    Element_toggle *nouveau = malloc(sizeof(*nouveau));
    if (liste == NULL || nouveau == NULL)    {
        exit(EXIT_FAILURE);
    }
    strcpy(nouveau->name, name);
    /* Insertion de l'élément au début de la liste */
    nouveau->suivant = liste->premier;
    liste->premier = nouveau;
}

void delete_toggle(Liste_toggle *liste){
    if (liste == NULL){
        exit(EXIT_FAILURE);
    }
    while (liste->premier != NULL){
        Element_toggle *aSupprimer = liste->premier;
        liste->premier = liste->premier->suivant;
        free(aSupprimer);
    }
    free(liste);
}

void rewrite_file(Liste_toggle *liste, FILE *fp){
  if (liste == NULL)
    {
      exit(EXIT_FAILURE);
    }
  Element_toggle *actuel = liste->premier;
  while (actuel != NULL)
    {
      if (strlen(actuel->name)!=0){
	fprintf(fp,"%s\n", actuel->name);
      }
      actuel = actuel->suivant;
    }
}

void toggle(int exists){
  /* Create one way pipe line with call to popen() */
      if (( pipe_popen = popen("ps -e | grep `xdotool getactivewindow getwindowpid` | sed -e 's/[0-9]*//' | sed -e 's/\\ .*\\ //'", "r")) == NULL)
	{
	  perror("popen");
	  exit(1);
	}
      fgets(app_name,100,pipe_popen);
      pclose(pipe_popen);
  
      check = fopen(path_black_list,"r");
      if(check==NULL){
	perror("cannot open file open_windows.conf");
      }
      Liste_toggle *maListe = init_toggle();
      while(fscanf(check, "%s", read_name)>0){
	//fgets add a newline
	//printf("app_name: %d\nread_name: %d\n",strlen(app_name),strlen(read_name));
	if(strncmp(app_name,read_name,strlen(read_name))!=0){
	  insertion_toggle(maListe, read_name);
	}
	else{
	  exists=1;
	}
      }
      if (exists==0){
	check = fopen(path_black_list,"a");
	fprintf(check,"%s",app_name);
	//printf("%s",app_name);
	fclose(check);
      }
      else {
	if (truncate(path_black_list, 0) == -1){
	  perror("Could not truncate");
	}
	check = fopen(path_black_list,"a");
	rewrite_file(maListe,check);
      }
      delete_toggle(maListe);
}

void running_check(void){
  if (( pipe_popen = popen("pgrep power_up -c", "r")) == NULL){
    perror("popen");
    exit(1);
  }
  fscanf(pipe_popen,"%d",&fd);
  if (fd>1){
    printf("ERROR: Power-up is already running.\n");
    exit(1);
  }
}

void add_to_list(char * app_name, FILE *fp, int exists){
  if (fp==NULL){
    perror("add_to_list pointer is non valid");
  }
  exists=0;
  while(fscanf(fp, "%s", read_name)>0){
    if(strncmp(app_name,read_name,strlen(read_name))==0){
      exists=1;
    }
  }
  if (exists==0){
    fprintf(fp,"%s\n",app_name);
  }
  else{
    printf("%s is already on the list chosen to add to.\n", app_name);
  }
}

Stop_list *init_stop_list(FILE *fp){
  Stop_list *list = malloc(sizeof(*list));
  pid_t tmp;
  int i=0;
  if (list == NULL || fp==NULL){
    exit(EXIT_FAILURE);
  }
  list->first = NULL;

  while (fscanf(fp, "%d", &tmp)>0){
    Proc *process = malloc(sizeof(*process));
    process->next=list->first;
    list->first = process;
    process->pid = tmp;
    process->time_added=time(NULL);
    process->paused=false;
    i++;
  }
  list->count_procs=i;
  return list;
}

void add_equal_count(Stop_list *list, pid_t new_active_pid, pid_t old_active_pid){
  Proc *pt_process;
  Proc *pt_previous;
  if (list == NULL){
    exit(EXIT_FAILURE);
  }
  if (old_active_pid != 0 && new_active_pid!=old_active_pid){
    pt_process=list->first;
    if (pt_process->pid == new_active_pid){
      list->first = pt_process->next;
      free(pt_process);
    }
    else{
      while (pt_process->pid != new_active_pid && pt_process->next!=NULL){
	pt_previous = pt_process;
	pt_process = pt_process->next;
      }
      pt_previous->next = pt_process->next;
      free(pt_process);
    }

    pt_previous=list->first;

    while(pt_previous->next!=NULL){
      pt_previous=pt_previous->next;
    }
    Proc *process = malloc(sizeof(*process));
    
    pt_previous->next = process;
    
    process->next=NULL;
    process->pid = old_active_pid;
    process->time_added=time(NULL);
    process->paused=false;
  }
}

void affiche_stop_liste(Stop_list *list){
  Proc *pt_process=list->first;
  printf("Processed to be stopped:\n");
  while (pt_process!=NULL){
    printf("Element: %d\n",pt_process->pid);
    pt_process = pt_process->next;
  }
  printf("--------------------------\n");
}

void add_diff_count(Stop_list *list, pid_t pid, int num){
  Proc *pt_previous;
  if (list == NULL){
    exit(EXIT_FAILURE);
  }
  if (num!=0){
    if (num<list->count_procs){
      while (list->first != NULL){
        Proc *aSupprimer = list->first;
        list->first = list->first->next;
        free(aSupprimer);
      }
      stop_list = init_stop_list(fp);
    }
    else{
      pt_previous=list->first;
      while(pt_previous->next!=NULL){
	pt_previous=pt_previous->next;
      }
      Proc *process = malloc(sizeof(*process));
      
      pt_previous->next = process;
      
      process->next=NULL;
      process->pid = pid;
      process->time_added=time(NULL);
      process->paused=false;
      
      list->count_procs=num;
    }
  }
}
void delete_stop_list(Stop_list *list){
      if (list == NULL){
        exit(EXIT_FAILURE);
    }
    while (list->first != NULL){
        Proc *aSupprimer = list->first;
        list->first = list->first->next;
        free(aSupprimer);
    }
    free(list);
}

void black_listing(FILE *fp, Stop_list *list){
  pid_t tmp;
  int found = 0;
  Proc *pt_process, *pt_previous;
  fp = fopen(path_black_list_pid,"r");
  
  while (fscanf(fp, "%d", &tmp)>0){
    
    pt_process=list->first;
    if (pt_process->pid == tmp){
      found = 1;
      list->first = pt_process->next;    
      free(pt_process);
    }
    else{
      while (pt_process!=NULL && found !=1){
	if (pt_process->pid == tmp){
	  found =1;
	  pt_previous->next = pt_process->next;
	  free(pt_process);
	}
	pt_previous = pt_process;
	pt_process = pt_process->next;
      }
    }
  }
  fclose(fp);
}
