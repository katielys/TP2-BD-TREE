using namespace std;

#include <cstring>
#include "PrimaryIndexBtree.h"

PrimaryIndexBtree *PrimaryIndexBtree::initTree() {
    return nullptr;
}

bool PrimaryIndexBtree::insertBtree(PrimaryIndexBtree *indexBtree, int key,int adress) {
    nodePrimaryIndex *node = (nodePrimaryIndex * ) calloc (1,sizeof(nodePrimaryIndex));
    if (node == NULL){
        return false;
    }
    *node = *indexBtree->root;
    if(splitBtree(indexBtree,node)==-1){
        return false;
    }
    int index = indexKeyBTree(node,key);
    while(node->seek[0]!=-1) {
        if (readBtreeFromDisk(indexBtree, node->seek[index], node) == -1) {
            return false;
        }
        if (splitBtree(indexBtree, node) == -1) {
            return false;
        }
        index = indexKeyBTree(node, key);
    }
    int idx ;
    for (idx= node->count; idx > index ; idx --){
        node->key[idx] = node->key[idx-1];
    }
    node->key[index] = key;
    node->count++;
    node->adress[index] = adress;
    if(writeDiskBtree(indexBtree,node->self,node)==-1){
        return false;
    }
    if(indexBtree->root->self == node->self){
        *indexBtree->root = *node;
        return true;
    }


    return true;

}

PrimaryIndexBtree *PrimaryIndexBtree::createTree() {

    FILE*fp = fopen("/home/katiely/Documents/BD/TP2-BD-TREE/primaryIndex.bin","wb");
    if(fp==NULL){
        return NULL;
    }
    PrimaryIndexBtree *indexBtree = initTree();
    indexBtree =  (PrimaryIndexBtree * ) calloc (1,sizeof(PrimaryIndexBtree));
    if(indexBtree == NULL){
        return NULL;
    }
    indexBtree->pfile = fp;
    indexBtree->root = createNode();
    indexBtree->root->count = 0;
    indexBtree->root->parent = -1;
    indexBtree->root->self = 0;
    if(writeDiskBtree(indexBtree,indexBtree->root->self, indexBtree->root)==0){
        return indexBtree;
    }
    return NULL;


}

int PrimaryIndexBtree::readBtreeFromDisk(PrimaryIndexBtree *indexBtree, int seekPosition, nodePrimaryIndex *node) {
    if(fseek(indexBtree->pfile,seekPosition,SEEK_SET)==-1){
        return -1;
    }
    char  *a = (char *) calloc(1,sizeof(nodePrimaryIndex));
    if(fread(a,sizeof(nodePrimaryIndex),1,indexBtree->pfile) ==-1){
        return -1;
    }
    memcpy(node,a,sizeof(nodePrimaryIndex));
    return 0;
}

nodePrimaryIndex *PrimaryIndexBtree::createNode() {
    nodePrimaryIndex *node = (nodePrimaryIndex *) malloc(sizeof(nodePrimaryIndex));
    node->self= -1;
    node->parent = -1;
    for (int i = 0; i < MAX_KEY ; ++i) {
        node->key[i] = 0;
        node->adress[i] = 0;
    }
    for (int i =0 ; i < MAX_SEEK ; i++){
        node->seek[i]= -1;
    }
    node->count = 0;
    return node;
}

int PrimaryIndexBtree::writeDiskBtree(PrimaryIndexBtree *indexBtree, int seekPosition, nodePrimaryIndex *node) {
    if(fseek(indexBtree->pfile,seekPosition,SEEK_SET)==-1){
        return -1;
    }
    if(fwrite( node, sizeof(nodePrimaryIndex) , 1 , indexBtree->pfile ) == -1){
        return -1;
    }
    return 0;
}

