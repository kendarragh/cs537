#include "queue.h"
#include <stdio.h>

int main() {
    Queue* q;
    q = CreateStringQueue(10);

    char* one = "First";
    char* two = "Second";
    char* three = "Third";

    EnqueueString(q, one);
    EnqueueString(q, two);
    EnqueueString(q, three);

    char* oneB = DequeueString(q);
    char* twoB = DequeueString(q);
    char* threeB = DequeueString(q);

    printf("Input: ");
    printf("%s %s %s", one, two, three);
    printf("Output: ");
    printf("%s %s %s", oneB, twoB, threeB);

}
