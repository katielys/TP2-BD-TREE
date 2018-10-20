//
// Created by taigo on 10/14/18.
//

#include <vector>
#include "HashFile.h"
void teste (){
    Article *teste ;
    char autorlist[MAX_SIZE_AUTOR];
    char snipped[MAX_SIZE_SNIPPET];
    char title[MAX_SIZE_TITLE];
    char date[MAX_SIZE_DATE];
    strcpy(autorlist,"Altigran|Nakamura|Edleno|Joao Marcos");
    strcpy(title,"Titulo artigo");
    strcpy(snipped,"Best article from universe");
    strcpy(date,"10.10.2018 23:23:09");
    teste = new Article(1,6,10,title,autorlist,snipped,date ) ;
    cout << teste->toString() <<endl;
    cout<< sizeof(*teste);

    std::vector<Block> blocks;
    Block *neoBlock;
    Block block;
    std::cout << "\nTrying too lookup" << std::endl;
    block.insertRecord(*teste);

    teste = new Article(2,3,11,title,autorlist,snipped,date );
    block.insertRecord(*teste);
    teste = new Article(3,2,1,title,autorlist,snipped,date );
    if(!block.insertRecord(*teste).first){
        blocks.emplace_back(block);
        neoBlock = new Block();
        neoBlock->insertRecord(*teste);
        blocks.emplace_back(*neoBlock);
    }

    Article copy;
    if(block.lookUpforRecord(1, sizeof(Article), copy)) {
        cout << copy.toString();
    }else{
        std::cout << "lookup failed :c";
    }

    FILE *pFile =fopen("./fileHash.bin", "wb+");
    //cout<< pFile;
    for (int i = 0; i < blocks.size();i++ ){
        //cout<<current_block.article.toString()<<endl;

        fseek(pFile, i*BLOCK_SIZE, SEEK_SET);
        fwrite(&blocks[i], BLOCK_SIZE, 1, pFile);
    }
    //this->hash_size  = totalReg;
    fclose(pFile);

}

void getArticleFromDisk(int id) {
    FILE *fp;
    fp = fopen("./fileHash.bin", "rb");
    if (fp== nullptr){
        cout<<"Cant open the file"<<endl<<endl;
        return;
    }
    unsigned int currentPosition = 0;
    while (!feof(fp)) {
        Block aux;
        Article top;
        fseek(fp, currentPosition * BLOCK_SIZE, SEEK_SET);
        //cout<< (ftell(fp)/BLOCK_SIZE) <<endl;
        size_t a = fread(&aux, BLOCK_SIZE, 1, fp);
        if (a==1 && aux.verificationMask == MASK_VALID) {
            if (aux.lookUpforRecord(id, sizeof(Article), top)) {
                std::cout << "found article on block " << currentPosition << std::endl;
                cout<<"Numero total de blocos "<< (currentPosition+1) <<endl;
                std::cout << top.toString();
                return;
            } else {
                currentPosition += 1;
            }
        } else {
            std::cout << "block at position " << currentPosition << " invalid!!";
            currentPosition += 1;
        }
    }

}


int main(int argc, char *argv[]){
//    teste();
//    getArticleFromDisk(4);

//    Block block;

//    std::cout << BLOCK_SIZE;

    Parser p;
    auto records = p.readCSV(argv);
    Hashing::createHash(1021443, 2, "hashing.bin");
    Hashing::createOverflow("overflow.bin");

    Hashing::HashInstance hash = Hashing::HashInstance("hashing.bin");
    Hashing::OverflowArea overflow = Hashing::OverflowArea("overflow.bin");

    for(auto &record : records){
        auto address = Hashing::insertOnHashFile(record, hash, overflow);
        std::cout << ((address.overflow) ? "Inserted in overflow" : "Inserted in hash main file") << std::endl;
        std:: cout << "Bucket: " << address.offset << std::endl;
        std:: cout << "Block Offset: " << address.blockOffset << std::endl;
        Article aux = Hashing::getRecordByAddress(address, hash, overflow);
        if(aux.getID() != record.getID()){
            std::cout << aux.getID() << " " << record.getID() << std::endl;
            return 0;
        } else{
            std::cout << "Found at this address" << std::endl;
        }
    }

//    for (auto &record : records) {
//        auto value = Hashing::findRecord(record.getID(), hash, overflow);
//        if (value.first) {
//            std::cout << "Blocks passed " << value.second.second << std::endl;
//            std::cout << value.second.first.toString() << std::endl;
//        }else{
//            return 0;
//        }
//    }

    std::cout << records.size() << std::endl;
    std::cout << "Buckets number: " << hash.buckets << std::endl;
    std::cout << "Overflow blocks: " << overflow.blocksCount << std::endl;

    hash.close();
    overflow.close();
}