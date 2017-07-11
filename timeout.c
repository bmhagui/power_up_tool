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

int main( )
{
    int length, i = 0;
    int fd;
    int wd;
    char buffer[EVENT_BUF_LEN];

    struct timeval time;
    fd_set rfds;
    int ret;
    
    /*creating the INOTIFY instance*/
    fd = inotify_init();
    /*checking for error*/
    if ( fd < 0 ) {
        perror( "inotify_init" );
    }
    /*adding the “/tmp” directory into watch list. Here, the suggestion is to validate the existence of the directory before adding into monitoring list.*/
    wd = inotify_add_watch( fd, "testnoti", IN_CLOSE_WRITE | IN_CREATE | IN_DELETE | IN_ALL_EVENTS | IN_OPEN);


    while(1)
      {   
    /* timeout after five seconds */
    time.tv_sec = 5;
    time.tv_usec = 0;

    /* zero-out the fd_set */
    FD_ZERO (&rfds);
    /*
     * add the inotify fd to the fd_set -- of course,
     * your application will probably want to add
     * other file descriptors here, too
     */
    FD_SET (fd, &rfds);
    ret = select (fd + 1, &rfds, NULL, NULL, &time);

    if (ret < 0)
        printf("error case in select");
    else if (!ret)
        printf("error case in select timed out\n");
    /* timed out! */
    else if (FD_ISSET (fd, &rfds)) /* inotify events are available! */
    {
        printf("Inotify events available\n");
            /*read to determine the event change happens on “/tmp” directory. Actually this read blocks until the change event occurs*/
            length = read( fd, buffer, EVENT_BUF_LEN );
            /*checking for error*/
            if ( length < 0 ) {
                perror( "read" );
            }
            /*actually read return the list of change events happens. Here, read the change event one by one and process it accordingly.*/
            while ( i < length ) {
                struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
                if ( event->len ) {
                    if ( event->mask & IN_CREATE ) {
                        if ( event->mask & IN_ISDIR ) {
                            printf( "New directory %s created.\n", event->name );
                        }
                        else {
                            printf( "New file %s created.\n", event->name );
                        }
                    }
                    else if ( event->mask & IN_CLOSE_WRITE ) {
                        if ( event->mask & IN_ISDIR ) {
                            printf( "Directory %s updated.\n", event->name );
                        }
                        else {
                            printf( "File %s updated\n", event->name );
                        }
                    }
                    else if ( event->mask & IN_DELETE ) {
                        if ( event->mask & IN_ISDIR ) {
                            printf( "Directory %s deleted.\n", event->name );
                        }
                        else {
                            printf( "File %s deleted.\n", event->name );
                        }
                    }
                }
                i += EVENT_SIZE + event->len;
            }

            i = 0;
            length = 0;
            memset(buffer, 0, EVENT_BUF_LEN);
            /*removing the “/tmp” directory from the watch list.*/
        }
    }
    inotify_rm_watch( fd, wd );

    /*closing the INOTIFY instance*/
    close( fd );

}
