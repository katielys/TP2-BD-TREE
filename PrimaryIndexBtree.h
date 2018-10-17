#ifndef TP2_BD_TREE_PRIMARYINDEXBTREE_H
#define TP2_BD_TREE_PRIMARYINDEXBTREE_H

using namespace std;

#include "Article.h"
#include <iostream>
#include <cstring>


#define MAX_KEY  511
#define MAX_SEEK MAX_KEY-1

typedef struct nodePrimaryIndex{
    int  key [MAX_KEY ];
    int  seek[MAX_SEEK];
    int  self;
    int  count;
    int  parent;
} nodePrimaryIndex;

class PrimaryIndexBtree {

  public:
    nodePrimaryIndex *root;
    FILE *pfile;
    PrimaryIndexBtree *initTree();
    PrimaryIndexBtree *createTree();
    bool insertBtree(PrimaryIndexBtree *indexBtree, int key);
    int readBtreeFromDisk(PrimaryIndexBtree * indexBtree,int seekPosition , nodePrimaryIndex * node);
    nodePrimaryIndex* createNode();
    int writeDiskBtree(PrimaryIndexBtree *indexBtree,int seekPosition , nodePrimaryIndex *node);
    nodePrimaryIndex *searchBtree(PrimaryIndexBtree *indexBtree , int key);
    int splitBtree(PrimaryIndexBtree *indexBtree , nodePrimaryIndex *node);
    int indexKeyBTree(nodePrimaryIndex *node, int k);
    int endFileIndex(PrimaryIndexBtree *indexBtree);
    void readRoot(PrimaryIndexBtree *indexBtree);
    void buildIndex( PrimaryIndexBtree *primaryIndexBtree);
    Article getArticleFromBtree(int id);
    string nodeToString(nodePrimaryIndex *index){
        string result = "Number of keys occuped: "+to_string(index->count) + "\n" ;
        return result;

    }

};

#endif //TP2_BD_TREE_PRIMARYINDEXBTREE_H