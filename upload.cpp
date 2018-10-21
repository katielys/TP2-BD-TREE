///\file upload.cpp Arquivo responsavel por criar o arquivo hash, o indice primario e indice secundario.
#include <map>
#include "PrimaryIndexBtree.h"
#include "SecondIndexBtree.h"

using  namespace std;


int main(int argc, char **argv) {
    Parser p;
    std::map< char *, Hashing::Address> mapAdress;
    auto records = p.readCSV(argv);
    clock_t begin;
    begin = clock();

    std::cout << "creating hashing file .." << std::endl;
    Hashing::createHash(records.size(), 2, "hashing.bin");
    Hashing::createOverflow("overflow.bin");

   // cout << "Creating primary index"<< endl;
    //btree tree = createIndex("primaryIndex.bin");
    BTreeS secondIndex;
    secondIndex = createSecondIndex("secondIndex.bin");
    Hashing::HashInstance hash = Hashing::HashInstance("hashing.bin");
    Hashing::OverflowArea overflow = Hashing::OverflowArea("overflow.bin");
    

    for (auto &record : records) {
         auto adress = Hashing::insertOnHashFile(record, hash, overflow);
         // inserting in mapAddress, trying to avoid copies using emplace
         mapAdress.emplace(std::piecewise_construct,
                           std::forward_as_tuple(record.getTitle()),
                           std::forward_as_tuple(adress));
//         addElementSecondIndex(secondIndex,record.getTitle(),adress);
    }
    records.clear();

    std::cout << "Buckets number: " << hash.buckets << std::endl;
    std::cout << "Overflow blocks: " << overflow.blocksCount << std::endl;
        
    hash.close();
    overflow.close();
    
    std::cout << "indexing records in second btree..." << std::endl;
    for (auto pair : mapAdress) {
        int check = addElementS(secondIndex, pair.first, pair.second);
        if (check < 0) return 0;
        std::cout << "sucessfully indexed record " << pair.first << std::endl;
    }
    // saving root address at beginning of the index
    saveRootOffsetSecond(secondIndex);

    fclose(secondIndex.fp);
    
    clock_t end = clock();

    auto elapsedTime = double(end-begin)/CLOCKS_PER_SEC;

    
    std::cout << "Total elapsed time: " << elapsedTime << std::endl;
    //ls
    
    return 0;
}
