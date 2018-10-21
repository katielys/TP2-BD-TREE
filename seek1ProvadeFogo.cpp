#include "PrimaryIndexBtree.h"
using namespace std;

//TODO : COLOCAR EM CLASSE
//TODO: usar a funcao do tiago para pegar os dados do artigos
int main(int argc, char ** argv){
    auto idToSeek = 0000000000000;
    idToSeek = stoi(argv[1]);
    btree indexPrimary;
    btree_node node;

    Parser p;
    auto records = p.readCSV(argv);

    // opening data file
    Hashing::HashInstance hash = Hashing::HashInstance("hashing.bin");
    Hashing::OverflowArea overflow = Hashing::OverflowArea("overflow.bin");
    // opening index file
    loadRoot(indexPrimary, "primaryIndex.bin");

//    cout<<"keys quantity on node: " << indexPrimary.root.key_num<<endl;
//    cout<<"key at position 39 in node: " << indexPrimary.root.key[39]<<endl;
    for (auto &record : records) {
        auto found = btree_search(indexPrimary, node, record.getID());

        if (!found.first) { // not found
            cout << "->ID: " << record.getID() << "   not found";
            return 0;
        } else {
            cout << "->ID :" << record.getID() << " found xD" << endl;
            Article art = Hashing::getRecordByAddress(found.second, hash, overflow);
            cout << art.toString() << std::endl;
        }
    }

    return 0;

}