///////////////////////////////////////////////////////////////////////////
//// File:             main.c
//// Semester:         537 fall 2018
////
//// Author:           Kenneth Darragh kdarragh2@wisc.edu
////                   Courtney Trescher trescher@wisc.edu
//// CS Login:         kenneth, courtney
//// Lecturer's Name:  Bart Miller
////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "main.h"
#include "graph.h"
#include "parse.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){

    //Parse options
    int user_makefile = 0;
    char* user_makefile_string;
    int opt;
    while ((opt = getopt(argc, argv, "f:")) != -1) {
        switch(opt) {
            case 'f':
                user_makefile = 1;
                user_makefile_string = optarg;
                break;
            default:
                fprintf(stderr, "Usage: 537make [-f makefile] [target]\n");
                exit(EXIT_FAILURE);
        }
    }
    
    //Parse the specified makefile, or null if none given
    Node** list;
    if (user_makefile == 1) {
        list = parse(user_makefile_string);
    } else {
        list = parse(NULL);
    }

    //Build the graph with specified root, or first target if none given
    Graph *g;
    if (optind < argc) {
        int i = 0;
        while(strcmp(list[i]->target, argv[optind]) != 0) {
            i++;
        }
        g = build_graph(list, list[i]);
    } else {
        g = build_graph(list, list[0]);
    }

    //Traverse graph
    traverse_graph(g);
}

