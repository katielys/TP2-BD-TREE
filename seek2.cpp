#include "SecondIndexBtree.h"

using namespace std;

int main (int argc, char **argv){
    auto stringToSeek = argv[1];
    cout<< stringToSeek << "Will be find if its exists" << endl;
    BTreeS secondIndex;
    NodeS node;

    // opening data file
    Hashing::HashInstance hash = Hashing::HashInstance("hashing.bin");
    Hashing::OverflowArea overflow = Hashing::OverflowArea("overflow.bin");
    // opening index file
    loadRootS(secondIndex, "secondIndex.bin");

    auto found = btree_searchS(secondIndex, node, stringToSeek);

    if(!found.first){ // not found
        cout<<"->Title: "<<stringToSeek<< "   not found";
    } else{
        cout<<"->title :"<< stringToSeek << " found xD" << endl;
        Article art = Hashing::getRecordByAddress(found.second, hash, overflow);
        cout << art.toString() << std::endl;
    }

    return 0;

}
