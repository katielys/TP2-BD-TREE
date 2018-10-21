#ifndef _BTREE_H_
#define _BTREE_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashFile.h"
#define T 136 //(2m * sizeof(seek) + (2m-1)* sizeof(key) + (2m-1)* sizeof(address) <= 4084 -> (4096 - 12(other attributes)))

#define BTREE_OK 0
#define BTREE_ERR -1

typedef struct btree_node{
    int key_num;
    Hashing::Address adress[2 * T - 1];
    int key[2 * T - 1];
    int seek[2 * T]; //!< Order from Btree
    int self;
    int parent;

    btree_node();
}btree_node;

typedef struct btree{
    btree_node root;
    FILE *fp;

    btree();
}btree;

//TODO vou colocar em classe deopois
int addElement(btree &tree, int key, Hashing::Address adress);
int btree_split(btree &tree, btree_node &node);
btree_node btree_search(btree &tree, int key);
btree createIndex(const char *file);
void loadRoot(btree &t,const char *file);
int btree_key_index(btree_node &node, int key);
#endif