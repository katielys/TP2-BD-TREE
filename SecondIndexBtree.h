#ifndef TP2_BD_TREE_SECONDINDEXBTREE_H
#define TP2_BD_TREE_SECONDINDEXBTREE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashFile.h"
///\file SecondIndexBtree.h
//tem o tamanho de 4096
#define TS 128 //(2m * sizeof(seek) + (2m-1)* sizeof(key) + (2m-1)* sizeof(address) <= 4084 -> (4096 - 12(other attributes)))

#define BTREE_OK 0
#define BTREE_ERR -1


typedef struct NodeS{
    int count;//4
    unsigned int self;
    int parent;
    Hashing::Address adress[2 * TS - 1] ;//12 bytes;
    int key[2 * TS - 1];
    int seek[2 * TS]; //!< Order from Btree


    NodeS();
}NodeS;

typedef struct SecondIndex{
    NodeS root; ///!< raiz do indice secundaag
    FILE *fp; ///!< ponteiro do arquivo

    SecondIndex();
}SecondIndex;
int endFilseSecond(SecondIndex &tree);
int stringToIndexNumberf(string toIndex);
int readSecond(SecondIndex &tree, int seek, NodeS &node);
int writeSecond(SecondIndex &tree, int seek, NodeS &node);
NodeS createNewNodeSecond();
int addElementSecondIndex(SecondIndex &tree, int key, Hashing::Address adress);
int splitSecond(SecondIndex &tree, NodeS &node);
pair<bool, Hashing::Address> searchAtSecond(SecondIndex &tree, NodeS &node, int key);
SecondIndex createSecondIndex(const char *file);
void loadSecondRoot(SecondIndex &t,const char *file);
void saveSecondRootOffset(SecondIndex &t);

#endif
