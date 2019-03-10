typedef struct Node {
    char* target;
    char** dependencies;
    int num_dependencies;
    int size_dependencies;

    char** commands;
    int num_commands;
    int size_commands;

    char** filenames;
    int num_filenames;
    int size_filenames;

    struct Node** children;
    int num_children;
    int size_children;

    int must_remake;
    int flagged;
}Node;

Node* CreateNode(char* target);
Node* expand_dependencies(Node* node);
Node* expand_commands(Node* node);
void add_dependency(Node* n, char* dependency);
void add_command(Node* n, char* command);
void print_node(Node* n);


Node* expand_filenames(Node* node);
Node* expand_children(Node* node);
void add_filename(Node* n, char* file);
void add_child(Node* parent, Node* child);
