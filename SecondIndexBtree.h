
#ifndef TP2_BD_TREE_SECONDINDEXBTREE_H
#define TP2_BD_TREE_SECONDINDEXBTREE_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 300
#define TS 13


using namespace std;
#define BTREE_OK 0
#define BTREE_ERR -1
typedef struct arraychar{
    char title[MAXLEN];
}arraychar;

typedef struct NodeS{
    int count;
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

//TODO colocar em classe depois
int writeSecond(BTreeS *tree, int seek, NodeS *node);
BTreeS *createIndexSecondary(const char *file);
NodeS *createNewSecondNode();
int readFromDiskS(BTreeS *tree, int seek, NodeS *node);
int Kindex(NodeS *node, char *key);
int seekEndFile(BTreeS *tree);
BTreeS *createSecondaryIndex(const char *file);
NodeS *searchAtBtreeS(BTreeS *tree, char *key);
void loadRootSecondIndex(BTreeS *indexSecundary, const char *file);
int addElementSecondIndex(BTreeS *tree, char *key, unsigned long adress);
short int SplitSecond(BTreeS *tree, NodeS *node);


#endif
