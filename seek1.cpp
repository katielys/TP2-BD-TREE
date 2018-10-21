#include "PrimaryIndexBtree.h"
#include "HashFile.h"
using namespace std;

//TODO : COLOCAR EM CLASSE
//TODO: usar a funcao do tiago para pegar os dados do artigos
int main(int argc, char ** argv){
    auto idToSeek = 0000000000000;
    idToSeek = stoi(argv[1]);
    btree indexPrimary;
    btree_node r;
    FILE *fp = fopen("primaryIndex.bin","rb");
    cout<<fseek(fp,0,SEEK_SET)<<endl;
    cout<< sizeof(btree_node)<<endl;
    cout<<fread(&indexPrimary.root, 1, sizeof(btree_node),fp)<<endl;
    cout<<indexPrimary.root.key_num<<endl;
//    for (int i = 0; i <indexPrimary.root.key_num ; ++i) {
//      cout<<indexPrimary.root.key[i]<<"  ";
//    }
    int h = btree_key_index(indexPrimary.root, idToSeek);

    r= btree_search(indexPrimary,idToSeek);
    cout<<r.key_num<<endl;
    Hashing::HashInstance hash = Hashing::HashInstance("hashing.bin");
    Hashing::OverflowArea overflow = Hashing::OverflowArea("overflow.bin");
    Article aux = Hashing::getRecordByAddress(r.adress[h], hash, overflow);
    cout<< aux.toString()<<endl;

    return 0;

}