#include "PrimaryIndexBtree.h"
#include "Article.h"
using namespace std;

//TODO : COLOCAR EM CLASSE
//TODO: usar a funcao do tiago para pegar os dados do artigos
int main(int argc, char ** argv){
    auto idToSeek = 0000000000000;
    idToSeek = stoi(argv[1]);
    btree indexPrimary;
    btree_node *r = nullptr;
    loadRoot(&indexPrimary, "primaryIndex.bin");
    cout<<indexPrimary.root->key_num<<endl;
    cout<<indexPrimary.root->key[57]<<endl;
    r= btree_search(&indexPrimary,idToSeek);
    if(r == nullptr){
        cout<<"->ID: "<<idToSeek<< "   not found";
    }
    else{
        cout<<"ID  "<< idToSeek << "has found" << endl;
        cout<<r->self<<endl;
        cout<<r->key_num<<endl;
    }

    return 0;

}