
#include "SecondIndexBtree.h"

int writeSecond(BTreeS *tree, int seek, NodeS *node) {
    if(fseek(tree->fp, seek, SEEK_SET) == -1)
        return BTREE_ERR;
    if(fwrite((void *)node, sizeof(NodeS), 1, tree->fp) == -1)
        return BTREE_ERR;
    return BTREE_OK;
}

NodeS *createNewSecondNode() {
    auto *node = (NodeS*) malloc(sizeof(NodeS));
    node->self = -1;
    node->parent = -1;
    int i;
    for(i = 0; i < 2 * TS - 1; i++){
        strcpy(node->key[i].title,"");
        node->adress[i] = 0;
    }
    for(i = 0; i < 2 * TS; i++){
        node->seek[i] = -1;
    }
    node->count = 0;
    return node;
}

BTreeS *createIndexSecondary(const char *file) {
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

int readFromDiskS(BTreeS *tree, int seek, NodeS *node) {
    if(fseek(tree->fp, seek, SEEK_SET) == -1){
        return BTREE_ERR;
    }

    char *read_buf =  (char * )calloc(1, sizeof(NodeS));
    if(fread(read_buf, sizeof(NodeS), 1, tree->fp) == -1)
        return BTREE_ERR;
    memcpy(node, read_buf, sizeof(NodeS));
    return BTREE_OK;
}



int Kindex(NodeS *node, char *key) {
    int low = 0;
    int high = 2*TS-2;
    int middle = (low + high) / 2;
    while(low <= high){
        if(strcmp(node->key[middle].title,key)==0)
            return middle;
        else if(strcmp(node->key[middle].title,key)> 0)
            high = middle - 1;
        else
            low = middle + 1;
        middle = (low + high) / 2;
    }
    return high + 1;
}
///param BTreeS *tree -> the secondary Index pointer
int seekEndFile(BTreeS *tree) { //!< Function to know location of pointers at Index file
    if(fseek(tree->fp, 0, SEEK_END) == -1){
        return BTREE_ERR;
    }

    return ftell(tree->fp);
}
///param *file is the name of file that need to be create
BTreeS *createSecondaryIndex(const char *file) { //!< Function used to build the Secondary indexfile
    FILE *fp = fopen(file, "w+");
    if(fp == nullptr){
        return nullptr;
    }

    BTreeS *tree =  (BTreeS *)calloc(1, sizeof(BTreeS));


    if(tree == nullptr){
        return nullptr;
    }

    tree->fp = fp;
    tree->root = createNewSecondNode();
    tree->root->self = 0;
    if(writeSecond(tree, tree->root->self, tree->root) == BTREE_ERR)
        return nullptr;
    return tree;
}

NodeS *searchAtBtreeS(BTreeS *tree, char * key) {
    short int countBlock = 1;
    NodeS *node = (NodeS *)calloc(1, sizeof(NodeS));
    if(node == nullptr){
        return nullptr;
    }

    *node = *tree->root;
    int key_index = Kindex(node, key);
    while(node->seek[0] != -1 && strcmp(node->key[key_index].title,key)!=0){
        if(readFromDiskS(tree, node->seek[key_index], node) == BTREE_ERR)
            return nullptr;
        key_index = Kindex(node, key);
        countBlock++;
    }
    cout<< "Numero de blocos lidos:  "<< countBlock << endl;
    return strcmp(node->key[key_index].title, key) == 0 ? node : nullptr;

}

void loadRootSecondIndex(BTreeS *indexSecundary, const char *file) {

    FILE *fp = fopen(file,"rb");
    fseek(fp, 0, SEEK_SET);
    NodeS *node =  (NodeS * )calloc(1, sizeof(NodeS));

    char *read_buf =  (char * )calloc(1, sizeof(NodeS));
    fread(read_buf, sizeof(NodeS), 1,fp);

    memcpy(node, read_buf, sizeof(NodeS));
    indexSecundary->fp = fp;
    indexSecundary->root = node;
}

int addElementSecondIndex(BTreeS *tree, char *key, unsigned long adress) {
    NodeS *node = (NodeS *) calloc(1, sizeof(NodeS));
    if(node == NULL)
        return BTREE_ERR;
    *node = *tree->root;
    if(SplitSecond(tree, node) == BTREE_ERR)
        return BTREE_ERR;
    int key_index = Kindex(node, key);
    while(node->seek[0] != -1){
        if(readFromDiskS(tree, node->seek[key_index], node) == BTREE_ERR)
            return BTREE_ERR;
        if(SplitSecond(tree, node) == BTREE_ERR)
            return BTREE_ERR;
        key_index = Kindex(node, key);
    }
    int i;
    for(i = node->count; i > key_index; i--){
        node->key[i] = node->key[i - 1];
        node->adress[i] = node->adress[i - 1];
    }
    strcpy(node->key[key_index].title,key);
    node->count++;
    if(writeSecond(tree, node->self, node) == BTREE_ERR)
        return BTREE_ERR;
    if(tree->root->self == node->self)
        *tree->root = *node;
    return BTREE_OK;
}

short int SplitSecond(BTreeS *tree, NodeS *node) {
    if(node->count < 2 * TS - 1){
        return BTREE_OK;
    }

    NodeS *brother = createNewSecondNode();
    brother->self = seekEndFile(tree);
    char * save_key ;
    strcpy(save_key, node->key[TS - 1].title);
    int i;
    for(i = TS; i < node->count; i++){
        brother->key[i - TS] = node->key[i];
        brother->adress[i - TS] = node->adress[i];
    }
    if(node->seek[0] != -1)
        for(i = TS; i < node->count + 1; i++)
        {
            brother->seek[i - TS] = node->seek[i];
            NodeS *child = (NodeS *) calloc(1, sizeof(NodeS));
            if(child == nullptr)
                return BTREE_ERR;
            if(readFromDiskS(tree, node->seek[i], child) == BTREE_ERR)
                return BTREE_ERR;
            child->parent = brother->self;
            if(writeSecond(tree, child->self, child) == BTREE_ERR){
                return BTREE_ERR;
            }

        }
    node->count = brother->count = TS - 1;
    NodeS *parent = (NodeS *)calloc(1, sizeof(NodeS));
    if(parent == NULL)
        return BTREE_ERR;
    if(node->parent == -1) {
        parent = createNewSecondNode();
        tree->root = parent;
        parent->self = brother->self + sizeof(NodeS);
        node->parent = parent->self;
    }
    else
    if(readFromDiskS(tree, node->parent, parent) == BTREE_ERR)
        return BTREE_ERR;
    int key_index = Kindex(parent, node->key[0].title);

    for(i = parent->count; i > key_index; i--){
        parent->key[i] = parent->key[i - 1];
        parent->adress[i] = parent->adress[i - 1];
    }
    strcpy(parent->key[i].title,save_key);
    for(i = parent->count + 1; i > key_index + 1; i--){
        parent->seek[i] = parent->seek[i - 1];
    }

    parent->seek[key_index] = node->self;
    parent->seek[key_index + 1] = brother->self;
    parent->count++;
    brother->parent = node->parent;

    writeSecond(tree, parent->self, parent);
    writeSecond(tree, brother->self, brother);
    writeSecond(tree, node->self, node);
    if(readFromDiskS(tree, tree->root->self, tree->root) == BTREE_ERR)
        return BTREE_ERR;
    *node = *parent;
    return BTREE_OK;
}
