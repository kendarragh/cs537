///////////////////////////////////////////////////////////////////////////
//// File:             graph.c
//// Semester:         537 fall 2018
////
//// Author:           Kenneth Darragh kdarragh2@wisc.edu
////                   Courtney Trescher trescher@wisc.edu
//// CS Login:         kenneth, courtney
//// Lecturer's Name:  Bart Miller
////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "parse.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include "execute.h"

typedef struct timespec timespec;
int compare_time(timespec , timespec );
//Returns the size of a list
int get_list_size(Node** targets) {
    int size = 0;
    int i = 0;
    while(targets[i]->target != NULL) {
        size++;
        i++;
    }
    return size;
}

//When list is built, there is no knowledge of other nodes
//So children are stored as strings and no differentiation 
//between files and other targets
//
//convert_nodes converts this because now we have full knowledge
//and can set children
void convert_nodes(Node** list) {
    for (int i = 0; i < get_list_size(list); i++) {
        Node* curr = list[i];
        for (int j = 0; j < curr->num_dependencies; j++) {
            char* dep = curr->dependencies[j];
            int index = is_target(list, dep);
            if (index == -1) {
                //is file
                add_filename(curr, dep);
            } else {
                //is target
                add_child(curr, list[index]);
            }
        }
    }
}
                
//Creates and returns a graph given a list of targets and a root node
Graph* build_graph(Node** targets, Node* root) {
    convert_nodes(targets);

    Graph* g = malloc(sizeof(Graph));
    if (g == NULL) {
        fprintf(stderr, "Error allocating memory");
        exit(EXIT_FAILURE);
    }
    g->targets = targets;
    g->num_targets = get_list_size(targets);
    g->root = root;
    
    //Checks for cycles
    if (is_cyclic(g)) {
        fprintf(stderr, "Error: cycle detected\n");
        exit(EXIT_FAILURE);
    }
    return g;
}

//Returns true if child is a child of parent
int is_child(Node* parent, Node* child) {
    for (int i = 0; i < parent->num_children; i++) {
        if (parent->children[i] == child) {
            return 1;
        }
    }
    return 0;
}

//Check if a target should be updated
int check_file(Node* node) {
    struct stat target_stat; 

    //Target should be updates if it has no children
    if ((node->num_filenames == 0) && (node->num_children == 0)) {
            return 1;
    }

    //Target should be updated if stat can't find file
    if (stat(node->target, &target_stat) != 0) {
        return 1;
    }

    //Target should be updated if source is newer
    for (int i = 0; i < node->num_filenames; i++) {
        char* filename = node->filenames[i];
        struct stat cmp_stat;
        if (stat(filename, &cmp_stat) != 0) {
            continue;
        }
        if (compare_time(target_stat.st_mtim, cmp_stat.st_mtim)) {
            return 1;
        }
    }

    //Target should be updated if a child should be updated
    for (int i = 0; i < node->num_children; i++) {
        if (check_file(node->children[i]) == 1) {
            return 1;
        }
    }

    //Otherwise, target does not need to be updated
    return 0;
}

//return true if two before one
int compare_time(timespec one, timespec two) {
    if (one.tv_sec == two.tv_sec) {
        return two.tv_nsec > one.tv_nsec;
    } else {
        return two.tv_sec > one.tv_sec;
    }
}

//Checks if a string is another target, useful 
//before we've converted nodes
//If it is, returns index
int is_target(Node** list, char* string) {
    for(int i = 0; i < get_list_size(list); i++) {
        if (strcmp(list[i]->target, string) == 0) {
            return i;
        }
    }
    return -1;
}

//Traverses the graph with a post order traversal
int traverse_graph(Graph* g) {
    //Reset visited flags first
    reset_flags(g);
    return post_order(g->root);
}

//Traverses the graph with a post order traversal
int post_order(Node* curr) {
    for (int i = 0; i < curr->num_children; i++) {
        if (curr->children[i]->flagged == 0) {
            post_order(curr->children[i]);
        }
    }
    if (check_file(curr)) {
        for (int i = 0; i < curr->num_commands; i++) {
            tokenize(curr->commands[i]);
        }
    }
    curr->flagged = 1;
    return 0;
}

//Returns true if cycles are found
int is_cyclic(Graph* g) {
    Node** no_parents = malloc(sizeof(Node*) * g->num_targets);
    if (no_parents == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        exit(EXIT_FAILURE);
    } 

    //Make list of nodes with no parents
    int count = 0;
    for (int i = 0; i < g->num_targets; i++) {
        Node* curr = g->targets[i];
        int has_parent = 0;
        for (int j = 0; j < g->num_targets; j++) {
            Node* cmp = g->targets[j];
            if (is_child(cmp, curr)) {
                has_parent = 1;
            }
        }
        if (has_parent == 0) {
            no_parents[count] = curr;
            count++;
        }
    }
    
    //If theres no nodes w/o parents, its cyclic
    if (count == 0) {
        free(no_parents);
        return 1;
    } 

    //Start dfs at a root nodes
    for(int i = 0; i < count; i++) {
        if (dfs(no_parents[i]) == 1) {
            free(no_parents);
            return 1;
        }
        reset_flags(g);
    }
    //No cycles found, return false
    free(no_parents);
    return 0;
}

//DFS for searching for cycles
//Kind of redundant but useful to separate from traversing
int dfs(Node* node) {
    if (node -> flagged == -1) {
        return 1;
    } 
    node->flagged = -1;
    for (int i = 0; i < node->num_children; i++) {
        if (node->children[i]->flagged != 1) {
            if (dfs(node->children[i]) == 1) {
                return 1;
            }
        }
    }
    node->flagged = 1;
    return 0;
}

//Reset flags of all nodes before traversing
void reset_flags(Graph* g) {
    for (int i = 0; i < g->num_targets; i++) {
        g->targets[i]->flagged = 0;
    }
}


