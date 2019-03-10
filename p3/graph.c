#include <stdio.h>

graphNode build_graph(char* filename) {
    FILE* makefile = fopen(filename);

