///\file upload.cpp Arquivo responsavel por criar o arquivo hash, o indice primario e indice secundario.
#include <iostream>
#include "HashFile.h"

using  namespace std;


int main (int argc, char** argv){

    Parser p;
    auto records = p.readCSV(argv);

    std::cout << "creating hashing file .." << std::endl;
    Hashing::createHash(1021443, 2, "hashing.bin");
    Hashing::createOverflow("overflow.bin");

    Hashing::HashInstance hash = Hashing::HashInstance("hashing.bin"); // opening hash file
    Hashing::OverflowArea overflow = Hashing::OverflowArea("overflow.bin"); // opening overflow area

    std::cout << "Buckets number: " << hash.buckets << std::endl;
    std::cout << "Overflow blocks: " << overflow.blocksCount << std::endl;

    // closing files
    hash.close();
    overflow.close();


    return 0;
}


