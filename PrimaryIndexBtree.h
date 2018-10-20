#ifndef _BTREE_H_
#define _BTREE_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define T 126

#define BTREE_OK 0
#define BTREE_ERR -1

typedef struct btree_node{
    int key_num;
    unsigned long adress [2 * T - 1];
    int key[2 * T - 1];
    int seek[2 * T]; //!< Order from Btree
    int self;
    int parent;
}btree_node;

typedef struct btree{
    struct btree_node *root;
    FILE *fp;
}btree;
//TODO vou colocar em classe deopois
int addElement(btree *tree, int key, unsigned long adress);
int btree_split(btree *tree, btree_node *node);
btree_node *btree_search(btree *tree, int key);
btree *createIndex(const char *file);
void loadRoot(btree *t,const char *file);

#endif