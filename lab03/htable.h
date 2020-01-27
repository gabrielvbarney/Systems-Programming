#ifndef HTABLE_H_
#define HTABLE_H_

typedef struct Node {
    char ch;
    int freq;
    struct Node *left;
    struct Node *right;
    struct Node *next;
} Node;
    
typedef struct Code {
    char *code;
    char ch;
} Code;

void create_node_list();
Node *freq_list_to_nodes(int *asciiBuffer);
int comparator(const void *a, const void *b);
Node *create_tree();
void sort_nodes(Node *nodeList);
void sort_nodes2(Code *listOfCodes, int n);
Node *create_super_node(Node *a, Node *b);
void walk_the_tree(Node *root, char str[], int d);
void print_codes();

#endif 
