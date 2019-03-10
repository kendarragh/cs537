//////////////////////////////////////////////////////////////////////////////
////                   ALL STUDENTS COMPLETE THESE SECTIONS
//// File:             stat_file_parser.c
//// Semester:         537 fall 2018
////
//// Author:           Kenneth Darragh kdarragh2@wisc.edu
////                   Courtney Trescher trescher@wisc.edu
//// CS Login:         kenneth, courtney
//// Lecturer's Name:  Bart Miller
//////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#define MAX 80
#include "stat_file_parser.h"
#include "options_processing.h"

void stat(int pid) {


	//Create filepaths
	char statpath[MAX], statmpath[MAX], cmdlinepath[MAX];
	FILE *stat = NULL;
	FILE *statm = NULL;
	FILE *cmdline = NULL;
	sprintf(statpath, "/proc/%d/stat", pid); //opens correct file
	sprintf(statmpath, "/proc/%d/statm", pid);
	sprintf(cmdlinepath, "/proc/%d/cmdline", pid);
	
	//Open stat file and read
	stat = fopen(statpath, "r");
	if (stat == NULL) {
		fprintf(stderr, "Cannot open file\n");
                exit(EXIT_FAILURE);
	}

	//Parse stat file and save info	
	char *stat_input = NULL;
	char stat_line[MAX];
        stat_input = fgets(stat_line, sizeof(stat_line), stat);
	char *stat_token = NULL; 
        
        //Separate first part of string because name could contain spaces
        const char* p = ")";
        stat_token = strtok(stat_input, p);
        if (stat_token == NULL) {
            fprintf(stderr, "stat file in unreadable format");
            exit(1);
        }
        stat_token = strtok(NULL, p);

        //Separate rest of string by spaces
	const char st[sizeof(stat_line)] = " ";
	int count = 0;
	char* state = NULL;
	char* utime = NULL;
	char* stime = NULL;
	state = stat_token = strtok(stat_token, st);
	while(stat_token != NULL) {
	    if (count == 10) {
		utime = strtok(NULL, st);
    	    } else if (count == 11) {
      		stime = strtok(NULL, st);
	    } else {
		stat_token = strtok(NULL, st);
            }
	    count++;
	}
        
	printf("%d: ", pid);
	if (s) {
            if (state == NULL) {
                fprintf(stderr, "stat file in unreadable format");
                exit(1);
            }
	    printf("%s ", state);
	}

	if (u) {
            if (utime == NULL) {
                fprintf(stderr, "stat file in unreadable format");
                exit(1);
            }
		printf("utime=%s ", utime);
	}
	
	if (bigS) {
            if (stime == NULL) {
                fprintf(stderr, "stat file in unreadable format");
                exit(1);
            }
		printf("stime=%s ", stime);
	}
	fclose(stat);
	
	//Open statm and get info
	statm = fopen(statmpath, "r");
	if (statm == NULL){
		fprintf(stderr, "Cannot open file");
                exit(EXIT_FAILURE);
	}
	char *statm_input = NULL;
	char statm_line[MAX];
	statm_input = fgets(statm_line, sizeof(statm_line), statm);
	char *statm_token = strtok(statm_input, " ");

	if (v) {
            if (statm_token == NULL) {
                fprintf(stderr, "statm file in unreadable format");
                exit(1);
            }
	    printf("vmemory=%s ", statm_token);
	}
	fclose(statm);

	//Open cmdline and get info
	cmdline = fopen(cmdlinepath, "r");
	if (cmdline == NULL) {
            fprintf(stderr, "Cannot open file");
            exit(EXIT_FAILURE);

	}
	char *cmd_input = NULL;
	char cmd_line[MAX];
	cmd_input = fgets(cmd_line, sizeof(cmd_line), cmdline);

	if (c) {
            if (cmd_input == NULL) {
                fprintf(stderr, "cmdline file in unreadable format");
                exit(1);
            }
	    printf("%s ", cmd_input);
	}
	fclose(cmdline);
	printf("\n");
}
	
