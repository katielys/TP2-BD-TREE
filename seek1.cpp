#include "PrimaryIndexBtree.h"
#include "Article.h"
using namespace std;


//int main(int argc, char ** argv){
//    int idToSeek = 232;
//    idToSeek = stoi(argv[1]);
//
//    PrimaryIndexBtree p;
//    p.pfile = fopen("primaryIndex.bin","r");
//    p.readRoot(&p);
//    nodePrimaryIndex *n = (nodePrimaryIndex *) malloc(sizeof(nodePrimaryIndex));
//    n = p.searchBtree(&p,idToSeek);
//    if (n==NULL){
//        cout <<"--->" << idToSeek << " not found " << endl;
//    }
//    else{
//        cout << "achei"<< endl;
        //cout<<p.parent<<endl;
//        //p.indexKeyBTree()
//    }
//
//    fclose(p.pfile);
//
//
//    return 0;

//}