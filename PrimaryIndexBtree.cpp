
#include <iostream>
#include "PrimaryIndexBtree.h"
using namespace std;
int btree_read_disk(btree *tree, int seek, btree_node *node){
    if(fseek(tree->fp, seek, SEEK_SET) == -1){
        return BTREE_ERR;
    }

    char *read_buf =  (char * )calloc(1, sizeof(btree_node));
    if(fread(read_buf, sizeof(btree_node), 1, tree->fp) == -1)
        return BTREE_ERR;
    memcpy(node, read_buf, sizeof(btree_node));
    return BTREE_OK;
}

int btree_write_disk(btree *tree, int seek, btree_node *node){
    if(fseek(tree->fp, seek, SEEK_SET) == -1)
        return BTREE_ERR;
    if(fwrite((void *)node, sizeof(btree_node), 1, tree->fp) == -1)
        return BTREE_ERR;
    return BTREE_OK;
}

 int btree_key_index(btree_node *node, int key){
    int low = 0;
    int high = 2*T-3;
    int middle = (low + high) / 2;
    while(low <= high){
        if(node->key[middle] == key) {
            return middle;
        }
        else if(node->key[middle] > key) {
            high = middle - 1;
        }else
            low = middle + 1;
        middle = (low + high) / 2;
    }
    return high + 1;
}

int btree_end_seek_of_file(btree *tree){
    if(fseek(tree->fp, 0, SEEK_END) == -1)
        return BTREE_ERR;
    return ftell(tree->fp);
}



btree_node *createNewNode(){
    btree_node *node = (btree_node*) malloc(sizeof(btree_node));
    node->self = -1;
    node->parent = -1;
    int i;
    for(i = 0; i < 2 * T - 1; i++){
        node->key[i] = 0;
        node->adress[i] = Hashing::Address();
    }
    for(i = 0; i < 2 * T; i++)
        node->seek[i] = -1;
    node->key_num = 0;
    return node;
}

btree *createIndex(const char *file){
    FILE *fp = fopen(file, "w+");
    if(fp == NULL)
        return NULL;
    btree *tree =  (btree *)calloc(1, sizeof(btree));
    if(tree == NULL)
        return NULL;
    tree->fp = fp;
    tree->root = createNewNode();
    tree->root->self = 0;
    if(btree_write_disk(tree, tree->root->self, tree->root) == BTREE_ERR)
        return NULL;
    return tree;
}

btree_node *btree_search(btree *tree, int key){
    short int countBlock = 1;
    btree_node *node = (btree_node *)calloc(1, sizeof(btree_node));
    if(node == NULL)
        return nullptr;
    //cout<< (*tree->root);
    *node = *tree->root;
    int key_index = btree_key_index(node, key);
    while(node->seek[0] != -1 && node->key[key_index] != key)
    {
        if(btree_read_disk(tree, node->seek[key_index], node) == BTREE_ERR)
            return nullptr;
        key_index = btree_key_index(node, key);
        countBlock++;
    }
    cout<< "Numero de blocos de indice lidos:  "<< countBlock << endl;
    if(node->key[key_index] == key)
        return node;
    return nullptr;
}

void loadRoot(btree *indexPrimary, const char *file){


    //btree indexPrimary;
    FILE *fp = fopen(file,"rb");
    fseek(fp, 0, SEEK_SET);
    btree_node *node =  (btree_node * )calloc(1, sizeof(btree_node));

    char *read_buf =  (char * )calloc(1, sizeof(btree_node));
    fread(read_buf, sizeof(btree_node), 1,fp);

    memcpy(node, read_buf, sizeof(btree_node));
    indexPrimary->fp = fp;
    indexPrimary->root = node;
}
int addElement(btree *tree, int key, Hashing::Address adress){
    btree_node *node = (btree_node *) calloc(1, sizeof(btree_node));
    if(node == NULL)
        return BTREE_ERR;

    *node = *tree->root;

    if(btree_split(tree, node) == BTREE_ERR)
        return BTREE_ERR;

    int key_index = btree_key_index(node, key);

    while(node->seek[0] != -1){
        if(btree_read_disk(tree, node->seek[key_index], node) == BTREE_ERR)
            return BTREE_ERR;
        if(btree_split(tree, node) == BTREE_ERR)
            return BTREE_ERR;
        key_index = btree_key_index(node, key);
    }

    int i;
    for(i = node->key_num; i > key_index; i--){
        node->key[i] = node->key[i - 1];
        node->adress[i] = node->adress[i - 1];
    }

    node->key[key_index] = key;
    node->adress[key_index] = adress;
    node->key_num++;
    if(btree_write_disk(tree, node->self, node) == BTREE_ERR)
        return BTREE_ERR;
    if(tree->root->self == node->self)
        *tree->root = *node;

    return BTREE_OK;
}

int btree_split(btree *tree, btree_node *node){
    if(node->key_num < 2 * T - 1)
        return BTREE_OK;

    btree_node *brother = createNewNode();
    brother->self = btree_end_seek_of_file(tree);

    int save_key = node->key[T - 1];
    int i;

    for(i = T; i < node->key_num; i++){
        brother->key[i - T] = node->key[i];
        brother->adress[i - T] = node->adress[i];
    }

    if(node->seek[0] != -1)
        for(i = T; i < node->key_num + 1; i++)
        {
            brother->seek[i - T] = node->seek[i];
            btree_node *child = (btree_node *) calloc(1, sizeof(btree_node));
            if(child == NULL)
                return BTREE_ERR;
            if(btree_read_disk(tree, node->seek[i], child) == BTREE_ERR)
                return BTREE_ERR;
            child->parent = brother->self;
            if(btree_write_disk(tree, child->self, child) == BTREE_ERR)
                return BTREE_ERR;
        }

    node->key_num = brother->key_num = T - 1;
    btree_node *parent = (btree_node *)calloc(1, sizeof(btree_node));
    if(parent == NULL)
        return BTREE_ERR;
    if(node->parent == -1) {
        parent = createNewNode();
        tree->root = parent;
        parent->self = brother->self + sizeof(btree_node);
        node->parent = parent->self;
    }
    else
    if(btree_read_disk(tree, node->parent, parent) == BTREE_ERR)
        return BTREE_ERR;
    int key_index = btree_key_index(parent, node->key[0]);
    for(i = parent->key_num; i > key_index; i--){
        parent->key[i] = parent->key[i - 1];
        parent->adress[i] = parent->adress[i - 1];
    }
    parent->key[i] = save_key;
    for(i = parent->key_num + 1; i > key_index + 1; i--)
        parent->seek[i] = parent->seek[i - 1];
    parent->seek[key_index] = node->self;
    parent->seek[key_index + 1] = brother->self;
    parent->key_num++;
    brother->parent = node->parent;

    btree_write_disk(tree, parent->self, parent);
    btree_write_disk(tree, brother->self, brother);
    btree_write_disk(tree, node->self, node);
    if(btree_read_disk(tree, tree->root->self, tree->root) == BTREE_ERR)
        return BTREE_ERR;
    *node = *parent;
    return BTREE_OK;
}
