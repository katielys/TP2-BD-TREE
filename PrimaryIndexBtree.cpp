#include <iostream>
#include "PrimaryIndexBtree.h"

using namespace std;

int btree_read_disk(btree &tree, int seek, btree_node &node){
    if(fseek(tree.fp, seek, SEEK_SET) == -1){
        return BTREE_ERR;
    }

    if(fread(&node, sizeof(btree_node), 1, tree.fp) == -1)
        return BTREE_ERR;

    return BTREE_OK;
}

int btree_write_disk(btree &tree, int seek, btree_node &node){
    if(fseek(tree.fp, seek, SEEK_SET) == -1) {
        return BTREE_ERR;
    }

    if(fwrite(&node, sizeof(btree_node), 1, tree.fp) == -1) {
        return BTREE_ERR;
    }

    return BTREE_OK;
}

 int btree_key_index(btree_node &node, int key){
    int low = 0;
    int high = node.key_num -1;
    int middle = (low + high) / 2;
    while(low <= high){
        if(node.key[middle] == key){
            return middle;
        }
        else if(node.key[middle] > key) {
            high = middle - 1;
        }else
            low = middle + 1;
        middle = (low + high) / 2;
    }
    return high + 1;
}

unsigned int btree_end_seek_of_file(btree &tree){
    if(fseek(tree.fp, 0, SEEK_END) == -1)
        return BTREE_ERR;
    return ftell(tree.fp);
}



btree_node createNewNode(){
    btree_node node;
    node.self = -1;
    node.parent = -1;
    int i;
    for(i = 0; i < 2 * T - 1; i++){
        node.key[i] = 0;
        node.adress[i] = Hashing::Address();
    }
    for(i = 0; i < 2 * T; i++)
        node.seek[i] = -1;
    node.key_num = 0;
    return node;
}

btree createIndex(const char *file){
    FILE *fp = fopen(file, "w+");
    btree tree;

    if(fp == nullptr){
        throw "Cannot open primary index file";
    }

    tree.fp = fp;
    tree.root = createNewNode();
    tree.root.self = 0;
    if(btree_write_disk(tree, tree.root.self, tree.root) == BTREE_ERR) { // mudar pra retornar bool se for o caso
        throw "Cannot write on primary index file";
    }
    // writing 0 to avoid garbage
//    unsigned long rootOffset = 0;
//    fseek(fp, 0, SEEK_SET);
//    fwrite(&rootOffset, sizeof(unsigned long), 1, fp);

    return tree;
}

pair<bool, Hashing::Address> btree_search(btree &tree, btree_node &node, int key){
    short int countBlock = 1;
    Hashing::Address ad;
    //cout<< (*tree->root);
    node = tree.root;
    int key_index = btree_key_index(node, key);
    while(node.seek[0] != -1 && node.key[key_index] != key)
    {
        if(btree_read_disk(tree, node.seek[key_index], node) == BTREE_ERR) {
            return std::make_pair(false, ad);
        }
        key_index = btree_key_index(node, key);
        countBlock++;
    }

    if(node.key[key_index] == key) {
        cout<< "Numero de blocos de indice lidos:  "<< countBlock << endl;
        ad = node.adress[key_index];
        return std::make_pair(true, ad);
    }

    return std::make_pair(false, ad);
}

void loadRoot(btree &indexPrimary, const char *file){

    //btree indexPrimary;
    FILE *fp = fopen(file,"rb");
    fseek(fp, 0, SEEK_END);
    auto curPos = ftell(fp);
    fseek(fp, (curPos- sizeof(unsigned int)), SEEK_SET);
    btree_node node;
    unsigned int rootOffset = 0;
    fread(&rootOffset, sizeof(unsigned int), 1,fp);

    fseek(fp, rootOffset, SEEK_SET);
    fread(&node, sizeof(btree_node), 1,fp);

    indexPrimary.fp = fp;
    indexPrimary.root = node;
}


