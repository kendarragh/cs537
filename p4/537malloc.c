///////////////////////////////////////////////////////////////////////////
//// File:             537malloc.c
//// Semester:         537 fall 2018
////
//// Author:           Kenneth Darragh kdarragh2@wisc.edu
////                   Courtney Trescher trescher@wisc.edu
//// CS Login:         kenneth, courtney
//// Lecturer's Name:  Bart Miller
////////////////////////////////////////////////////////////////////////////
#include "537malloc.h"
#include "tuple.h"
#include "range_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

void *malloc537(size_t size) {
	//TODO check if operation is valid and makes sense
	if (size == 0) {
		fprintf(stderr, "WARNING: size of malloc is 0.\n");
	}
	void *addr = malloc(size);
        if (addr == NULL) {
            fprintf(stderr, "Error allocating memory, with regular malloc");
            exit(-1);
        }

        //Check if it's already there
        void* check = contains(addr);
        if (check == addr) {
            Tuple* conflict = lookupTuple(addr);
            if (conflict->mal == 1) {
                //Should never happen
                fprintf(stderr, "Error: we think malloc failed, but it didn't\n");
                exit(-1);
            }
            if (size == conflict->len) {
                conflict->mal = 1;
                return addr;
            } else {
                //We assume malloc didn't mess up?
                conflict->len = size;
                conflict->mal = 1;
                return addr;
            }
        }

	Tuple *t = createTuple(addr, size);
        setAlloc(t);
        //This is a new addr
        insertTuple(t);
        return addr;
}

void free537(void *ptr) {
        void* check = contains(ptr);
        if (check == NULL) {
            fprintf(stderr, "Attempted to free a pointer not allocated by malloc537\n");
            exit(-1);
        } 
        if (check != ptr) {
            Tuple* outer = lookupTuple(check);
            if (outer->mal == 1) {
            fprintf(stderr, "Attempted to free memory that was not the first byte of allocated range\n");
            exit(-1);
            }
        }  
        Tuple* t = lookupTuple(ptr);
        if (t->mal == 0) {
            fprintf(stderr, "Attempted to double free\n");
            exit(-1);
        } 
        setFree(t);
	free(ptr);
}

void *realloc537(void *ptr, size_t size) {
        if (size == 0) {
		fprintf(stderr, "WARNING: size of malloc is 0.\n");
	}
	if (ptr == NULL) {
	    return malloc537(size);
        }
        void* check = contains(ptr);
        if (check == NULL) {
            void* newAddr = realloc(ptr, size);
            Tuple* new = createTuple(newAddr, size);
            setAlloc(new);
            insertTuple(new);
            return newAddr;
        } else if (check != ptr) {
            fprintf(stderr, "Attempted to realloc memory that was not the first byte of allocated range\n");
            exit(-1);
        }
        
        Tuple* old = lookupTuple(ptr);
        setFree(old);
        void* newAddr = realloc(ptr, size);
        if (size == 0) {
            return newAddr;
        } 
        if (newAddr == NULL) {
            fprintf(stderr, "Error allocating memory with realloc (from stdlib)");
            exit(-1);
        }
        Tuple* new = createTuple(newAddr, size); 
        setAlloc(new);
        insertTuple(new);
        

        return newAddr;
}

void memcheck537(void *ptr, size_t size) {
    //search the graph and check if overlaps already malloc'd. have clear errors
    //Cases:
    //Not there
    //There but free
    //There and size is valid
    //There and size is invalid
    void* check = contains(ptr);
    if (check == NULL) {
        fprintf(stderr, "Pointer passed to memcheck537 was not ever allocated\n");
        exit(-1);
    } 
    Tuple* container = lookupTuple(check);
    if (container->mal == 0) {
        fprintf(stderr, "Pointer passed to memcheck537 was previously freed\n");
        exit(-1);
    }
    if ((container->ptr + container->len) < (ptr + size)) {
        fprintf(stderr, "Size passed to memcheck537 was larger than allocated block\n");
        exit(-1);
    } 
}
