#ifndef TP2_BD_TREE_SECONDINDEXBTREE_H
#define TP2_BD_TREE_SECONDINDEXBTREE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashFile.h"
//tem o tamanho de 4076
#define TS 102 //(2m * sizeof(seek) + (2m-1)* sizeof(key) + (2m-1)* sizeof(address) <= 4084 -> (4096 - 12(other attributes)))

#define BTREE_OK 0
#define BTREE_ERR -1
#define HEADER sizeof(unsigned long)

typedef struct NodeS{
    int count;
    Hashing::Address adress[2 * TS - 1];
    int key[2 * TS - 1];
    int seek[2 * TS]; //!< Order from Btree
    unsigned int self;
    int parent;

    NodeS();
}NodeS;

typedef struct SecondIndex{
    NodeS root;
    FILE *fp;

    SecondIndex();
}SecondIndex;

int stringToIndexNumberf(string toIndex);
//TODO vou colocar em classe deopois
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
