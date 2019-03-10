//////////////////////////////////////////////////////////////////////////////
////                   ALL STUDENTS COMPLETE THESE SECTIONS
//// File:             main.c 
//// Semester:         537 fall 2018
////
//// Author:           Kenneth Darragh kdarragh2@wisc.edu
////                   Courtney Trescher trescher@wisc.edu
//// CS Login:         kenneth, courtney
//// Lecturer's Name:  Bart Miller
//////////////////////////////////////////////////////////////////////////////
#include "options_processing.h"
#include "process_list.h"
#include "stat_file_parser.h"
#include <stdlib.h>

int main(int argc, char* argv[]) {
    options(argc, argv);

    if (p) {
        get_proc_by_pid(pid);
    } else {
        int* pids = NULL;
        int i = 0;
        pids = get_proc();
        if (pids == NULL) {
            free(pids);
            exit(0);
        }
        while (pids[i] != 0) {
            get_proc_by_pid(pids[i]);
            i++;
        }
        free(pids);
    }
    exit(0);
}
