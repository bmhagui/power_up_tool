#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

void print_usage(void) {
    printf("-----Help-----\nHere is how to use the flags integrated in Power-up:\n\n");
    printf("--add-white or -w to add the current window's PID to the application's white list.\n");
    printf("--add-black or -b to add the current window's PID to the application's black list.\n\n");
}

int main(int argc, char *argv[]) {
    int opt= 0;

    static struct option long_options[] = {
        {"help",      no_argument, 0,  'h' },
	{"add-white", no_argument, 0,  'w' },
	{"add-black", no_argument, 0,  'b' },
        {    0,           0,       0,   0  }
    };
    
    int long_index =0;
    while ((opt = getopt_long(argc, argv,"hwb", long_options, &long_index )) != -1) {
      switch (opt) {
      case 'h' :
	print_usage();
	exit (0);
      case 'w' :
	printf("Add to white list\n");
	break;
      case 'b' :
        printf("Add to black list\n");
	break;
      }
    }
    printf("Continue the rest.................\n");
    return 0;
}
