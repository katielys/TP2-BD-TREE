///\file upload.cpp Arquivo responsavel por criar o arquivo hash, o indice primario e indice secundario.
#include "PrimaryIndexBtree.h"
#include "Parser.h"
#include "HashFile.h"
#include "SecondIndexBtree.h"

using  namespace std;


int main(int argc, char **argv) {
    Parser p;
    auto records = p.readCSV(argv);
    clock_t begin = clock();

    std::cout << "creating hashing file .." << std::endl;
    Hashing::createHash(records.size(), 2, "hashing.bin");
    Hashing::createOverflow("overflow.bin");

    cout << "Creating primary index"<< endl;
    btree *tree = createIndex("primaryIndex.bin");
//    BTreeS *secondIndex;
//    secondIndex = createIndexSecondary("secondIndex.bin");
    Hashing::HashInstance hash = Hashing::HashInstance("hashing.bin");
    Hashing::OverflowArea overflow = Hashing::OverflowArea("overflow.bin");

    for (auto &record : records) {
         auto adress = Hashing::insertOnHashFile(record, hash, overflow);
         std::cout << "trying to index in primary btree..." << std::endl;
         int check = addElement(tree, record.getID(),adress);
         if(check < 0) return 0;
//         addElementSecondIndex(secondIndex,record.getTitle(),adress);
    }

    clock_t end = clock();

    auto elapsedTime = double(end-begin)/CLOCKS_PER_SEC;

    std::cout << "Buckets number: " << hash.buckets << std::endl;
    std::cout << "Overflow blocks: " << overflow.blocksCount << std::endl;
    std::cout << "Elapsed Time: " << elapsedTime << std::endl;
    records.clear();
    //ls
    fclose(tree->fp);
    hash.close();
    overflow.close();
    return 0;
}
