#ifndef TP2_BD_TREE_PRIMARYINDEXBTREE_H
#define TP2_BD_TREE_PRIMARYINDEXBTREE_H

using namespace std;

#include "Article.h"
#include <iostream>

#define MAX_KEY  211
#define MAX_SEEK MAX_KEY-1

typedef struct nodePrimaryIndex{
    int  key [MAX_KEY ];
    unsigned int adress[MAX_KEY];
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
    bool insertBtree(PrimaryIndexBtree *indexBtree, int key, int adress);
    int readBtreeFromDisk(PrimaryIndexBtree * indexBtree,int seekPosition , nodePrimaryIndex * node);
    nodePrimaryIndex* createNode();
    int writeDiskBtree(PrimaryIndexBtree *indexBtree,int seekPosition , nodePrimaryIndex *node);
    nodePrimaryIndex *searchBtree(PrimaryIndexBtree *indexBtree , int key);
    int splitBtree(PrimaryIndexBtree *indexBtree , nodePrimaryIndex *node);
    int indexKeyBTree(nodePrimaryIndex *node, int k);
    int endFileIndex(PrimaryIndexBtree *indexBtree);
    void readRoot(PrimaryIndexBtree *indexBtree);
    void buildIndex( PrimaryIndexBtree *primaryIndexBtree);
    string nodeToString(nodePrimaryIndex *index){
        string result = "Number of keys occuped: "+to_string(index->count) + "\n" ;
        return result;

    }

};

#endif //TP2_BD_TREE_PRIMARYINDEXBTREE_H

//#ifndef PRIMARYINDEXBTREE_H
//#define PRIMARYINDEXBTREE_H
//
//#include <iostream>
//#include <cstring>
//using namespace std;
//#define MAXKEY 339
//#define ORDER MAXKEY+1  //!<ORDER BTREE
//#define EMPTY 99999999
//
//
//typedef struct ItemType_H{
//    unsigned int id;
//    unsigned int adress;
//}ItemType_H;
//
//typedef struct NodeType_H{
//    unsigned short int count ; //!< Number Of Key at Node;
//    ItemType_H KEYS[MAXKEY]; //!< KEYS:
//    unsigned int CHILD[ORDER];  //!< childs
//    int self;
//}NodeType_H;
//
//class PrimaryIndexBtree {
//
//    public:
//
//    FILE *fp;
//    NodeType_H *root;
//    NodeType_H * createNewNodo( );
//    void loadData(PrimaryIndexBtree *indexBtree);
//    void buildIndex(PrimaryIndexBtree *indexBtree);
//    int needSplit(PrimaryIndexBtree *indexBtree, NodeType_H *node);
//    bool addElement(PrimaryIndexBtree *indexBtree, unsigned int id, unsigned int adress);
//    int readDisk(PrimaryIndexBtree *indexBtree,int position, NodeType_H *nodo);
//    int writeDisk(PrimaryIndexBtree *indexBtree,int position, NodeType_H *nodo);
//    int binarySearch(NodeType_H *nodo,int key);
//    NodeType_H * searchNode(PrimaryIndexBtree *indexBtree,int k);
//
//
//};
//
//
//
//
//#endif //PRIMARYINDEXBTREE_H