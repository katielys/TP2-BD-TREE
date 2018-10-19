
#ifndef TP2_BD_TREE_SECONDINDEXBTREE_H
#define TP2_BD_TREE_SECONDINDEXBTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 300
#define TS 13

#define BTREE_OK 0
#define BTREE_ERR -1
typedef struct arraychar{
    char title[MAXLEN];
}arraychar;

typedef struct NodeS{
    int key_num;
    unsigned long adress [2 * TS - 1];
    arraychar key[2 * TS - 1];
    int seek[2 * TS]; //!< Order from Btree
    int self;
    int parent;
}NodeS;

typedef struct BTreeS{
    NodeS *root;
    FILE *fp;
}BTreeS;
int writeSecond(BTreeS *tree, int seek, NodeS *node);
BTreeS *createIndexSecondary(const char *file);
NodeS *createNewSecondNode();

#endif //TP2_BD_TREE_SECONDINDEXBTREE_H
