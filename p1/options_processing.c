//////////////////////////////////////////////////////////////////////////////
////                   ALL STUDENTS COMPLETE THESE SECTIONS
//// File:             options_processing.c
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
#include "options_processing.h"
#include <string.h>
#include <errno.h>

//Flags for options
int p = 0;
int s = 0;
int u = 1;
int bigS = 0;
int v = 0;
int c = 1;
int pid = 0;

//Set options flags and catch any errors
void options(int argc, char* argv[]) {
    int opt;
    //Flags to find duplicate arguments
    int pflag = 0, sflag = 0, uflag = 0, bigSflag = 0, vflag = 0, cflag = 0;

    //Usage string, made variable because its printed many times
    const char* USAGE  = "Usage: 537ps [-p pid] [-c] [-U] [-S] [-v] [-C]\n";
   
    while ((opt = getopt(argc, argv, ":p:s::U::S::v::c::")) != -1) {
        switch (opt) {
        case 'p':
            //Check for duplicate argument
            if (pflag) {
                fprintf(stderr, "%s", USAGE); 
                exit(EXIT_FAILURE);
            }
            pflag = 1; 
            
            //Set flag to 1
            p = 1;
                        
            //Check that pid is a number and convert it
            if (optarg == NULL) {
                fprintf(stderr, "%s", USAGE);
                exit(EXIT_FAILURE);
            }
            char** endptr = NULL; 
            pid = (int) strtol(optarg, endptr, 10);
            
            //Error, will occur if pid is not a valid number
            //Could occur if user actually entered 0, but I think
            //that is an invalid pin as it is not a user process
            if((pid == 0) || (endptr != NULL)) {
                fprintf(stderr, "Please enter a valid pid.\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 's':
            if (sflag) {
                fprintf(stderr, "%s", USAGE); 
                exit(EXIT_FAILURE);
            }
            sflag = 1; 

            s = 1;
            if (optarg == NULL) {
                break;
            }
            if ((strcmp(optarg, "-") != 0)) {
                fprintf(stderr, "-s was given an invalid argument");
                exit(EXIT_FAILURE);   
            }
            s = 0;
            break;
        case 'U':
            if (uflag) {
                fprintf(stderr, "%s", USAGE); 
                exit(EXIT_FAILURE);
            }
            uflag = 1; 

            u = 1;
            if (optarg == NULL) {
                break;
            }
            if ((strcmp(optarg, "-") != 0)) {
                fprintf(stderr, "-U was given an invalid argument");
                exit(EXIT_FAILURE);   
            }
            u = 0; 
            break;
        case 'S':
            if (bigSflag) {
                fprintf(stderr, "%s", USAGE); 
                exit(EXIT_FAILURE);
            }
            bigSflag = 1; 

            bigS = 1;
            if (optarg == NULL) {
                break;
            }
            if ((strcmp(optarg, "-") != 0)) {
                fprintf(stderr, "-S was given an invalid argument");
                exit(EXIT_FAILURE);   
            }
            bigS = 0;
            break;
        case 'v':
            if (vflag) {
                fprintf(stderr, "%s", USAGE); 
                exit(EXIT_FAILURE);
            }
            vflag = 1; 

            v = 1;
            if (optarg == NULL) {
                break;
            }
            if ((strcmp(optarg, "-") != 0)) {
                fprintf(stderr, "-v was given an invalid argument");
                exit(EXIT_FAILURE);   
            }
            v = 0;
            break;
        case 'c':
            if (cflag) {
                fprintf(stderr, "%s", USAGE); 
                exit(EXIT_FAILURE);
            }
            cflag = 1; 

            c = 1;
            if (optarg == NULL) {
                break;
            }
            if ((strcmp(optarg, "-") != 0)) {
                fprintf(stderr, "-c was given an invalid argument");
                exit(EXIT_FAILURE);   
            }
            c = 0;
            break;
        default:
            fprintf(stderr, "%s", USAGE);
            exit(EXIT_FAILURE);
        }
    }
    //Was given extra arguments
    if (optind < argc) {
            fprintf(stderr, "%s", USAGE);
            exit(EXIT_FAILURE);
    }


    //printf("Input:");
    //for (int i = 0; i < argc; i++) {
    //    printf(" %s", argv[i]);
    //}
    //printf("\np=%d (%d)\ns=%d\nU=%d\nS=%d\nv=%d\nc=%d\n",p,pid,s,u,bigS,v,c);

}
