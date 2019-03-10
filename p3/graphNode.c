#include <stdio.h>
static const int INIT_BUF_SIZE = 4;
static const int LINE_SIZE = 1024;

graphNode *CreateGraphNode(graphNode parent, char* target, char** commands) {
    //Construct graphNode
    graphNode *node;
    node = malloc(sizeof(graphNode));
    node->target = target;
    node->parent = parent;

    node->commands = malloc(INIT_BUF_SIZE * sizeof(char*);
    for (int i = 0; i < INIT_BUF_SIZE; i++) {
        node->commands[i] = malloc(LINE_SIZE * sizeof(char));
    }

    node->children = malloc(sizeof(graphNode*) * INIT_BUF_SIZE);
    for (int i = 0; i < INIT_BUF_SIZE; i++) {
        node->children[i] = malloc(sizeof(graphNode));
    }


}

void expand_buf_children(graphNode node) {
    //expand buffer
    realloc(node->children, sizeof(node->children) * 2);     
}

void expand_buf_commands(graphNode node) {
    realloc(node->commands, sizeof(node->commands) * 2);
}
