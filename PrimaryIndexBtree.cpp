using namespace std;

#include <cstring>
#include "PrimaryIndexBtree.h"

PrimaryIndexBtree *PrimaryIndexBtree::initTree() {
    return nullptr;
}

bool PrimaryIndexBtree::insertBtree(PrimaryIndexBtree *indexBtree, int key) {
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
    FILE *hash = fopen("/home/katiely/Documents/BD/TP2-BD-TREE/hashing.bin","rb");
    if(fp==NULL || hash==NULL){
        return NULL;
    }
    PrimaryIndexBtree *indexBtree = initTree();
   // cout<<sizeof(nodePrimaryIndex) << endl;
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

Article PrimaryIndexBtree::getArticleFromBtree(int id) {
    //nodePrimaryIndex *node = searchBtree(this->oot,id);)
    return Article();
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
    //fp = fopen("hashing.bin","r");

}
