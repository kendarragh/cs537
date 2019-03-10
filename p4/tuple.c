///////////////////////////////////////////////////////////////////////////
//// File:             tuple.c
//// Semester:         537 fall 2018
////
//// Author:           Kenneth Darragh kdarragh2@wisc.edu
////                   Courtney Trescher trescher@wisc.edu
//// CS Login:         kenneth, courtney
//// Lecturer's Name:  Bart Miller
////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include "tuple.h"

Tuple* createTuple(void* ptr, size_t len) {
    Tuple* ret = malloc(sizeof(Tuple));
    ret->ptr = ptr;
    ret->len = len;
    ret->mal = 0;
    return ret;
}

void setAlloc(Tuple* t) {
    t->mal = 1;
}

void setFree(Tuple* t) {
    t->mal = 0;
}
