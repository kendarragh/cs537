///////////////////////////////////////////////////////////////////////////
//// File:             execute.c
//// Semester:         537 fall 2018
////
//// Author:           Kenneth Darragh kdarragh2@wisc.edu
////                   Courtney Trescher trescher@wisc.edu
//// CS Login:         kenneth, courtney
//// Lecturer's Name:  Bart Miller
////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "execute.h"
#include "main.h"

void tokenize(char *string) {
        printf("%s\n", string);
	const int BUF_SIZE = 1024;
	char *myargs[BUF_SIZE];
	myargs[0] = strtok(string, " ");
	//printf("MY ARGS 0: %s\n", myargs[0]);
	int i=0;
	while (myargs[i] != NULL) {
		i++;
		if (i >= BUF_SIZE) {
			fprintf(stderr, "Input is too long\n");
			exit(1);
		}
		myargs[i] = strtok(NULL, " ");
		//printf("MY ARGS %d: %s\n", i, myargs[i]);
		
	}
	execute(myargs);
}

//int because return number value
int execute(char **myargs) {
	//run the target specified. Should be passed in correctly aka arr[1]
	pid_t cpid = fork();
	if (cpid < 0) {
		fprintf(stderr, "fork failed\n");
		exit(1);
	} else if (cpid == 0) {
		//what child does
		myargs[0] = strdup(myargs[0]);
		int rc = execvp(myargs[0], myargs);
		//successful exec doesn't return
		if (rc == EXIT_FAILURE) {
			perror("Command failed");
			exit(EXIT_FAILURE);
		}
	} else {
		int wstatus;
		//what parent does
		//Waits for child to run, then parent runs
		int cpid_wait = wait(&wstatus);
		if (cpid_wait == -1) {
			perror("wait");
			exit(EXIT_FAILURE);
		}
		if (WIFEXITED(wstatus)) {
			//printf("exited, status=%d\n", WEXITSTATUS(wstatus));
		} 
		if (WEXITSTATUS(wstatus) == EXIT_FAILURE) {
			// we exited with an error
			// print meaningfull error and exit
			fprintf(stderr, "execvp failed\n");
			exit(EXIT_FAILURE);
		}
			
	}

	return 0; //if good
}

