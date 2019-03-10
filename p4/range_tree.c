///////////////////////////////////////////////////////////////////////////
//// File:             range_tree.c
//// Semester:         537 fall 2018
////
//// Author:           Kenneth Darragh kdarragh2@wisc.edu
////                   Courtney Trescher trescher@wisc.edu
//// CS Login:         kenneth, courtney
//// Lecturer's Name:  Bart Miller
////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "tuple.h"

static const int RED = 1;
static const int BLACK = 0;

typedef struct Node {
	void* key;
	struct Node *parent, *left, *right;
        //0 = black, 1 = red
	int color;
	int size;
        Tuple* tuple;
}Node;

void fix(Node* node);
Node* lookup(void* key);
Node* lookupHelper(Node* n, void* key);
void print(Node* n);
void* contains(void* ptr);
Node* containsHelper(Node* n, void* ptr);
void insertTuple(Tuple* t);

struct Node *root; //global root?
Node *newNode(void* key) {
	struct Node *t = (Node*) malloc(sizeof(Node));
	t->key = key;
	t->parent = NULL;
	t->left = NULL;
	t->right = NULL;
        t->tuple = NULL;
	return t;
}

Node* binaryInsert(Node* node, Node* prev, void* key) {
	//Node *t = newNode(key);
	if (node == NULL) {
            Node* ret = newNode(key);
            ret->parent = prev;
            return ret;
	}
	if (node->key > key) {
            node->left = binaryInsert(node->left,node, key);
	    return node;
	} else if (node->key < key) {
	    node->right = binaryInsert(node->right, node, key);
	    return node;
	}	
        return node;
        //return NULL;
}

Node* insert(void* key) {
    //Empty tree
    if (root == NULL) {
        root = newNode(key);
        root->color = BLACK;
        return root;
    }

    //Non empty tree
    binaryInsert(root,NULL, key);
    Node* node = lookup(key);
    node->color = RED;

    if(node->parent == NULL) {
        return node;
    }
    //Red black property not violated, we are done
    if (node->parent->color == BLACK) {
        return node;
    }

    //We need to fix
    fix(node);
    return node;
}

void insertTuple(Tuple* t) {
    Node* n = lookup(t->ptr);
    if (n == NULL) {
        insert(t->ptr);
    } else {
        n->tuple = t;
    }
    n = lookup(t->ptr);
    n->tuple = t;
}

void fix(Node* node) {
    Node* sibling;
    Node* parent = node->parent;
    Node* grandparent = node->parent->parent;
    if (grandparent->left == parent) {
        sibling = grandparent->right;
    } else {
        sibling = grandparent->left;
    }
    
    int left = 0;
    if (grandparent->parent == NULL) {
        left = -1;
    } else if (grandparent->parent->left == grandparent) {
        left = 1;
    }

    //TODO recursive
    //sibling is black or null
    if ((sibling == NULL) || (sibling->color == BLACK)) {
        if (grandparent->left == parent) {
            //Case 1 or 2
            if (parent->left == node) {
                //Case 1
                if (root == grandparent) {
                    root = parent;
                }

                Node* tmp = parent->right;
                parent->right = grandparent;
                parent->parent = grandparent->parent;
                grandparent->parent = parent;
                grandparent->left = tmp;
                if (tmp != NULL) {
                    tmp->parent = grandparent;
                }
                grandparent->right = sibling;
                
                if (left != -1) {
                    if (left) {
                        parent->parent->left = parent;
                    } else {
                        parent->parent->right = parent;
                    }
                }

                grandparent->color = RED;
                parent->color = BLACK;

                //Fix names
            } else {
                //Case 2
                if (root == grandparent) {
                    root = node;
                }

                parent->right = NULL;
                grandparent->left = NULL;
                node->left = parent;
                node->right = grandparent;
                node->parent = grandparent->parent;
                grandparent->parent = node;
                parent->parent = node;

                if (left != -1) {
                    if (left) {
                        node->parent->left = node;
                    } else {
                        node->parent->right = node;
                    }
                }

                grandparent->color = RED;
                node->color = BLACK;
            }
        } else {
            //Case 3 or 4
            if (parent->right == node) {
                //Case 3
                if (root == grandparent) {
                    root = parent;
                }

                Node* tmp = parent->left; 
                parent->left = grandparent;
                parent->parent = grandparent->parent;
                grandparent->parent = parent;
                grandparent->right = tmp;
                if (tmp != NULL) {
                    tmp->parent = grandparent;
                }

                if (left != -1) {
                    if (left) {
                        parent->parent->left = parent;
                    } else {
                        parent->parent->right = parent;
                    }
                }

                grandparent->color = RED;
                parent->color = BLACK;
            } else {
                //Case 4
                if (root == grandparent) {
                    root = node;
                }

                node->left = grandparent;
                node->right = parent;
                node->parent = grandparent->parent;
                grandparent->right = NULL;
                grandparent->parent = node;
                parent->left = NULL;
                parent->parent = node;

                if (left != -1) {
                    if (left) {
                        node->parent->left = node;
                    } else {
                        node->parent->right = node;
                    }
                }

                grandparent->color = RED;
                node->color = BLACK;
            }
        } 
    //Sibling is red
    } else {
        if (grandparent != root) {
            grandparent->color = RED;
        }
        parent->color = BLACK;
        sibling->color = BLACK;

        if (grandparent->parent != NULL) {
            if (grandparent->parent->color == RED) {
                fix(grandparent);
            }
        }

    }
    return;
}

Node* lookup(void* key) {
    return lookupHelper(root, key);
}

Node* lookupHelper(Node* n, void* key) {
    if (n == NULL) {
        return n;
    }
    if (n->key == key) {
        return n;
    }

    if (n->key < key) {
        return lookupHelper(n->right, key);
    }
    return lookupHelper(n->left, key);
}

Tuple* lookupTuple(void* addr) {
    return lookup(addr)->tuple;
}

//0 = no
//key = yes
//other = is in range of this
void* contains(void* ptr) {
    Node* n = containsHelper(root, ptr);
    if (n == NULL) {
        return NULL;
    }
    return n->key;
}

Node* containsHelper(Node* n, void* key) {
    if (n == NULL) {
        return NULL;
    }
    if (n->key == key) {
        return n;
    }

    if (n->key < key) {
        if ((n->key + n->tuple->len) > key) {
            return n;
        }
        return containsHelper(n->right, key);
    }
    return containsHelper(n->left, key);
}

void print(Node* node) {
    if (node == NULL) {
        return;
    }
    if (node->left == NULL && node->right == NULL) {
        if (node->parent != NULL) {
            fprintf(stdout, "%p (%d) child of %p (%d)\n", node->key, node->color, node->parent->key, node->parent->color);
        } else {
            fprintf(stdout, "%p (%d) has no parent\n", node->key, node->color); 
        }
        return;
    } 
    if (node->left != NULL) {
        print(node->left);
    }
    if (node->right != NULL) {
        print(node->right);
    }
        if (node->parent != NULL) {
            fprintf(stdout, "%p (%d) child of %p (%x)\n", node->key, node->color, node->parent->key, node->parent->color);
        } else {
            fprintf(stdout, "%p (%d) has no parent\n", node->key, node->color); 
            printf("____________________________\n");
        }
}
