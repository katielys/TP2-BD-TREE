///\file upload.cpp Arquivo responsavel por criar o arquivo hash, o indice primario e indice secundario.
#include "PrimaryIndexBtree.h"
#include "Parser.h"
#include "HashFile.h"

using  namespace std;


int main(int argc, char **argv) {
    Parser p;
    auto records = p.readCSV(argv);

    std::cout << "creating hashing file .." << std::endl;
    Hashing::createHash(records.size(), 2, "./hashing.bin");
    Hashing::createOverflow("./overflow.bin");

    PrimaryIndexBtree indexBtree;

    indexBtree.buildIndex(&indexBtree);
    Hashing::HashInstance hash = Hashing::HashInstance("./hashing.bin");
    Hashing::OverflowArea overflow = Hashing::OverflowArea("./overflow.bin");

    for (auto &record : records) {

         auto adress = Hashing::insertOnHashFile(record, hash, overflow);
         indexBtree.insertBtree(&indexBtree,record.getID(),adress);
    }

    std::cout << "Buckets number: " << hash.buckets << std::endl;
    std::cout << "Overflow blocks: " << overflow.blocksCount << std::endl;
    records.clear();
    hash.close();
    overflow.close();

    return 0;
}
