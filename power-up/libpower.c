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

void print_usage(void) {
    printf("-----Help-----\nHere is how to use the flags integrated in Power-up:\n\n");
    printf("--add-white or -w to add the current window's PID to the application's white list.\n");
    printf("--add-black or -b to add the current window's PID to the application's black list.\n\n");
    printf("--kill or -k to brutally terminate the powerup tool running in the background.\n\n");
}
