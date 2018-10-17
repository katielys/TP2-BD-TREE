#include <iostream>
#include "HashFile.h"
///\autor: Katiely

using namespace std;

int main(int argc, char** argv){

    Hashing::HashInstance hash = Hashing::HashInstance("hashing.bin");
    Hashing::OverflowArea overflow = Hashing::OverflowArea("overflow.bin");

    unsigned int numberReg =  stoi(argv[1]);

    auto value = Hashing::findRecord(numberReg, hash, overflow);
    if (value.first) {
        std::cout << "Blocks passed " << value.second.second << std::endl;
        std::cout << value.second.first.toString() << std::endl;
    }else{
        std::cout << "ID nao encontrado" << std::endl;
    }

    return 0;
}