////////////////////////////////////////////////////////////////////////    
//// File:             queue.c
//// Semester:         537 fall 2018
////
//// Author:           Kenneth Darragh kdarragh2@wisc.edu
////                   Courtney Trescher trescher@wisc.edu
//// CS Login:         kenneth, courtney
//// Lecturer's Name:  Bart Miller
////////////////////////////////////////////////////////////////////////////
#include "queue.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
static const int LINE_SIZE = 1024;

//Constructs a Queue of string of given size
Queue *CreateStringQueue(int size) {
    Queue* q;

    //Allocate memory
    q = malloc(sizeof(struct Queue) );
    if (q == NULL) {
        fprintf(stderr, "Error allocating memory");
        exit(1);
    }
    q->strings = malloc(sizeof(char*) * size);
    if (q->strings == NULL) {
        fprintf(stderr, "Error allocating memory");
        exit(1);
    }
    for(int i = 0; i < size; i++) {
        q->strings[i] = malloc(sizeof(char) * LINE_SIZE);
        if (q->strings[i] == NULL) {
            fprintf(stderr, "Error allocating memory");
            exit(1);
        }
    }

    //Initialize variables
    q->size = 0;
    q->front = 0;
    q->rear = 0;
    q->maxsize = size;
    q->enqueueCount = 0;
    q->dequeueCount = 0;
    q->enqueueBlockCount = 0;
    q->dequeueBlockCount = 0;
    
    //Initialize lock and condition variables
    if (pthread_mutex_init(&q->lock, NULL) != 0) {
        fprintf(stderr, "Error initializing lock");
        exit(1);
    }
    if ((pthread_cond_init(&q->notFull, NULL) != 0) || 
            (pthread_cond_init(&q->notEmpty, NULL) != 0)) {
        fprintf(stderr, "Error initializing condition variables");
        exit(1);
    }
    return q;
}

//Enqueues a string to the given queue
void EnqueueString(Queue *q, char *string) {
    //Check for mutex
    if (pthread_mutex_lock(&q->lock) != 0) {
        fprintf(stderr, "Error in pthread_mutex_lock");
        exit(1);
    }
    //Block while queue is full
    while (q->size == q->maxsize) {
        q->enqueueBlockCount++;
        if (pthread_cond_wait(&q->notFull, &q->lock) != 0) {
            fprintf(stderr, "Error in pthread_cond_wait");
            exit(1);
        }
    }
    //Special case where string is null, used to indicate last string
    if(string != NULL) { 
        strcpy(q->strings[q->rear], string);
        q->enqueueCount++;
    } else {
        q->strings[q->rear] = NULL;
    }
    //Adjust rear pointer
    q->rear = ((q->rear + 1) % q->maxsize);
    q->size++;
       

    //Release lock and monitor 
    if (pthread_cond_signal(&q->notEmpty) != 0) {
        fprintf(stderr, "Error in pthread_cond_signal");
        exit(1);
    }
    if (pthread_mutex_unlock(&q->lock) != 0) {
        fprintf(stderr, "Error in pthread_mutex_unlock");
        exit(1);
    }
}

//Removes and returns a string from the queue
char * DequeueString(Queue *q) {
    //Check for mutex
    if (pthread_mutex_lock(&q->lock) != 0) {
        fprintf(stderr, "Error in pthread_mutex_lock");
        exit(1);
    }
    //Block while queue is empty
    while(q->size == 0) {
        q->dequeueBlockCount++;
        if (pthread_cond_wait(&q->notEmpty, &q->lock) != 0) {
            fprintf(stderr, "Error in pthread_cond_wait");
            exit(1);
        }
    }

    //Get the first string
    char* str = q->strings[q->front];
    q->front = (q->front + 1) % q->maxsize;
    q->size--;
    //Only increment stats when string is not null
    //Null string isn't from input so it doesn't count
    if (str != NULL) {
        q->dequeueCount++;  
    }
    if (pthread_cond_signal(&q->notFull) != 0) {
        fprintf(stderr, "Error in pthread_cond_signal");
        exit(1);
    }
    if (pthread_mutex_unlock(&q->lock) != 0) {
        fprintf(stderr, "Error in pthread_mutex_unlock");
        exit(1);
    }
    return str;
}

//Print queue stats to stderr
void PrintQueueStats(Queue *q) {
    fprintf(stderr, "enqueueCount: %d\ndequeueCount: %d\n", q->enqueueCount, q->dequeueCount);
    fprintf(stderr, "enqueueBlockCount: %d\ndequeueBlockCount: %d\n", q->enqueueBlockCount, q->dequeueBlockCount);
}

