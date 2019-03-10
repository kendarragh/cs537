//////////////////////////////////////////////////////////////////////////
//// File:             parse.c
//// Semester:         537 fall 2018
////
//// Author:           Kenneth Darragh kdarragh2@wisc.edu
////                   Courtney Trescher trescher@wisc.edu
//// CS Login:         kenneth, courtney
//// Lecturer's Name:  Bart Miller
////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "node.h"
#include "parse.h"

//Constants
const int LINE_SIZE = 1024;
const int BUF_SIZE = 8;

//Splits a line
char **split_line(int linenum, char* line) {
    int curr_size = BUF_SIZE;
    char** ret = malloc(BUF_SIZE * sizeof(char*));
    if (ret == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        exit(-1);
    }
    for (int i = 0; i < BUF_SIZE; i++) {
        ret[i] = malloc(LINE_SIZE * sizeof(char));
        if (ret[i] == NULL) {
            fprintf(stderr, "Error allocating memory\n");
            exit(-1);
        }
    }

    char* sv = NULL;
    char* sv2 = NULL;
    
    char* str = malloc(LINE_SIZE * sizeof(char));
    if (str == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        exit(-1);
    }

    strcpy(str, line);

    //Count colons, anything other than 0 or 1 is invalid
    int colon_count = 0;
    for(int i = 0; i < (int) strlen(line); i++) {
        if (line[i] == ':') {
            colon_count++;
        }
    }

    //Split for command line
    if(starts_with_tab(line)) {
        //Check for colon
        if (colon_count != 0) {
            print_error(linenum, line);
            exit(-1);
        }
        strcpy(ret[0], "\t");
        //ret[0] = "\t"; 
        strcpy(ret[1], strtok(str, "\t"));
        //ret[1] = strtok(str, "\t");
    //Split for target line
    } else { 
        strcpy(ret[0],strtok_r(str, ":", &sv));
       // ret[0] = strtok_r(str, ":", &sv);
        //Check for colon
        if (colon_count != 1) {
            print_error(linenum, line);
            exit(-1);
        }
        //Check for spaces
        int trailing = 0;
        for(int i = 0; i < (int) strlen(ret[0]); i++) {
            if (ret[0][i] == ' ') {
                trailing = 1;
            } else if (trailing == 1) {
                print_error(linenum, line);
                exit(-1);
            }
        }
        //There were trailing spaces, trim them
        if (trailing) {
            strtok_r(ret[0], " ", &sv2);
        }
        char* no_trails = strtok_r(NULL, " ",&sv);
        if (no_trails != NULL) {
            strcpy(ret[1], no_trails);
        }
        //ret[1] = strtok_r(NULL, " ", &sv);

        //get rest of string
        int i = 2;
        char* curr;
        while ((curr = strtok_r(NULL, " ", &sv))) {
            strcpy(ret[i], curr);
            //Expand array
            if (i == curr_size-1) {
                char** new = realloc(ret, curr_size * 2 * LINE_SIZE); 
                if (new) {
                    ret = new;
                } else {
                    fprintf(stderr, "Realloc failed\n");
                    exit(-1);
                }
            }
            i++;
        }
    }
    return ret;
}

//Print linenum and full line of error
void print_error(int linenum, char* line) {
    fprintf(stderr, "%d: Invalid line: %s\n", linenum, line);
}

//Check if a string starts with tab, which indicates a command
int starts_with_tab(char* line) {
    if(line[0] == '\t') {
        return 1;
    }
    return 0;
}

//Parses the makefile and returns a list of nodes
Node** parse(char* filename) {
    //Default to makefile, otherwise Makefile
    FILE* makefile;
    if (filename == NULL) {
        makefile = fopen("makefile", "r");
        if (makefile == NULL) {
            makefile = fopen("Makefile", "r");
        }
    } else {
        makefile = fopen(filename, "r");
    }
    if (makefile == NULL) {
        fprintf(stderr, "No makefile found\n");
        exit(-1);
    }
   
    Node** list = malloc(sizeof(Node) * BUF_SIZE);
    if(list == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(-1);
    }
    int num_targets = 0;
    int linenum = 0;
    int done = 0;
    Node* prev = NULL;
    
    //Loop through all lines
    while(!done) {
        //Read a line
        char* line = malloc(LINE_SIZE * sizeof(char));
        if(line == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(-1);
        }
        int i = 0;
        char c;
        c = fgetc(makefile);
        if (feof(makefile)) {
            done = 1;
            break;
        }
        while ((c != '\n') && (c != EOF)) {
            line[i] = c;
            i++;
            c = fgetc(makefile);
        }
        linenum++;
       
        //Ignore if empty
        if ((line == NULL) || (strcmp(line, "") == 0)) {
            continue;
        }

        //Ignore if comment
        if (line[0] == '#') {
            continue;
        }
        
        //Split line
        char** arr = split_line(linenum, line);

        //If line is a command, add to prev target
        if (strcmp(arr[0],"\t") == 0) {
            if((prev == NULL) || (arr[1] == NULL)) {
                print_error(linenum, line);
            }
            add_command(prev, arr[1]);
            continue;
        }

        //If line is a target, make a new target
        Node* target = CreateNode(arr[0]);
        int j = 1;
        while (arr[j] != NULL) {
            add_dependency(target, arr[j]);
            j++;
        }
        prev = target;
        list[num_targets] = target;
        num_targets++;

        //Not good enough if expands
    }

    //Sentinel value
    list[num_targets] = CreateNode(NULL);
    return list;
}
