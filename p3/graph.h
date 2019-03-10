struct graphNode {
    graphNode ** children;
    graphNode * parent;
    char* target;
    char** commands;
} 

//build the graph using parse.c
graphNode build_graph(char* filename);

//Post-order traversal
int traverse_graph(graphNode root);

//Check if a file has been changed
int check_file(FILE* file);
