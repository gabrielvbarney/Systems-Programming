#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "htable.h"
#define SIZE 256

/*Node *new_node_list;*/
/*Node newNodeList[SIZE*SIZ*/
int nodeCount = 0;
int code_i = 0;
int str_i = 0;    
int ogD = 0;
char listOfCodes[SIZE];

Node *freq_list_to_nodes(int *asciiBuffer) {
    Node *nodeList = (Node*)malloc(sizeof(Node)*SIZE);
    int i;
    int current_val;
    int node_i = 0;
    for (i = 0; i < (SIZE-1); i++) {
        if (asciiBuffer[i] == 0) {
            continue;
        } else {
            Node *new_node = (Node*)malloc(sizeof(Node));
            current_val = asciiBuffer[i];
            new_node->ch = i;
            new_node->freq = current_val;
            new_node->left = NULL;
            new_node->right = NULL;
            new_node->next = NULL;
            nodeList[node_i] = *new_node;
            node_i++;
        }
    }
    sort_nodes(nodeList); 
    for (i = 0; i < node_i; i++) {
        nodeList[i].next = &nodeList[i + 1];
/*        printf("node %d, nodeCh: %c, nodeFreq: %d\n", i+1, 
        nodeList[i].ch, nodeList[i].freq);*/
    }
    return nodeList;
}

int comparator(const void *a, const void *b) {
    char *ch1 = ((Node*)a)->ch;
    char *ch2 = ((Node*)b)->ch;
    int freq1 = ((Node*)a)->freq;
    int freq2 = ((Node*)b)->freq;
    if (freq1 != freq2) {
        return freq1-freq2;
    }
    if (ch1 == 0 && ch2 == 0) {
        return 1;
    }
    return ch1-ch2;
}

int comparator2(const void *a, const void *b) {
    char *ch1 = ((Code*)a)->ch;
    char *ch2 = ((Code*)b)->ch;
    return ch1-ch2;
}

void sort_nodes(Node *nodeList) {
    qsort(nodeList, nodeCount, sizeof(Node), comparator);
} 

void sort_nodes2(Code *hCodeArray, int n) {
    qsort(hCodeArray, n, sizeof(Code), comparator2);
} 

Node *create_super_node(Node *a, Node *b) {
    Node *super_node = (Node*)malloc(sizeof(Node));
    int s_node_val = a->freq + b->freq;   
    super_node->freq = s_node_val;
    super_node->ch = '\0';
    super_node->left = a;
    super_node->right = b;
    super_node->next = NULL;  
/*    printf("superFreq: %d, leftCh: %c, rightCh: %c\n", 
    super_node->freq, super_node->left->ch, super_node->right->ch);
    printf("\n"); */
    return super_node;
}

Node *create_tree(Node *nodeList) { 
    int i;
    Node *superNode;
    int n = nodeCount;
    int ogN = n;
    Node *temp;
    Node *currNode = &nodeList[0];
    Node *ogNextNext = currNode->next->next;
    int tracker = 0;
    int ogFlag = 1;
    for (i = 0; i < n; i++) {
        if (n - i == 1) {
            nodeList[i].next = NULL;
        } else {
            nodeList[i].next = &nodeList[i + 1];
        }
    }
    currNode = &nodeList[0];
    ogNextNext = currNode->next->next;
    while (n != 1) {
        if (ogFlag == 2) {
            currNode = ogNextNext;
            superNode = create_super_node(currNode, currNode->next);
        } else { 
            superNode = create_super_node(currNode, currNode->next);
            if (ogNextNext) {
                currNode = ogNextNext;
            }
            else if (!currNode->next->next) {
                return superNode;
            }
        }
        i = tracker;
        ogFlag = 0;
        while (i < ogN) {
            if (!currNode->next) {
                currNode->next = superNode;
                if (i == tracker) {
                    ogFlag = 2;
                    break;
                } else {
                    currNode = ogNextNext;
                    ogFlag = 0;
                    break;
                }
            }
            if (i == tracker) {
                if ((superNode->freq < currNode->freq) || ((superNode->freq == 
                        currNode->freq) && (superNode->ch == currNode->ch))) {
                    superNode->next = currNode;
                    currNode = superNode;
                    break;       
                }
                if ((superNode->freq == currNode->freq) && 
                        (superNode->ch < currNode->ch)) {
                    superNode->next = currNode;
                    currNode = superNode;
                    break;       
                } /*else {
                    currNode = currNode->next;
                }*/
            } 
            if (((superNode->freq > currNode->freq) && ((superNode->freq <= 
                    currNode->next->freq) && (superNode->ch <= 
                    currNode->next->ch))) || ((superNode->ch <= currNode->ch) 
                    && (superNode->freq == currNode->freq))){
                temp = currNode->next;
                currNode->next = superNode;
                superNode->next = temp;
                currNode = ogNextNext; 
                break;   
            } 
            else {
                currNode = currNode->next;
            }
            i++;    
        }
        if (ogFlag == 0) { 
            ogNextNext = currNode->next->next;
        }
        n--;
        tracker++; 
    }
    return superNode; 
} 

