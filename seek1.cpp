#include "PrimaryIndexBtree.h"
#include "Article.h"
using namespace std;


int main(int argc, char ** argv){
    int idToSeek = 00000;
    idToSeek = stoi(argv[1]);
    btree *indexPrimary;
    loadRoot(indexPrimary, "primaryIndex.bin");
//    btree_search(indexPrimary,idToSeek); ta dando falha de segmentacao mas okay

    return 0;

}