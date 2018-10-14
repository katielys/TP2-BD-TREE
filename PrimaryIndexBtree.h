#ifndef TP2_BD_TREE_PRIMARYINDEXBTREE_H
#define TP2_BD_TREE_PRIMARYINDEXBTREE_H
using namespace std;

#include "Article.h"
#include <iostream>

#define MAX_KEY  66 //NAO SEI AINDA QUANTOS CHAVES DAO POR bloco

typedef struct nodePrimaryIndex{
    int *keys;
    int *child;
} nodePrimaryIndex;

class PrimaryIndexBtree {
    public:
    nodePrimaryIndex *root;
    FILE pfile;
    PrimaryIndexBtree *initTree();
    PrimaryIndexBtree *createTree(FILE *fp);
    bool insertBtree(PrimaryIndexBtree *indexBtree, int key);
    int readBtreeFromDisk(PrimaryIndexBtree * indexBtree,int seekPosition );
    nodePrimaryIndex* createNode();
    int writeDiskBtree(PrimaryIndexBtree *indexBtree,int seekPosition , nodePrimaryIndex *node);

};




#endif //TP2_BD_TREE_PRIMARYINDEXBTREE_H
