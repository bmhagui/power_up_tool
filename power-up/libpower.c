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
    //pclose(pipe_popen);
    exit(0);
  }
  else {
    printf("Erreur de gestion de signal SIGINT ou SIGTERM\n");
    exit(-1);
  }
}

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

Liste *create_list(char *file_path){
  
  Liste *mylist = initialisation();
  
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

void print_usage(void) {
    printf("Usage: ./latest_version [OPTION]\nWith no arguments the application is launched normally.\n\n");
    printf("[OPTION]:\n");
    printf("--add-refresh or -r\n\tadd the next window you click on's PID to the application's refresh list.\n");
    printf("--add-black or -b\n\tadd the next window you click on's PID to the application's black list.\n");
    printf("--kill or -k\n\tbrutally terminate the powerup tool running in the background.\n");
    printf("--list or -l\n\tdisplay a list of open window applications and their PIDs.\n");
}

void check_config(void) {
  
  wordexp("~/.config/power-up/", &expansion, 0);
  strcpy(path_config_powerup, *(expansion.we_wordv));
  wordfree(&expansion);
  strcpy(path_black_list, path_config_powerup);
  strcat(path_black_list, "black_list.conf");
  strcpy(path_black_list_pid, path_config_powerup);
  strcat(path_black_list_pid, "black_list_pid.conf");
  strcpy(path_refresh_list, path_config_powerup);
  strcat(path_refresh_list, "refresh_list.conf");
  strcpy(path_refresh_list_pid, path_config_powerup);
  strcat(path_refresh_list_pid, "refresh_list_pid.conf");
  strcpy(path_open_windows, path_config_powerup);
  strcat(path_open_windows, "open_windows.conf");
  strcpy(path_window_change, path_config_powerup);
  strcat(path_window_change, "notif/window_change.conf");
  strcpy(path_notif, path_config_powerup);
  strcat(path_notif, "notif/");
  
  dir = opendir(path_config_powerup);
  if (dir == NULL){
    /* Error */
    if (ENOENT == errno){
      printf("Config folder does not exist, creating now.\n"); 
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
    printf("Config folder in place.\n");
    closedir(dir);
  }
  
  printf("Checking config files and creating them if needed.\n");
  system("cp `sudo find ~ -name \"get_pid.sh\" -user $USER | grep bin/get_pid.sh` -uv ~/.config/power-up");
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
  //wordfree(&expansion);
  printf("--Done--\n\n");
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
      exit(0);
}
