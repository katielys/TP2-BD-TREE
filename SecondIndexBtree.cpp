
#include "SecondIndexBtree.h"

int writeSecond(BTreeS *tree, int seek, NodeS *node){
    if(fseek(tree->fp, seek, SEEK_SET) == -1)
        return BTREE_ERR;
    if(fwrite((void *)node, sizeof(NodeS), 1, tree->fp) == -1)
        return BTREE_ERR;
    return BTREE_OK;
}

BTreeS *createIndexSecondary(const char *file){
    FILE *fp = fopen(file, "w+");
    if(fp == nullptr)
        return nullptr;
    BTreeS *tree =  (BTreeS *)calloc(1, sizeof(BTreeS));
    if(tree == nullptr)
        return nullptr;
    tree->fp = fp;
    tree->root = createNewSecondNode();
    tree->root->self = 0;
    if(writeSecond(tree, tree->root->self, tree->root) == BTREE_ERR)
        return nullptr;
    return tree;
}
NodeS *createNewSecondNode(){
    NodeS *node = (NodeS*) malloc(sizeof(NodeS));
    node->self = -1;
    node->parent = -1;
    int i;
    for(i = 0; i < 2 * TS - 1; i++){
        node->key[i].title;
        strcpy(node->key[i].title,"");
        node->adress[i] = 0;
    }
    for(i = 0; i < 2 * TS; i++)
        node->seek[i] = -1;
    node->key_num = 0;
    return node;
}
