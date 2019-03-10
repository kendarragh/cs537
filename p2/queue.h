#include <pthread.h>
typedef struct Queue {
	char **strings;
        int maxsize;
        int size;
        int front, rear;
	int enqueueCount;
	int dequeueCount;
	int enqueueBlockCount;
	int dequeueBlockCount;
        pthread_cond_t notEmpty;
        pthread_cond_t notFull;
        pthread_mutex_t lock;
}Queue;

Queue *CreateStringQueue(int size);

void EnqueueString(Queue *q, char *string);

char* DequeueString(Queue *q);

void PrintQueueStats(Queue *q);
