
#include "SecondIndexBtree.h"

int readSecond(BTreeS &tree, int seek, NodeS &node){
    if(fseek(tree.fp, seek, SEEK_SET) == -1){
        return BTREE_ERR;
    }

    if(fread(&node, sizeof(NodeS), 1, tree.fp) == -1)
        return BTREE_ERR;

    return BTREE_OK;
}

int writeSecond(BTreeS &tree, int seek, NodeS &node){
    if(fseek(tree.fp, seek, SEEK_SET) == -1) {
        return BTREE_ERR;
    }

    if(fwrite(&node, sizeof(NodeS), 1, tree.fp) == -1) {
        return BTREE_ERR;
    }

    return BTREE_OK;
}

int Kindex(NodeS &node, char key[MAXLEN]){
    int low = 0;
    int high = node.count -1;
    int middle = (low + high) / 2;
    while(low <= high){
        if(strcmp(node.key[middle].title ,key)== 0){
            return middle;
        }
        else if(strcmp(node.key[middle].title ,key)>0) {
            high = middle - 1;
        }else
            low = middle + 1;
        middle = (low + high) / 2;
    }
    return high + 1;
}

unsigned int endFilseSecond(BTreeS &tree){
    if(fseek(tree.fp, 0, SEEK_END) == -1)
        return BTREE_ERR;
    return ftell(tree.fp);
}



NodeS createNewNodeSecond(){
    NodeS node;
    node.self = -1;
    node.parent = -1;
    char aux[MAXLEN];
    strcpy(aux,"");
    int i;
    for(i = 0; i < 2 * TS - 1; i++){
        strcpy(node.key[i].title,aux);
        //node.key[i] =
        node.adress[i] = Hashing::Address();
    }
    for(i = 0; i < 2 * TS; i++)
        node.seek[i] = -1;
    node.count = 0;
    return node;
}

BTreeS createSecondIndex(const char *file){
    FILE *fp = fopen(file, "w+");
    BTreeS tree;

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

pair<bool, Hashing::Address> btree_searchS(BTreeS &tree, NodeS &node, char key[MAXLEN]){
    short int countBlock = 1;
    Hashing::Address ad;
    node = tree.root;
    int key_index = Kindex(node, key);
    while(node.seek[0] != -1 && strcmp(node.key[key_index].title,key) != 0)
    {
        if(readSecond(tree, node.seek[key_index], node) == BTREE_ERR) {
            return std::make_pair(false, ad);
        }
        key_index = Kindex(node, key);
        countBlock++;
    }

    if(strcmp(node.key[key_index].title,key) == 0) {
        cout<< "Numero de blocos de indice lidos:  "<< countBlock << endl;
        ad = node.adress[key_index];
        return std::make_pair(true, ad);
    }

    return std::make_pair(false, ad);
}

void loadRootS(BTreeS &indexPrimary, const char *file){

    //btree indexPrimary;
    FILE *fp = fopen(file,"rb");
    fseek(fp, 0, SEEK_END);
    auto curPos = ftell(fp);
    fseek(fp, (curPos- sizeof(unsigned int)), SEEK_SET);
    NodeS node;
    unsigned int rootOffset = 0;
    fread(&rootOffset, sizeof(unsigned int), 1,fp);

    fseek(fp, rootOffset, SEEK_SET);
    fread(&node, sizeof(BTreeS), 1,fp);

    indexPrimary.fp = fp;
    indexPrimary.root = node;
}


int addElementS(BTreeS &tree, char key[MAXLEN], Hashing::Address adress){

    NodeS node;

    node = tree.root;
    if(key==NULL){
        cout<<"aaaaaaaaaaaaaaaaaaaaaaaaaaa"<<endl;
        strcpy(key,"empty");
        char auxxx[MAXLEN];
        strcpy(key,auxxx);
        auxxx[0]= 'e';
        auxxx[1] = 'm';
        auxxx[2] = 't';
        auxxx[3] = 'y';
        strcpy(key,auxxx);
        //strcpy(key,"empty");
    }
    cout<<"inserting key::" << key <<endl;
    if(btree_splitS(tree, node) == BTREE_ERR)
        return BTREE_ERR;

    int key_index = Kindex(node, key);

    while(node.seek[0] != -1) {

        if(readSecond(tree, node.seek[key_index], node) == BTREE_ERR) {
            return BTREE_ERR;
        }

        if(btree_splitS(tree, node) == BTREE_ERR) {
            return BTREE_ERR;
        }

        key_index = Kindex(node, key);
    }

    int i;
    for(i = node.count; i > key_index; i--){
        //strcpy(node.key[i].title,node.key[i - 1].title);
        node.key[i] = node.key[i - 1];
        node.adress[i] = node.adress[i - 1];
    }
    strcpy(node.key[key_index].title,key);
    //node.key[key_index] = key;
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

int btree_splitS(BTreeS &tree, NodeS &node){
    if(node.count < 2 * TS - 1) {
        return BTREE_OK;
    }

    NodeS brother = createNewNodeSecond();
    brother.self = endFilseSecond(tree);

    auto save_key = node.key[TS - 1];
    //strcpy(save_key.title,node.key[TS-1].title);
    int i;

    for(i = TS; i < node.count; i++){
       // strcpy(brother.key[i - TS].title,node.key[i].title);
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

        int key_index = Kindex(tree.root, node.key[0].title);
        i =0;
        for(i = tree.root.count; i > key_index; i--){
            //strcpy(tree.root.key[i].title,tree.root.key[i - 1].title);
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
    int key_index = Kindex(parent, node.key[0].title);

    for(i = parent.count; i > key_index; i--){
        strcpy(parent.key[i].title,parent.key[i - 1].title);
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

void saveRootOffsetSecond(BTreeS &t) {
    auto end = endFilseSecond(t);
    fseek(t.fp, end, SEEK_SET);
    auto check = fwrite(&t.root.self, sizeof(unsigned int), 1, t.fp);
}

NodeS::NodeS() {

}

BTreeS::BTreeS() {

}
