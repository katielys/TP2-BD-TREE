///\file upload.cpp Arquivo responsavel por criar o arquivo hash, o indice primario e indice secundario.
#include <iostream>
#include "HashFile.h"
#include "Block.h"
using  namespace std;


int main (int argc, char** argv){
    Parser p;
    auto records = p.readCSV(argv);
    Hashing::createHash(records.size(), 2, "hashing.bin");
    Hashing::createOverflow("overflow.bin");
    Hashing::HashInstance hash = Hashing::HashInstance("hashing.bin");
    Hashing::OverflowArea overflow = Hashing::OverflowArea("overflow.bin");
    for(auto &record : records){
        Hashing::insertOnHashFile(record, hash, overflow);
    }
    PrimaryIndexBtree *indexBtree;
    indexBtree->buildIndex(indexBtree);
    for (int i = 0; i < records.size(); ++i) {
        indexBtree->insertBtree(indexBtree,records[i].getID());
    }
    records.clear();

    Parser p;
    auto records = p.readCSV(argv);

    std::cout << "creating hashing file .." << std::endl;
    Hashing::createHash(1021443, 2, "hashing.bin");
    Hashing::createOverflow("overflow.bin");

    std::cout << "Buckets number: " << hash.buckets << std::endl;
    std::cout << "Overflow blocks: " << overflow.blocksCount << std::endl;
    hash.close();
    overflow.close();