nodePrimaryIndex *PrimaryIndexBtree::searchBtree(PrimaryIndexBtree *indexBtree, int key) {
    nodePrimaryIndex *node = (nodePrimaryIndex *) calloc(1,sizeof(nodePrimaryIndex));
    if(node == NULL){
        cout<< "CANT ALLOCATE SPACE " << endl;

        return NULL;
    }
    *node = *indexBtree->root;
    int kindex= indexKeyBTree(node,key);
    while(node->seek[0]!=1 && node->key[kindex]!=key){
        if(readBtreeFromDisk(indexBtree,node->seek[kindex],node)==-1){
            return NULL;
        }
        kindex = indexKeyBTree(node,key);
    }
    if(node->key[kindex] == key){
        return node;
    }
    return NULL;
}

int PrimaryIndexBtree::splitBtree(PrimaryIndexBtree *indexBtree, nodePrimaryIndex *node) {
    if(node->count < MAX_SEEK)
        return 0;
    nodePrimaryIndex *brother = createNode();
    brother->self = endFileIndex(indexBtree);
    int save_key = node->key[MAX_SEEK - 1];
    int i;
    for(i = MAX_SEEK; i < node->count; i++)
        brother->key[i] = node->key[i];
    if(node->seek[0] != -1)
        for(i = MAX_SEEK; i < node->count + 1; i++)
        {
            brother->seek[i] = node->seek[i];
            nodePrimaryIndex *child = ( nodePrimaryIndex * )calloc(1, sizeof(nodePrimaryIndex));
            if(child == NULL)
                return -1;
            if(readBtreeFromDisk(indexBtree, node->seek[i], child) == -1)
                return -1;
            child->parent = brother->self;
            if(writeDiskBtree(indexBtree, child->self, child) == -1)
                return -1;
        }
    node->count = brother->count = MAX_KEY;
    nodePrimaryIndex *parent =  (nodePrimaryIndex * )calloc(1, sizeof(nodePrimaryIndex));
    if(parent == NULL)
        return -1;
    if(node->parent == -1)
    {
        parent = createNode();
        indexBtree->root = parent;
        parent->self = brother->self + sizeof(nodePrimaryIndex);
        node->parent = parent->self;
    }
    else
    if(readBtreeFromDisk(indexBtree, node->parent, parent) == -1)
        return -1;
    int key_index = indexKeyBTree(parent, node->key[0]);
    for(i = parent->count; i > key_index; i--)
        parent->key[i] = parent->key[i - 1];
    parent->key[i] = save_key;
    for(i = parent->count + 1; i > key_index + 1; i--)
        parent->seek[i] = parent->seek[i - 1];
    parent->seek[key_index] = node->self;
    parent->seek[key_index + 1] = brother->self;
    parent->count++;
    brother->parent = node->parent;

    writeDiskBtree(indexBtree, parent->self, parent);
    writeDiskBtree(indexBtree, brother->self, brother);
    writeDiskBtree(indexBtree, node->self, node);
    if(readBtreeFromDisk(indexBtree, indexBtree->root->self, indexBtree->root) == -1)
        return -1;
    *node = *parent;
    return 0;

}

int PrimaryIndexBtree::endFileIndex(PrimaryIndexBtree *indexBtree) {
    if(fseek(indexBtree->pfile,0,SEEK_END)==-1){
        return -1;
    }
    return ftell(indexBtree->pfile);
}

int PrimaryIndexBtree::indexKeyBTree(nodePrimaryIndex *node, int k) {
///Binary search
    int l = 0;
    int h = node->count ;
    int m = (l+h) /2 ;
    while(l <= h){
        if(node->key[m]==k)
            return m;
        else if(node->key[m]>k)
            h  =  m -1;
        else
            l = m +1;

        m = (l+h) /2 ;

    }

    return h+1;
}




void PrimaryIndexBtree::readRoot(PrimaryIndexBtree *i ) {
    nodePrimaryIndex *node = NULL;
    node = i->createNode();
    cout<< readBtreeFromDisk(i,0,node);

}

void PrimaryIndexBtree::buildIndex( PrimaryIndexBtree *pib) {
    //PrimaryIndexBtree *pib;
    pib = initTree();
    pib = createTree();

}


