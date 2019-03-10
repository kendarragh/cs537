//////////////////////////////////////////////////////////////////////////////
////                   ALL STUDENTS COMPLETE THESE SECTIONS
//// File:             process_list.c
//// Semester:         537 fall 2018
////
//// Author:           Kenneth Darragh kdarragh2@wisc.edu
////                   Courtney Trescher trescher@wisc.edu
//// CS Login:         kenneth, courtney
//// Lecturer's Name:  Bart Miller
//////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include "stat_file_parser.h"

//Determines if string is a number
int isnumber(char*);

//Create array of pids that match uid of user
int* get_proc() {
    int* pids = malloc(256 * sizeof(int)); 
    int maxsize = 256;
    int npids = 0;
    DIR *dir;
    struct dirent *entry;
    FILE *file;
    //Try to open /proc
    if ((dir = opendir("/proc/")) == NULL) {
        fprintf(stderr, "Could not open /proc");
        exit(1);
    } else {
        //Loop through all directories that are numbers 
        while ((entry = readdir(dir)) != NULL) { 
            char* name = (char*)entry->d_name;
            //Don't look at hidden files
            if (name[0] == '.') {
                continue;
            }
            //Skip folders that arent processes
            if(!isnumber(name)) {
                continue;
            }
            
            //Construct file name in format /proc/<pid>/status
            char fname[256];
            strcpy(fname, "/proc/");
            strcat(fname, name);
            strcat(fname, "/status");

            //Open file 
            file = fopen(fname, "r"); 
            if (file != NULL) { 
                char line[80];
                int count = 0;
                int uid = 0;
                while (fgets(line, sizeof line, file) != NULL) {
                    //Check if uid of process matches current uid
                    if (count == 8) {
                        sscanf(line, "Uid:\t%u\t", &uid);
                        if(uid == (int)getuid()) {
                            //Increase array size if full
                            if (npids == maxsize) {
                                maxsize = maxsize * 2;
                                pids = realloc(pids, maxsize * sizeof(int));
                            }
                            //Match found, add to array 
                            pids[npids] = atoi(entry->d_name);
                            npids++;
                        }
                        break;
                    }
                    count++;
                }
                fclose(file);
            }
        }
    }
    if (dir) {
        closedir(dir);
    }
    return pids;
}
            
//Pretty useless, just calls stat
//was used before when TA said we should sort output
void get_proc_by_pid(int pid) {
    stat(pid);
    return;
}

int isnumber(char* input) {
    for (int i = 0; i < (int) strlen(input); i++) {
        if (!isdigit(input[i])) {
            return 0;
        }
    }
    return 1;
}
             
