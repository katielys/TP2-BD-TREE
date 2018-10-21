#ifndef TP2_BD_TREE_SECONDINDEXBTREE_H
#define TP2_BD_TREE_SECONDINDEXBTREE_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashFile.h"
#define MAXLEN 300
#define TS 6

//node tem 3492 de tamanho


using namespace std;

#define BTREE_OK 0
#define BTREE_ERR -1

typedef struct arraychar{
    char title[MAXLEN];
}arraychar;

typedef struct NodeS{
    int count;
    Hashing::Address adress [2 * TS - 1];
    arraychar key[2 * TS - 1];
    int seek[2 * TS]; //!< Order from Btree
    int self;
    int parent;
    NodeS();
}NodeS;
typedef struct BTreeS{
    NodeS root;
    FILE *fp;
    BTreeS();
}BTreeS;


//TODO colocar em classe depois
int addElementS(BTreeS &tree, char *key, Hashing::Address adress);
int btree_splitS(BTreeS &tree, NodeS &node);
std::pair<bool, Hashing::Address> btree_searchS(BTreeS &tree, NodeS &node, char* key);
BTreeS createSecondIndex(const char *file);
void loadRootS(BTreeS &t,const char *file);
void saveRootOffsetSecond(BTreeS &t);
int readSecond(BTreeS &tree, int seek, NodeS &node);
int writeSecond(BTreeS &tree, int seek, NodeS &node);
int Kindex(NodeS &node, char *key);
unsigned int endFilseSecond(BTreeS &tree);

#endif
