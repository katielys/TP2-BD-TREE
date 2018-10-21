
#include "SecondIndexBtree.h"
int stringToIndexNumberf(string toIndex) {
    auto sizes = toIndex.size();

    auto indexed = 00;
    if(sizes<1){
        indexed = 01;
        return indexed;
    }
    for (int i = 1; i < sizes; ++i) {
        indexed += int(toIndex[i-1])*i*i+(i%104393);
    }
	//indexed=indexed+sizes;
    return indexed;
}
int readSecond(SecondIndex &tree, int seek, NodeS &node){
    if(fseek(tree.fp, seek, SEEK_SET) == -1){
        return BTREE_ERR;
    }

    if(fread(&node, sizeof(NodeS), 1, tree.fp) == -1)
        return BTREE_ERR;

    return BTREE_OK;
}

int writeSecond(SecondIndex &tree, int seek, NodeS &node){
    if(fseek(tree.fp, seek, SEEK_SET) == -1) {
        return BTREE_ERR;
    }

    if(fwrite(&node, sizeof(NodeS), 1, tree.fp) == -1) {
        return BTREE_ERR;
    }

    return BTREE_OK;
}


int Kindex(NodeS &node, int key){
    int low = 0;
    int high = node.count -1;
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
int endFilseSecond(SecondIndex &tree){
    if(fseek(tree.fp, 0, SEEK_END) == -1)
        return BTREE_ERR;
    return ftell(tree.fp);
}



NodeS createNewNodeSecond(){
    NodeS node;
    node.self = -1;
    node.parent = -1;
    int i;
    for(i = 0; i < 2 * TS - 1; i++){
        node.key[i] = 0;
        node.adress[i] = Hashing::Address();
    }
    for(i = 0; i < 2 * TS; i++)
        node.seek[i] = -1;
    node.count = 0;
    return node;

  
}

SecondIndex createSecondIndex(const char *file){
    FILE *fp = fopen(file, "w+");
    SecondIndex tree;

    if(fp == nullptr){
        throw "Cannot open second index file";
    }

    tree.fp = fp;
    tree.root = createNewNodeSecond();
    tree.root.self = 0;
    if(writeSecond(tree, tree.root.self, tree.root) == BTREE_ERR) { // mudar pra retornar bool se for o caso
        throw "Cannot write on second index file";
    }


    return tree;
}

pair<bool, Hashing::Address> searchAtSecond(SecondIndex &tree, NodeS &node, int key){
    short int countBlock = 1;
    Hashing::Address ad;
    //cout<< (*tree->root);
    node = tree.root;
    int key_index = Kindex(node, key);
    while(node.seek[0] != -1 && node.key[key_index] != key)
    {
        if(readSecond(tree, node.seek[key_index], node) == BTREE_ERR) {
            return std::make_pair(false, ad);
        }
        key_index = Kindex(node, key);
        countBlock++;
    }

    if(node.key[key_index] == key) {
        cout<< "Numero de blocos de indice lidos:  "<< countBlock << endl;
        ad = node.adress[key_index];
        return std::make_pair(true, ad);
    }

    return std::make_pair(false, ad);
}

void  loadSecondRoot(SecondIndex &t,const char *file){

    FILE *fp = fopen(file,"rb");
    fseek(fp, 0, SEEK_END);
    auto curPos = ftell(fp);
    fseek(fp, (curPos- sizeof(unsigned int)), SEEK_SET);
    NodeS node;
    unsigned int rootOffset = 0;
    fread(&rootOffset, sizeof(unsigned int), 1,fp);

    fseek(fp, rootOffset, SEEK_SET);
    fread(&node, sizeof(NodeS), 1,fp);

    t.fp = fp;
    t.root = node;
}


int addElementSecondIndex(SecondIndex &tree, int key, Hashing::Address adress){

    NodeS node;

    node = tree.root;

    if(splitSecond(tree, node) == BTREE_ERR)
        return BTREE_ERR;

    int key_index = Kindex(node, key);

    while(node.seek[0] != -1) {

        if(readSecond(tree, node.seek[key_index], node) == BTREE_ERR) {
            return BTREE_ERR;
        }

        if(splitSecond(tree, node) == BTREE_ERR) {
            return BTREE_ERR;
        }

        key_index = Kindex(node, key);
    }

    int i;
    for(i = node.count; i > key_index; i--){
        node.key[i] = node.key[i - 1];
        node.adress[i] = node.adress[i - 1];
    }

    node.key[key_index] = key;
    node.adress[key_index] = adress;
    node.count++;

    if(writeSecond(tree, node.self, node) == BTREE_ERR) {
        return BTREE_ERR;
    }

    if(tree.root.self == node.self) {
        tree.root = node;
    }

    return BTREE_OK;
}
int splitSecond(SecondIndex &tree, NodeS &node){
    if(node.count < 2 * TS - 1) {
        return BTREE_OK;
    }

    NodeS brother = createNewNodeSecond();
    brother.self = endFilseSecond(tree);

    int save_key = node.key[TS - 1];
    int i;

    for(i = TS; i < node.count; i++){
        brother.key[i - TS] = node.key[i];
        brother.adress[i - TS] = node.adress[i];
    }

    if(node.seek[0] != -1)
        for(i = TS; i < node.count + 1; i++)
        {
            brother.seek[i - TS] = node.seek[i];
            NodeS child;
            if(readSecond(tree, node.seek[i], child) == BTREE_ERR)
                return BTREE_ERR;

            child.parent = brother.self;

            if(writeSecond(tree, child.self, child) == BTREE_ERR)
                return BTREE_ERR;
        }

    node.count = brother.count = TS - 1;

    NodeS parent;

    if(node.parent == -1) {
        // the actual node is root, thus we have to do all changes in the root by this way
        parent = createNewNodeSecond();
        tree.root = parent;
        tree.root.self = brother.self + sizeof(NodeS);
        node.parent = tree.root.self;

        int key_index = Kindex(tree.root, node.key[0]);

        for(i = tree.root.count; i > key_index; i--){
            tree.root.key[i] = tree.root.key[i - 1];
            tree.root.adress[i] = tree.root.adress[i - 1];
        }

        tree.root.key[i] = save_key;

        for(i = tree.root.count + 1; i > key_index + 1; i--) {
            tree.root.seek[i] = tree.root.seek[i - 1];
        }

        tree.root.seek[key_index] = node.self;
        tree.root.seek[key_index + 1] = brother.self;
        tree.root.count++;
        brother.parent = node.parent;

        writeSecond(tree, brother.self, brother);
        writeSecond(tree, tree.root.self, tree.root);
        writeSecond(tree, node.self, node);
        if(readSecond(tree, tree.root.self, tree.root) == BTREE_ERR) {
            return BTREE_ERR;
        }
        node = tree.root;
        return BTREE_OK;

    } else if(readSecond(tree, node.parent, parent) == BTREE_ERR) { // the node has a parent, read it from disk
        return BTREE_ERR;
    }
    // the actual isn't a root, so we proceed normaly, making changes in the parent read from the disk
    int key_index = Kindex(parent, node.key[0]);

    for(i = parent.count; i > key_index; i--){
        parent.key[i] = parent.key[i - 1];
        parent.adress[i] = parent.adress[i - 1];
    }

    parent.key[i] = save_key;

    for(i = parent.count + 1; i > key_index + 1; i--) {
        parent.seek[i] = parent.seek[i - 1];
    }

    parent.seek[key_index] = node.self;
    parent.seek[key_index + 1] = brother.self;
    parent.count++;
    brother.parent = node.parent;

    writeSecond(tree, parent.self, parent);
    writeSecond(tree, brother.self, brother);
    writeSecond(tree, node.self, node);
    if(readSecond(tree, tree.root.self, tree.root) == BTREE_ERR) {
        return BTREE_ERR;
    }
    node = parent;
    return BTREE_OK;
}

void saveSecondRootOffset(SecondIndex &t){
    auto end = endFilseSecond(t);
    fseek(t.fp, end, SEEK_SET);
    auto check = fwrite(&t.root.self, sizeof(unsigned int), 1, t.fp);
}

NodeS::NodeS() {

}

SecondIndex::SecondIndex() {

}