//using namespace std;
//
//#include "PrimaryIndexBtree.h"
//
//void PrimaryIndexBtree::loadData(PrimaryIndexBtree *indexBtree) {
//    indexBtree->fp = fopen("primaryIndex.bin","rb");
//    fseek(fp,0,SEEK_SET);
//    NodeType_H node;
//    fread(&node,sizeof(NodeType_H),1,fp);
//    indexBtree->root = &node;
//
//}
//
//
//
//void PrimaryIndexBtree::buildIndex(PrimaryIndexBtree *indexBtree) {
//    indexBtree = (PrimaryIndexBtree*) malloc(sizeof(PrimaryIndexBtree));
//    FILE *pfile = fopen("primaryIndex.bin","wb");
//    indexBtree->fp = pfile;
//    fseek(pfile,0,SEEK_SET);
//    NodeType_H *node;
//    //node = (NodeType_H *) malloc(sizeof(NodeType_H));
//    node =  createNewNodo();
//    indexBtree->root =  node;
//    fwrite(indexBtree->root,sizeof(NodeType_H),1,indexBtree->fp);
//    fclose(indexBtree->fp);
//
//}
//
//NodeType_H *PrimaryIndexBtree::createNewNodo() {
//    NodeType_H *node;
//    node = (NodeType_H *) malloc(sizeof(NodeType_H));
//    node->count = 0;
//    for (int i = 0; i < MAXKEY ; ++i) {
//        node->KEYS[i].adress = EMPTY;
//        node->KEYS[i].id = EMPTY;
//        node->CHILD[i] = EMPTY;
//    }
//    node->CHILD[ORDER] =  EMPTY;
//    return  node;
//}
//int PrimaryIndexBtree::needSplit(PrimaryIndexBtree *indexBtree, NodeType_H *node){
//    if(node->count<MAXKEY){
//        return  0;
//    }
//    NodeType_H *brother ;
//    brother = createNewNodo();
//    brother->count = 0;
//    unsigned int auxK = node->KEYS[MAXKEY-1].id;
//    int i;
//    for(i = T; i < node->count; i++){
//        brother->KEYS[i - T].id = node->KEYS[i].id;
//        brother->KEYS[i - T].adress = node->KEYS[i].adress;
//    }
//
//
//    if(node->CHILD[0] != -1)
//        for(i = T; i < node->count + 1; i++) {
//            brother->CHILD[i - T] = node->CHILD[i];
//            NodeType_H *child = calloc(1, sizeof(NodeType_H));
//            if(child == NULL)
//                return -1;
//            if(btree_read_disk(indexBtree, node->CHILD[i], child) == -1)
//                return -1;
//            //child->parent = brother->self;
//            if(writeDisk(index, child->self, child) == -1)
//                return -1;
//        }
//    node->count = brother->count = MAXKEY - 1;
//    NodeType_H *parent = calloc(1, sizeof(NodeType_H));
//    if(parent == NULL)
//        return -1;
//    if(node->parent == -1) {
//        parent = createNewNodo();
//        indexBtree->root = parent;
//        parent->self = brother->self + sizeof(NodeType_H);
//
//    }
//    else
//    if(readDisk(indexBtree, node->parent, parent) == -1)
//        return -1;
//    int key_index = binarySearch(parent, node->KEYS[0].id);
//    for(i = parent->key_num; i > key_index; i--)
//        parent->key[i] = parent->key[i - 1];
//    parent->key[i] = -1;
//    for(i = parent->count + 1; i > key_index + 1; i--)
//        parent->CHILD[i] = parent->CHILD[i - 1];
//    parent->CHILD[key_index] = node->self;
//    parent->CHILD[key_index + 1] = brother->self;
//    parent->count++;
//    writeDisk(indexBtree, parent->self, parent);
//    writeDisk(indexBtree, brother->self, brother);
//    writeDisk(indexBtree, node->self, node);
//    if(writeDisk(indexBtree, indexBtree->root->self, indexBtree->root) == -1)
//        return -1;
//    *node = *parent;
//   // return BTREE_OK;
//    return 0;
//
//}
//bool PrimaryIndexBtree::addElement(PrimaryIndexBtree *indexBtree, unsigned int id, unsigned int adress) {
//    NodeType_H *nodo = (NodeType_H *) calloc(1,sizeof(NodeType_H *));
//    if(nodo==NULL){
//        cout<< "Erro na alocacao \n";
//        return false;
//    }
//    int po = 0;
//    *nodo = *indexBtree->root;
//
//    if(needSplit(indexBtree,nodo)==-1){
//
//        return false;
//    }
//    int kindex = binarySearch(nodo,id);
//    while(nodo->CHILD[0]!=EMPTY){
//        if(readDisk(indexBtree,nodo->CHILD[kindex],nodo)==-1){
//            return -1;
//        }
//        if(needSplit(indexBtree,nodo)==-1){
//            return -1;
//        }
//        kindex=binarySearch(nodo,id);
//        po++;
//
//    }
//    for (int i = nodo->count; i > kindex ; --i) {
//        nodo->KEYS[i].id = nodo->KEYS[i-1].id;
//        nodo->KEYS[i].adress = nodo->KEYS[i-1].adress;
//    }
//    nodo->KEYS[kindex].id = id;
//    nodo->KEYS[kindex].adress = adress;
//    nodo->count++;
//    if(writeDisk(indexBtree,po,nodo)==-1){
//        return false;
//    }
//
//    return true;
//}
//
//NodeType_H *PrimaryIndexBtree::searchNode(PrimaryIndexBtree *indexBtree, int k) {
//    NodeType_H *node = (NodeType_H *) calloc(1,sizeof(indexBtree));
//    *node = *indexBtree->root;
//    int kindex= binarySearch(node,k);
//    while(node->CHILD[0]!=EMPTY && node->KEYS[kindex].id!=k){
//        if(readDisk(indexBtree,node->CHILD[kindex],node)==-1){
//            return nullptr;
//        }
//        kindex = binarySearch(node,kindex);
//
//    }
//    if(node->KEYS[kindex].id == k){
//        return  node;
//    }
//    return nullptr;
//}
//
//int PrimaryIndexBtree::binarySearch(NodeType_H *nodo, int key) {
//    int l = 0 ;
//    int h = nodo->count -1;
//    int m = (l+h)/2;
//    while(l<=h){
//        if(nodo->KEYS[m].id>key)
//            h = m -1;
//        else if(nodo->KEYS[m].id <key)
//            l = m+1;
//        else
//            return m;
//        m = (l+h)/2;
//    }
//    return  h+1;
//}
//
//int PrimaryIndexBtree::readDisk(PrimaryIndexBtree *indexBtree, int position, NodeType_H *nodo) {
//    if(fseek(indexBtree->fp,position,SEEK_SET)==-1){
//        cout<< "ERROR TO SEEK FILE" << endl;
//        return  -1;
//    }
//    char *buf = (char *) calloc(1,sizeof(NodeType_H));
//    if(fread(buf,sizeof(NodeType_H),1,indexBtree->fp)==-1){
//
//        cout<< "ERROR TO READ FILE" << endl;
//        return -1;
//    }
//    memcpy(nodo,buf, sizeof(NodeType_H));
//    return  0;
//}
//
//int PrimaryIndexBtree::writeDisk(PrimaryIndexBtree *indexBtree, int position, NodeType_H *nodo) {
//    if(fseek(indexBtree->fp,position,SEEK_SET)==-1){
//        cout<< "ERROR TO SEEK FILE" << endl;
//        return  -1;
//    }
//    if(fwrite(nodo,sizeof(NodeType_H),1,indexBtree->fp)==-1){
//        cout<< "ERROR TO WRITE ON FILE" << endl;
//        return -1;
//    }
//    return 0;
//}
