#include <stdio.h>
#include <string.h>
#define LINE_SIZE 80
#include <stdlib.h>

char * getstr();
int i = 0;

int main() {
    char line[LINE_SIZE] = "";
    char* i;

    strcpy(line, getstr());
    while(line != NULL) {
        i = index(line, ' ');
        while (i != NULL) {
            *i = '*';
            i = index(line, ' ');
        }
        printf("%s", line);
        char* str = getstr();
        if (str != NULL) {
            strcpy(line, str);
        }

    }

}

char * getstr() {
    char ret[80];
    return fgets(ret, 80, stdin);
}