int addElement(btree &tree, int key, Hashing::Address adress){

    btree_node node;

    node = tree.root;

    if(btree_split(tree, node) == BTREE_ERR)
        return BTREE_ERR;

    int key_index = btree_key_index(node, key);

    while(node.seek[0] != -1) {

        if(btree_read_disk(tree, node.seek[key_index], node) == BTREE_ERR) {
            return BTREE_ERR;
        }

        if(btree_split(tree, node) == BTREE_ERR) {
            return BTREE_ERR;
        }

        key_index = btree_key_index(node, key);
    }

    int i;
    for(i = node.key_num; i > key_index; i--){
        node.key[i] = node.key[i - 1];
        node.adress[i] = node.adress[i - 1];
    }

    node.key[key_index] = key;
    node.adress[key_index] = adress;
    node.key_num++;

    if(btree_write_disk(tree, node.self, node) == BTREE_ERR) {
        return BTREE_ERR;
    }

    if(tree.root.self == node.self) {
        tree.root = node;
    }

    return BTREE_OK;
}

int btree_split(btree &tree, btree_node &node){
    if(node.key_num < 2 * T - 1) {
        return BTREE_OK;
    }

    btree_node brother = createNewNode();
    brother.self = btree_end_seek_of_file(tree);

    int save_key = node.key[T - 1];
    int i;

    for(i = T; i < node.key_num; i++){
        brother.key[i - T] = node.key[i];
        brother.adress[i - T] = node.adress[i];
    }

    if(node.seek[0] != -1)
        for(i = T; i < node.key_num + 1; i++)
        {
            brother.seek[i - T] = node.seek[i];
            btree_node child;
            if(btree_read_disk(tree, node.seek[i], child) == BTREE_ERR)
                return BTREE_ERR;

            child.parent = brother.self;

            if(btree_write_disk(tree, child.self, child) == BTREE_ERR)
                return BTREE_ERR;
        }

    node.key_num = brother.key_num = T - 1;

    btree_node parent;

    if(node.parent == -1) {
        // the actual node is root, thus we have to do all changes in the root by this way
        parent = createNewNode();
        tree.root = parent;
        tree.root.self = brother.self + sizeof(btree_node);
        node.parent = tree.root.self;

        int key_index = btree_key_index(tree.root, node.key[0]);

        for(i = tree.root.key_num; i > key_index; i--){
            tree.root.key[i] = tree.root.key[i - 1];
            tree.root.adress[i] = tree.root.adress[i - 1];
        }

        tree.root.key[i] = save_key;

        for(i = tree.root.key_num + 1; i > key_index + 1; i--) {
            tree.root.seek[i] = tree.root.seek[i - 1];
        }

        tree.root.seek[key_index] = node.self;
        tree.root.seek[key_index + 1] = brother.self;
        tree.root.key_num++;
        brother.parent = node.parent;

        btree_write_disk(tree, brother.self, brother);
        btree_write_disk(tree, tree.root.self, tree.root);
        btree_write_disk(tree, node.self, node);
        if(btree_read_disk(tree, tree.root.self, tree.root) == BTREE_ERR) {
            return BTREE_ERR;
        }
        node = tree.root;
        return BTREE_OK;

    } else if(btree_read_disk(tree, node.parent, parent) == BTREE_ERR) { // the node has a parent, read it from disk
        return BTREE_ERR;
    }
    // the actual isn't a root, so we proceed normaly, making changes in the parent read from the disk
    int key_index = btree_key_index(parent, node.key[0]);

    for(i = parent.key_num; i > key_index; i--){
        parent.key[i] = parent.key[i - 1];
        parent.adress[i] = parent.adress[i - 1];
    }

    parent.key[i] = save_key;

    for(i = parent.key_num + 1; i > key_index + 1; i--) {
        parent.seek[i] = parent.seek[i - 1];
    }

    parent.seek[key_index] = node.self;
    parent.seek[key_index + 1] = brother.self;
    parent.key_num++;
    brother.parent = node.parent;

    btree_write_disk(tree, parent.self, parent);
    btree_write_disk(tree, brother.self, brother);
    btree_write_disk(tree, node.self, node);
    if(btree_read_disk(tree, tree.root.self, tree.root) == BTREE_ERR) {
        return BTREE_ERR;
    }
    node = parent;
    return BTREE_OK;
}

void saveRootOffset(btree &t) {
    auto end = btree_end_seek_of_file(t);
    fseek(t.fp, end, SEEK_SET);
    auto check = fwrite(&t.root.self, sizeof(unsigned int), 1, t.fp);
}

btree_node::btree_node() {}

btree::btree() {}
