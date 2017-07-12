#include "libpower.h"

void activate_all(void){
  fp = fopen(path_open_windows,"r");
  if(fp==NULL){
    perror("cannot open file open_windows.conf");
  }
  while(fscanf(fp, "%d", &pid)>0) {
    kill(pid, SIGCONT);
  }
  fclose(fp);
  fp = fopen(path_refresh_list_pid,"r");
  if(fp==NULL){
    perror("cannot open file open_windows.conf");
  }
  while(fscanf(fp, "%d", &pid)>0) {
    kill(pid, SIGCONT);
  }
  fclose(fp);
}

void hand(int sig)
{
  if (sig == SIGINT || sig == SIGTERM){
    printf("\nYou have pressed Ctrl+C.\nActivating all processes, cleaning up and exiting.\n");
    activate_all();
    inotify_rm_watch( fd, wd );
    close(fd);
    system("pkill -SIGTERM xprop");
    exit(0);
  }
  else {
    printf("Erreur de gestion de signal SIGINT ou SIGTERM\n");
    exit(-1);
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
  
  //system cp
  check = fopen(path_time,"a+");
  if(fscanf(check,"%s",tmp)<=0){
    fprintf(check,"STOP_AFTER_S 0\nREFRESH_RATE_S 60\nREFRESH_FOR_S 5\n");
  }
  if( check != NULL){
    fclose(check);
  }
  check = fopen(path_black_list,"a+");
  if( check != NULL){
    fclose(check);
  }
  check = fopen(path_refresh_list,"a+");
  if( check != NULL){
    fclose(check);
  }
  
  system("> $XDG_RUNTIME_DIR/black_list_pid.conf");
  system("> $XDG_RUNTIME_DIR/refresh_list_pid.conf");
  /*check = fopen(path_black_list_pid,"a+");
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
    }*/
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
  if (list == NULL || fp==NULL){
    exit(EXIT_FAILURE);
  }
  list->refresh_active=false;
  list->first = NULL;
  list->count_procs=0;

  return list;
}

void equal_count(Stop_list *list, pid_t new_active_pid, time_t STOP_AFTER_S, bool verbose_bool){
  if (list == NULL){
    exit(EXIT_FAILURE);
  }
  Proc *pt_process=list->first;
  time_t time_now;
  while (pt_process!=NULL){
    if (pt_process->pid == new_active_pid){
      pt_process->paused = false;
      pt_process->time_added = time(NULL);
    }
    if (pt_process->paused == false && pt_process->pid != new_active_pid){
      time_now=time(NULL);
      if(time_now-pt_process->time_added >= STOP_AFTER_S){
	kill(pt_process->pid, SIGSTOP);
	pt_process->paused = true;
	if (verbose_bool){
	  sprintf(verbose,"ps -e | grep %d | awk '{print $4}'",pt_process->pid);
	  system(verbose);
	  printf("has been paused\n\n");
	}
      }
    }
    
    pt_process = pt_process->next;
  } 
}

void affiche_stop_liste(Stop_list *list){
  Proc *pt_process=list->first;
  printf("Processed to be stopped:\n");
  while (pt_process!=NULL){
    printf("Element: %d, paused: %d\n",pt_process->pid, pt_process->paused);
    pt_process = pt_process->next;
  }
  printf("Number of processes in this list: %d\n",list->count_procs);
  printf("--------------------------\n");
}

void diff_count(Stop_list *list, FILE *fp, pid_t new_active_pid, time_t STOP_AFTER_S, bool verbose_bool){
  if (list == NULL){
    exit(EXIT_FAILURE);
  }
  int scanned_pid;
  bool found = false;
  Proc *pt_process=list->first;
  time_t time_now;

  while (pt_process!=NULL){
    if (pt_process->pid == new_active_pid){
      pt_process->paused = false;
      pt_process->time_added = time(NULL);
    }    
    pt_process = pt_process->next;
  }
  
  fp = fopen(path_open_windows,"r");
  if(fp==NULL){
    perror("cannot open file open_windows.conf");
  }
  while (fscanf(fp, "%d", &scanned_pid)>0){
    pt_process=list->first;
    found=false;
    while (pt_process!=NULL && found==false){
      if (pt_process->pid == scanned_pid){
        found = true;
      }
      else{
	pt_process = pt_process->next;
      }
    }
    if (found == false){
      Proc *process = malloc(sizeof(*process));
      process->next=list->first;
      process->pid = scanned_pid;
      process->time_added=time(NULL);
      process->paused=false;
      list->first = process;
      list->count_procs++;
    }
  }
  
  pt_process=list->first;
  while (pt_process!=NULL){
    if (pt_process->paused == false && pt_process->pid != new_active_pid){
      time_now=time(NULL);
      if(time_now-pt_process->time_added >= STOP_AFTER_S){
	kill(pt_process->pid, SIGSTOP);
	pt_process->paused = true;
	if (verbose_bool){
	  sprintf(verbose,"ps -e | grep %d | awk '{print $4}'",pt_process->pid);
	  system(verbose);
	  printf("has been paused\n\n");
	}
      }
    }  
    pt_process = pt_process->next;
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

void delete_unused_pid(Stop_list *list){
  if (list == NULL){
    exit(EXIT_FAILURE);
  }
  
  while (kill(list->first->pid,0) != 0){
    Proc *pt_delete;
    pt_delete = list->first;
    list->first=list->first->next;
    free(pt_delete);
    list->count_procs--;
  }

  Proc *pt_previous=list->first, *pt_process=list->first->next;
  while (pt_process!=NULL){
    if (kill(pt_process->pid,0) != 0){
      Proc *pt_delete = pt_process;
      pt_previous->next = pt_process->next;
      free(pt_delete);
      pt_process=pt_previous->next;
      list->count_procs--;
    }
    else{
      pt_previous = pt_previous->next;
      pt_process = pt_previous->next;
    }
  }
}

void get_pid(void){
  FILE *black, *refresh;
  char get_pid[100],name[100];
  black = fopen(path_black_list,"r");
  refresh = fopen(path_refresh_list,"r");
  
  while (fscanf(black,"%s",name)>0){
    sprintf(get_pid,"pgrep %s > $XDG_RUNTIME_DIR/black_list_pid.conf",name);
    system(get_pid);
  }
  fclose(black);
  
  while (fscanf(refresh,"%s",name)>0){
    sprintf(get_pid,"pgrep %s > $XDG_RUNTIME_DIR/refresh_list_pid.conf",name);
    system(get_pid);
  }
  fclose(refresh);
}

void handle_applications(int STOP_AFTER_S,int REFRESH_RATE_S, int REFRESH_FOR_S, int count, bool verbose_bool){
  old_active_pid = new_active_pid;
  if (( pipe_wc = popen("xdotool getwindowfocus getwindowpid", "r")) == NULL){
    perror("popen");
    exit(1);
  }
  fscanf(pipe_wc,"%d",&new_active_pid);
  pclose(pipe_wc);
  kill(new_active_pid, SIGCONT);
  if (verbose_bool && new_active_pid!=old_active_pid){
    sprintf(verbose,"ps -e | grep %d | awk '{print $4}'",new_active_pid);
    system(verbose);
    printf("is active.\n\n");
  }
  
  get_pid();
  system("wmctrl -l -p | grep -vf $XDG_RUNTIME_DIR/black_list_pid.conf | grep -vf $XDG_RUNTIME_DIR/refresh_list_pid.conf | awk '{print $2,$3}' | grep -v - | awk '{print $2}' | sort -u -b > $XDG_RUNTIME_DIR/open_windows.conf");
  
  //STOP
  if (( pipe_wc = popen("grep -cve '^\\s*$' $XDG_RUNTIME_DIR/open_windows.conf", "r")) == NULL){
    perror("popen");
    exit(1);
  }
  fscanf(pipe_wc,"%d",&count);
  pclose(pipe_wc);
  
  //affiche_stop_liste(stop_list);
  if (count != 0){
    if (count==stop_list->count_procs){
      equal_count(stop_list, new_active_pid, STOP_AFTER_S, verbose_bool);
    }
    else{
      if (stop_list->count_procs > count){
	delete_unused_pid(stop_list);
      }
      else{
	diff_count(stop_list, fp, new_active_pid, STOP_AFTER_S, verbose_bool);
      }
    }
  }
  //Refreshing if needed here;
  //printf("Refresh_active: %d\n",);
  time_now=time(NULL);
  if (time_now-changed_refresh_status > REFRESH_RATE_S && !stop_list->refresh_active){
    refresh_fp = fopen(path_refresh_list_pid,"r");
    if(refresh_fp==NULL){
      perror("cannot open file refresh_list_pid");
    }
    //printf("Activating refresh list\n");
    while (fscanf(refresh_fp, "%d", &pid)>0){
      kill(pid,SIGCONT);
    }
    stop_list->refresh_active=true;
    changed_refresh_status = time(NULL);
    fclose(refresh_fp);
  }
  else if(time_now-changed_refresh_status > REFRESH_FOR_S && stop_list->refresh_active){
    refresh_fp = fopen(path_refresh_list_pid,"r");
    if(refresh_fp==NULL){
      perror("cannot open file refresh_list_pid");
    }
    //printf("Pausing refresh list\n");
    while (fscanf(refresh_fp, "%d", &pid)>0){
      if (pid!=new_active_pid){
	kill(pid,SIGSTOP);
      }
    }
    stop_list->refresh_active=false;
    changed_refresh_status = time(NULL);
    fclose(refresh_fp);
  }
  //refresh over
}
