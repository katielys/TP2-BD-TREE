#include "SecondIndexBtree.h"

using namespace std;

int main (int argc, char **argv){
    auto stringToSeek =(argv[1]);
    //cout<< stringToSeek << "Will be find if its exists" << endl;
    SecondIndex secondIndex;
    NodeS node;

    // opening data file
    Hashing::HashInstance hash = Hashing::HashInstance("hashing.bin");
    Hashing::OverflowArea overflow = Hashing::OverflowArea("overflow.bin");
    // opening index file
    loadSecondRoot(secondIndex, "secondIndex.bin");
    auto aux = stringToIndexNumberf(stringToSeek);
    auto found = searchAtSecond(secondIndex, node, aux);

    if(!found.first){ // not found
        cout<<"->Title: "<<stringToSeek<< "   not found";
    } else{
        cout<<"->title :"<< stringToSeek << " found xD" << endl;
        Article art = Hashing::getRecordByAddress(found.second, hash, overflow);
        cout << art.toString() << std::endl;
    }


    return 0;

}
