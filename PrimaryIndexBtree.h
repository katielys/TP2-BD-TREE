#ifndef _BTREE_H_
#define _BTREE_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashFile.h"
//tem o tamanho de 4096
#define T  128 //(2m * sizeof(seek) + (2m-1)* sizeof(key) + (2m-1)* sizeof(address) <= 4084 -> (4096 - 12(other attributes)))

#define BTREE_OK 0
#define BTREE_ERR -1
#define HEADER sizeof(unsigned long)

typedef struct btree_node{
    int key_num; //4
    unsigned int self; //4;
    int parent;//4
    Hashing::Address adress[2 * T - 1];
    int key[2 * T - 1];
    int seek[2 * T]; //!< Order from Btree


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
std::pair<bool, Hashing::Address> btree_search(btree &tree, btree_node &node, int key);
btree createIndex(const char *file);
void loadRoot(btree &t,const char *file);
void saveRootOffset(btree &t);
unsigned int btree_end_seek_of_file(btree &tree);
#endif