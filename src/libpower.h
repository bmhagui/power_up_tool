#define STOP_AFTER_S 5
#define REFRESH_RATE_S 2

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

char path_black_list[100], path_black_list_pid[100],
  path_refresh_list[100], path_refresh_list_pid[100], path_config_powerup[100],
  path_open_windows[100], path_window_change[100], path_notif[100], path_libpowerh[100],
  app_name[100], read_name[100], path_runtime_dir[100];

struct sigaction action;
pid_t pid, active_pid;
int fd, wd;
FILE *fp,*fl,*flp, *pipe_popen, *check;
DIR* dir;
wordexp_t expansion;

void activate_all(void);

void hand(int sig);

typedef struct Element Element;
struct Element{
    pid_t pid;
    Element *next;
};

typedef struct Liste Liste;
struct Liste{
    Element *first;
};

void activate_list(Liste *liste);

Liste *initialisation(void);

void insertion(Liste *liste, pid_t new_pid);

bool member(pid_t cpid, Liste *liste);

Liste *create_list(char *file_path);

void print_usage(void);

void check_paths(void);

Liste_toggle *init_toggle(void);

void insertion_toggle(Liste_toggle *liste, char name[]);

void delete_toggle(Liste_toggle *liste);

void rewrite_file(Liste_toggle *liste, FILE *fp);

void toggle(int exists);

void running_check(void);
