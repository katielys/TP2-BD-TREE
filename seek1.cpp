#include "PrimaryIndexBtree.h"
using namespace std;

//TODO : COLOCAR EM CLASSE
//TODO: usar a funcao do tiago para pegar os dados do artigos
int main(int argc, char ** argv){
    auto idToSeek = 0000000000000;
    idToSeek = stoi(argv[1]);
    btree indexPrimary;
    btree_node node;

    //cout<<sizeof(btree_node)<<endl;
    // opening data file
    Hashing::HashInstance hash = Hashing::HashInstance("hashing.bin");
    Hashing::OverflowArea overflow = Hashing::OverflowArea("overflow.bin");
    // opening index file
    loadRoot(indexPrimary, "primaryIndex.bin");

    auto found = btree_search(indexPrimary, node, idToSeek);

    if(!found.first){ // not found
        cout<<"->ID: "<<idToSeek<< "   not found";
    } else{
        cout<<"->ID :"<< idToSeek << " found xD" << endl;
        Article art = Hashing::getRecordByAddress(found.second, hash, overflow);
        cout << art.toString() << std::endl;
    }
    unsigned int numberTotalBlocsofBtree=  btree_end_seek_of_file(indexPrimary);
    cout<<"TOTAL DE BLOCOS NO ARQUIVO DE INDICE PRIMARIO : "<< (numberTotalBlocsofBtree/sizeof(btree_node)-4);
    return 0;

}