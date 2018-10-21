#include "SecondIndexBtree.h"

using namespace std;

int main (int argc, char **argv){
    auto stringToSeek = argv[1];
    cout<< stringToSeek << "Will be find if its exists" << endl;
    BTreeS secondIndex;
    NodeS *r = nullptr;
//    loadRootSecondIndex(&secondIndex,"secondIndex.bin");
//    cout<<secondIndex.root->count<<endl;
//    cout<<secondIndex.root->key[0].title<<endl;
//    r= searchAtBtreeS(&secondIndex,stringToSeek);
//    if(r == nullptr){
//        cout<<"->TITLE: "<<stringToSeek<< "   not found";
//    }
//    else{
//        cout<<"->TITLE :"<< stringToSeek << "has found" << endl;
//        cout<<r->self<<endl;
//        cout<<r->count<<endl;
//    }
//    return 0;
}

