#include <iostream>
#include "HashFile.h"
///\file findrec arquivo responsavel por buscar no hash dado o id
///\author Tiago Santos

using namespace std;

int main(int argc, char** argv){

    Hashing::HashInstance hash = Hashing::HashInstance("hashing.bin");
    Hashing::OverflowArea overflow = Hashing::OverflowArea("overflow.bin");

    unsigned int numberReg =  stoi(argv[1]);

    auto value = Hashing::findRecord(numberReg, hash, overflow);
    if (value.first) {
        std::cout << "Blocks read quantity: " << value.second.second << std::endl;
        std::cout << value.second.first.toString() << std::endl;
    }else{
        std::cout << "ID not found in data file :/" << std::endl;
    }

    return 0;
}