///\file upload.cpp Arquivo responsavel por criar o arquivo hash, o indice primario e indice secundario.
#include <map>
#include <set>
#include "PrimaryIndexBtree.h"
#include "SecondIndexBtree.h"

using  namespace std;

std::set<unsigned int>
        ban = {
        52271, 53856, 54139, 59107,122434, 122446, 112901, 114691, 114793, 287499, 288560, 296325, 296828, 382214, 392008,
        393294, 394601, 401374, 403827, 442051, 443858, 448383, 452335, 452451, 454907, 456935
};


int main(int argc, char **argv) {
    Parser p;
    std::map<unsigned int, Hashing::Address> mapAdress;
    std::map<unsigned int, std::string> mapTitle;
    auto records = p.readCSV(argv);
    clock_t begin;
    begin = clock();
    Hashing::Address add;

    std::cout << "creating hashing file .." << std::endl;
    Hashing::createHash(records.size(), 2, "hashing.bin");
    Hashing::createOverflow("overflow.bin");

    cout << "Creating primary index"<< endl;
    btree tree = createIndex("primaryIndex.bin");

    BTreeS secondIndex;
    secondIndex = createSecondIndex("secondIndex.bin");
    Hashing::HashInstance hash = Hashing::HashInstance("hashing.bin");
    Hashing::OverflowArea overflow = Hashing::OverflowArea("overflow.bin");


    for (auto &record : records) {
         auto adress = Hashing::insertOnHashFile(record, hash, overflow);
         // inserting in mapAddress, trying to avoid copies using emplace
//         mapAdress.emplace(std::piecewise_construct,
//                           std::forward_as_tuple(record.getID()),
//                           std::forward_as_tuple(adress));
////        std::string title(record.getTitle());
////        mapTitle.emplace(std::piecewise_construct,
////                          std::forward_as_tuple(record.getID()),
////                          std::forward_as_tuple(title));
    }

    std::cout << "Buckets number: " << hash.buckets << std::endl;
    std::cout << "Overflow blocks: " << overflow.blocksCount << std::endl;

    hash.close();
    overflow.close();

    std::cout << "indexing records in second btree..." << std::endl;
    for (auto &record : records) {
        try {
            std::string title(record.getTitle());
            unsigned int id = record.getID();
            if (ban.find(id) == ban.end()) {
                int check = addElementS(secondIndex, title.c_str(), add);

                if (check == 0) {
                    std::cout << "sucessfully indexed record " << id << " " << title
                              << std::endl;
                }
                std::cout << sizeof(NodeS) << std::endl;
            }else{
                record.toString();
            }
        }catch (exception &exception1){
            std::cout << exception1.what() << std::endl;
        }


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
