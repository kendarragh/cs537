typedef struct Node {
    void* key;
    struct Node* parent, *left, *right;
    //0 = black, 1 = red
    int color;
    int size;
    Tuple* tuple;
}Node;

Node* newNode(void* key);
Node* lookup(void* key);
Tuple* lookupTuple(void* key);
void insertTuple(Tuple* t);
void* contains(void* ptr);
