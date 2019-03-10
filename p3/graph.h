#include "node.h"
typedef struct Graph {
    Node** targets;
    Node* root;
    int num_targets;
}Graph;

Graph* build_graph(Node** targets, Node* root);

int traverse_graph(Graph* g);

int check_file(Node* node);

int is_child(Node* parent, Node* child);

void convert_nodes(Node** list);

int is_target(Node** list, char* string);

int post_order(Node* curr);

int is_cyclic(Graph* g);

int dfs(Node* node);

void reset_flags(Graph* g);
