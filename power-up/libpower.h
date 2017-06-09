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

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )
#define REFRESH_RATE_S 5

struct sigaction action;
pid_t pid, active_pid;
int fd;
int wd;
FILE *fp,*fl,*flp;

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
