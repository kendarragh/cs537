#include "stat_file_parser.h"
#include "options_processing.h"
#include "process_list.h"

void stat(int pid) {
    printf("entering stat");
    printf("%d: ", pid);

    //Create filepaths
    FILE* stat, statm, cmdline;
    char* statpath, statmpath, cmdlinepath;
    sprintf(statpath, "/proc/%d/stat", pid);
    sprintf(statmpath, "/proc/%d/statm", pid);
    sprintf(cmdlinepath, "/proc/%d/cmdline", pid);

    //Open stat file and read
    stat = fopen(statpath, "r");
    if (stat ==  NULL) {
        //error
        exit(1);
    }
    //char line[80];
    //fgets(line, sizeof line, stat);

    //Parse stat file and save info
    char state;
    unsigned long utime;
    unsigned long stime;
    fscanf(stat, "%*d %*s %c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %lu %lu", state, utime, stime);
    if (s) {
        //find state
        printf("%c", state); 
    } 
    if (!u) {
        //fint user time
        printf(" utime=%lu", utime);
    }

    if (bigS) {
        //find system time
        printf(" stime=%lu", stime);
    }
    fclose(stat);
    //statm = fopen(statmpath, "r");
    //if (statm == NULL) {
        //error
     //   exit(1);
    //}

    if (v) {
        //find virtual memory
    }
    //fclose(statm);
    //cmdline = fopen(cmdpath, "r");
    if (!c) {
        //display command line
    }
    
    //construct output string
    //print
}
