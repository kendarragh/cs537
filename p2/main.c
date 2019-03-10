///////////////////////////////////////////////////////////////////////////
//// File:             main.c
//// Semester:         537 fall 2018
////
//// Author:           Kenneth Darragh kdarragh2@wisc.edu
////                   Courtney Trescher trescher@wisc.edu
//// CS Login:         kenneth, courtney
//// Lecturer's Name:  Bart Miller
////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include "queue.h"
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <ctype.h>

//Constants used to define size
static const int QUEUE_SIZE = 10;
static const int LINE_SIZE = 1024;

//Four different processes
void Reader();
void Munch1();
void Munch2();
void Writer();
pthread_t r, m1, m2, w;

//Three queues used in between threads
Queue *q1, *q2, *q3;


int main() {
    //Create queues
    q1 = CreateStringQueue(QUEUE_SIZE); 
    q2 = CreateStringQueue(QUEUE_SIZE); 
    q3 = CreateStringQueue(QUEUE_SIZE); 

    //Create pthread threads
    pthread_create(&r, NULL, (void *) Reader, NULL);
    pthread_create(&m1, NULL, (void *) Munch1, NULL);
    pthread_create(&m2, NULL, (void *) Munch2, NULL);
    pthread_create(&w, NULL, (void *) Writer, NULL);

    //Wait for them
    pthread_join(r, NULL);
    pthread_join(m1, NULL);
    pthread_join(m2, NULL);
    pthread_join(w, NULL);

    //print stats
    fprintf(stderr, "Queue 1 Stats:\n");
    PrintQueueStats(q1);
    fprintf(stderr, "Queue 2 Stats:\n");
    PrintQueueStats(q2);
    fprintf(stderr, "Queue 3 Stats:\n");
    PrintQueueStats(q3);
}

void Reader() {
    //Read input
    int c = 0;
    //char line[LINE_SIZE] = "";
    int count = 0;
    char* line = malloc(sizeof(char) * LINE_SIZE);
    while (c != EOF) {
        for (int i = 0; i <= LINE_SIZE; i++) {
            if (i == LINE_SIZE) {
                fprintf(stderr, "Line size too big, line discarded\n");
                while(c != '\n') {
                    count++;
                    c = getc(stdin);
                    if (feof(stdin)) {
                        EnqueueString(q1, NULL);
                        pthread_exit(NULL);
                    }
                }
                break;
            }
            c = getc(stdin);
            if (feof(stdin)) {
                EnqueueString(q1, NULL);
                pthread_exit(NULL);
            }
            if (c == '\n') {
                line[i] = '\0';
                EnqueueString(q1, line);
                break;
            }
            line[i] = c;
        }
    }
    EnqueueString(q1, NULL);                     
    pthread_exit(NULL);
    //should never get here
}

//Replace all spaces with stars
void Munch1() {
    char* line;
    char* i;
    line = DequeueString(q1);

    //Will keep running until it reaches NULL
    //This signals q1 is done
    while(line != NULL) {
        i = index(line, ' ');
        //Replace all spaces with stars
        while(i != NULL) {
            *i = '*';
            i = index(line, ' ');
        }
        //Add to q2
        EnqueueString(q2, line);
        line = DequeueString(q1);
    }

    //Signal that q2 is done and exit
    EnqueueString(q2, NULL);
    //If q2 is done then q1 is ready to be freed
    for (int i = 0; i < q1->maxsize; i++) {
        free(q1->strings[i]);
    }
    free(q1->strings);
    free(q1);
    pthread_exit(NULL);
}

//Make all letters uppercase
void Munch2() {
    char* line;
    line = DequeueString(q2);
    //Make all letters uppercase and add to q3
    while(line != NULL) {
        for(int i = 0; i < (int)strlen(line); i++) {
            if(islower(line[i])) {
                line[i] = toupper(line[i]);
            }
        }
        EnqueueString(q3, line);
        line = DequeueString(q2);
    }

    //Indicate q3 is done and free q2
    EnqueueString(q3, NULL);
    for (int i = 0; i < q2->maxsize; i++) {
        free(q2->strings[i]);
    }
    free(q2->strings);
    free(q2);
    pthread_exit(NULL);
}

//Write final strings to stdout
void Writer() {
    char* line;
    int count = 0;
    line = DequeueString(q3);
    while(line != NULL) {
        printf("%s\n", line);
        count++;
        line = DequeueString(q3);
    }
    //print total writes
    printf("Total writes: %d\n", count);

    //Free q3 and exit
    for (int i = 0; i < q3->maxsize; i++) {
        free(q3->strings[i]);
    }
    free(q3->strings);
    free(q3);
    pthread_exit(NULL);
}
