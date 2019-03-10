#include "node.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>


Node* CreateNode(char* target) {
    Node *n;
    n = malloc(sizeof(Node));
    if (n == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        exit(-1);
    }

    n->target = target;
    n->num_dependencies = 0;
    n->num_commands = 0;

    n->dependencies = malloc(sizeof(char*) * BUF_SIZE);
    n->commands = malloc(sizeof(char*) * BUF_SIZE);
    if ((n->dependencies == NULL) || (n->commands == NULL)) {
        fprintf(stderr, "Error allocating memory\n");
        exit(-1);
    }

    n->size_dependencies = BUF_SIZE;
    n->size_commands = BUF_SIZE;
    
    n->filenames = malloc(sizeof(char*) * BUF_SIZE);
    n->children = malloc(sizeof(char*) * BUF_SIZE);
    if ((n->filenames == NULL) || (n->children == NULL)) {
        fprintf(stderr, "Error allocating memory\n");
        exit(-1);
    }
    n->num_filenames = 0;
    n->num_children = 0;
    n->size_filenames = BUF_SIZE;
    n->size_children = BUF_SIZE;
    n->must_remake = 1;
    n->flagged = 0;

    return n;
}

void add_dependency(Node* n, char* dependency) {
    if (n->num_dependencies == n->size_dependencies) {
        n = expand_dependencies(n);
        n->size_dependencies *= 2;
    }

    n->dependencies[n->num_dependencies] = dependency;
    n->num_dependencies++;
}    

void add_command(Node* n, char* command) {
    if (n->num_commands == n->size_commands) {
        n = expand_commands(n);
        n->size_commands *= 2;
    }

    n->commands[n->num_commands] = command;
    n->num_commands++;
}

void add_filename(Node* n, char* file) {
    if (n->num_filenames == n->size_filenames) {
        n = expand_filenames(n);
        n->size_filenames *= 2;
    }

    n->filenames[n->num_filenames] = file;
    n->num_filenames++;
}

void add_child(Node* n, Node* child) {
    if (n->num_children == n->size_children) {
        n = expand_children(n);
        n->size_children *= 2;
    }

    n->children[n->num_children] = child;
    n->num_children++;
}

Node* expand_dependencies(Node* n) {
    char** tmp;
    tmp = realloc(n->dependencies, n->size_dependencies * 2 * sizeof(char*));
    if (tmp == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        exit(-1);
    }
    n->dependencies = tmp;
    return n;
}

Node* expand_commands(Node* n) {
    char** tmp;
    tmp = realloc(n->commands, n->size_commands * 2 * sizeof(char*));
    if (tmp == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        exit(-1);
    }
    n->commands = tmp;
    return n;
}


Node* expand_filenames(Node* n) {
    char** tmp;
    tmp = realloc(n->filenames, n->size_filenames * 2 * sizeof(char*));
    if (tmp == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        exit(-1);
    }
    n->filenames = tmp;
    return n;
}

Node* expand_children(Node* n) {
    Node** tmp;
    tmp = realloc(n->children, n->size_children * 2 * sizeof(Node*));
    if (tmp == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        exit(-1);
    }
    n->children = tmp;
    return n;
}

void print_node(Node* n) {
    printf("%s: ", n->target);
    for (int i = 0; i < n->num_dependencies; i++) {
        printf("%s ", n->dependencies[i]);
    }
    printf("\n");
    for (int i = 0; i < n->num_commands; i++) {
        printf("\t%s\n", n->commands[i]);
    }
}
