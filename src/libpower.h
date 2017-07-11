#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <wordexp.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

typedef struct Element_toggle Element_toggle;
struct Element_toggle
{
    char name[100];
    Element_toggle *suivant;
};

typedef struct Liste_toggle Liste_toggle;
struct Liste_toggle
{
    Element_toggle *premier;
};

typedef struct Proc Proc;
struct Proc{
  pid_t pid;
  time_t time_added;
  bool paused;
  Proc *next;
};

typedef struct Stop_list Stop_list;
struct Stop_list{
  bool refresh_active;
  int count_procs;
  Proc *first;
};

struct sigaction action;

Stop_list *stop_list;

char path_black_list[100], path_black_list_pid[100], verbose[100],
  path_refresh_list[100], path_refresh_list_pid[100], path_config_powerup[100],
  path_open_windows[100], path_window_change[100], path_notif[100], path_time[100],
  app_name[100], read_name[100], path_runtime_dir[100], tmp[100];

pid_t pid, new_active_pid, old_active_pid;
int fd, wd;
FILE *fp,*fl,*flp, *pipe_popen, *check, *pipe_wc, *refresh_fp;
DIR* dir;
wordexp_t expansion;

time_t time_now, changed_refresh_status;

void activate_all(void);

void hand(int sig);

void print_usage(void);

void check_paths(void);

Liste_toggle *init_toggle(void);

void insertion_toggle(Liste_toggle *liste, char name[]);

void delete_toggle(Liste_toggle *liste);

void rewrite_file(Liste_toggle *liste, FILE *fp);

void toggle(int exists);

void running_check(void);

void add_to_list(char * app_name, FILE *fp, int exists);

Stop_list *init_stop_list();

void equal_count(Stop_list *list, pid_t new_active_pid, time_t STOP_AFTER_S, bool verbose_bool);

void affiche_stop_liste(Stop_list *list);

void diff_count(Stop_list *list, FILE *fp, pid_t new_active_pid, time_t STOP_AFTER_S, bool verbose_bool);

void delete_stop_list(Stop_list *list);

void delete_unused_pid(Stop_list *list);

void get_pid(void);

void handle_applications(int STOP_AFTER_S,int REFRESH_RATE_S, int REFRESH_FOR_S, int count, bool verbose_bool);