void walk_the_tree(Node *root, char str[], int d) {
    int i;
    char nodeCh;
    if (root) {
        if (root->left) {
            str[d] = '0';
            str[d+1] = '\0';
            walk_the_tree(root->left, str, d + 1);
        }
        if (root->right) {
            str[d] = '1';
            str[d+1] = '\0';
            walk_the_tree(root->right, str, d + 1); 
        }
        else {
            nodeCh = root->ch;
            for (i = 0; i <= d; i++) {
                if (i == d) {
                    listOfCodes[ogD] = nodeCh;
                    ogD += 1;
                } else {
                    listOfCodes[ogD] = str[i];
                    ogD += 1;
                }
            }
            return;
        }
    }
}     

void print_codes() {
    int i;
    int code_idx = 0;
    int code_array_idx = 0;
    int oneCharHexDown = 9;
    int oneCharHexUp = 16;
    int transformer = 55;
    char toLowerCh;
    Code *hCode = (Code*)malloc(sizeof(Code));
    Code *hCodeArray = (Code*)malloc(sizeof(Code)*SIZE);
    char *code = (char*)malloc(sizeof(char)*SIZE);


    for (i = 0; i <= ogD; i++) {
        if (listOfCodes[i] == '0' || listOfCodes[i] == '1') {
            code[code_idx] = listOfCodes[i];
            code_idx += 1;
        } else {
            hCode->code = code;
            hCode->ch = listOfCodes[i];
            hCodeArray[code_array_idx] = *hCode;
            code_idx = 0;
            code_array_idx += 1;
            code = (char*)malloc(sizeof(char)*SIZE);
            hCode = (Code*)malloc(sizeof(Code));
        }
    }
    sort_nodes2(hCodeArray, nodeCount);
    for (i = 0; i < nodeCount; i++) {
        if (hCodeArray[i].ch == 0) {
            continue;
        } else if ((((int)hCodeArray[i].ch < oneCharHexUp)) && 
                (((int)hCodeArray[i].ch > oneCharHexDown))) {
            toLowerCh = tolower(hCodeArray[i].ch +transformer);
            printf("0x0%c: %s\n", toLowerCh, hCodeArray[i].code);
        } else if ((int)hCodeArray[i].ch <= oneCharHexDown ) {
            printf("0x0%x: %s\n", hCodeArray[i].ch, hCodeArray[i].code);
        } else if (hCodeArray[i].ch) {
            printf("0x%x: %s\n", hCodeArray[i].ch, hCodeArray[i].code);
        }
    }
}

int main(int argc, const char* argv[]) {
    FILE *file;
    int c;
    int i;
    Node *root;
    Node *nodeList;
    int asciiBuffer[SIZE];
    int d = 0;
    char str[SIZE];
    file = fopen(argv[1], "r");  
    if (!file) {
        printf("File not found.\n");
        exit(1);
    }
    c = getc(file);
    
    if (c == EOF) {
        exit(0);
    }
    for (i = 0; i < (SIZE + 1); i++) {
        asciiBuffer[i] = 0;
    }
    while (c != EOF) {
        asciiBuffer[c] += 1;
        if (asciiBuffer[c] == 1) {
            nodeCount++;
        }
        c = getc(file);
    }
    nodeList = freq_list_to_nodes(asciiBuffer);
    if (nodeCount == 1) {
        printf("0x%x: \n", nodeList[0].ch);
        return 0;
    }
    root = create_tree(nodeList);
    walk_the_tree(root, str, d);
    print_codes();
    return 0;
}

