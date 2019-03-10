#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parse.h"

const int BUF_SIZE = 8;
const int LINE_SIZE = 1024; 

void print_error(int linenum, char* line);
int expand(char** arr, int size);

//TODO check system calls
char **split_line(int linenum, char* line) {
    int curr_size = BUF_SIZE;
    char** ret = malloc(BUF_SIZE * sizeof(char*));
    for (int i = 0; i < BUF_SIZE; i++) {
        ret[i] = malloc(LINE_SIZE * sizeof(char));
    }
    
    char* str = malloc(LINE_SIZE * sizeof(char));
    strcpy(str, line);
    ret[0] = strtok(str, ":");
    if (strcmp(str, ret[0]) == 0) {
        //Should be a command, starts with tab
        if (line[0] == '\t') {
            ret[0] = "\t";
        } else {
            print_error(linenum, line);
        }
    } else {
        //Check that target contains no spaces
        for (int i = 0; i < strlen(ret[0]); i++) {
            if (ret[0][i] == ' ') {
                print_error(linenum, line);
                exit(-1);
            }
        }
    }
    str = strtok(str, "\t");
    //printf("ret[0] = \"%s\"\n", ret[0]);
    ret[1] = strtok(str, " ");
    int i = 2;
    while (ret[i] = strtok(NULL, " ")) {
        if (i == curr_size-1) {
            curr_size = expand(ret, curr_size);
        }
        i++;
    }
    return ret;
}

int expand(char** arr, int size) {
    realloc(arr, size * 2);
    return size * 2;
}

void print_error(int linenum, char* line) {
    fprintf(stderr, "%d: Invalid line: %s\n", linenum, line);
}

int main() {
    char* string1 = "537ps.o: readproc.o parseopts.o output.o";
    char** arr = split_line(0, string1);
    int i = 0;
    while (arr[i] != NULL) {
        fprintf(stdout, "%s\n", arr[i]);
        i++;
    }
    return 0;
}


