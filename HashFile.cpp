
#include "HashFile.h"

void Hashing::createHash(unsigned long numberOfRecords, unsigned int recordsPerBucket, const char *path) {
    FILE *hashingFile = fopen(path, "wb+");

    if(hashingFile == nullptr){
        std::cout << "Cannot open the file " << path << std::endl;
        return;
    }
//    unsigned long buckets = (numberOfRecords / recordsPerBucket) + 1;
    unsigned long buckets = 30000; // prime number
    Block emptyBlock;

    fseek(hashingFile, 0, SEEK_SET);
    fwrite(&buckets, HEADER_SIZE, 1, hashingFile); // writing the quantity of buckets in the file
    try {
        for (int i = 0; i < buckets; i++) {
            fseek(hashingFile, (i * sizeof(Block) + HEADER_SIZE), SEEK_SET);
            fwrite(&emptyBlock, sizeof(Block), 1, hashingFile);
        }
        fclose(hashingFile);
    }catch (const char e){
        throw e;
    }
}

void Hashing::createOverflow(const char *path) {
    FILE *overflowFile = fopen(path, "wb+");
    if(overflowFile == nullptr){
        std::cout << "Cannot open the file " << path << std::endl;
        return;
    }

    fseek(overflowFile, 0, SEEK_SET);
    int blockCount = 0;
    fwrite(&blockCount, HEADER_SIZE, 1, overflowFile); // writing the quantity of buckets in the file
    fclose(overflowFile);
}

unsigned long Hashing::insertOnHashFile(Article record, Hashing::HashInstance &hash, Hashing::OverflowArea &overflow) {
    unsigned long mappedBlock = record.getID() % hash.buckets; // hashing function
    Block aux;
    bool update = false;

    fseek(hash.hashingFile, (mappedBlock * sizeof(Block)) + HEADER_SIZE, SEEK_SET); // seeks the bucket's position
    size_t a = fread(&aux, sizeof(Block), 1, hash.hashingFile); // reads the bucket

    if (a==1 && aux.verificationMask == MASK_VALID) { // valid

        std::cout << "trying to allocate record with id " << record.getID() << " on block " << mappedBlock << std::endl;
        bool inserted = aux.insertRecord(record);

        if(!inserted){ // if not inserted, goes to overflow

            insertOnOverflow(record, overflow, aux.nextBlock, update, aux.overflow);

            if(update){ // update the current block if necessary
                aux.overflow = true;
                fseek(hash.hashingFile, (mappedBlock * sizeof(Block) + HEADER_SIZE), SEEK_SET);
                fwrite(&aux, sizeof(Block), 1, hash.hashingFile);
            }
        } else{
            fseek(hash.hashingFile, (mappedBlock * sizeof(Block) + HEADER_SIZE), SEEK_SET);
            fwrite(&aux, sizeof(Block), 1, hash.hashingFile);
        }
    }
    return mappedBlock;
}

void Hashing::insertOnOverflow(Article &record, Hashing::OverflowArea &overflowArea, unsigned int &offset,
                               bool &needsUpdate, bool alreadyLinked) {

    unsigned int currentOffset = offset;
    if(alreadyLinked){ // if a block already has an linked block in overflow area
        Block aux;

        fseek(overflowArea.fp, (currentOffset * sizeof(Block)), SEEK_SET); // seeks the position of the linked block
        size_t a = fread(&aux, sizeof(Block), 1, overflowArea.fp); // reads the block

        if (a==1 && aux.verificationMask == MASK_VALID) { // if the is valid
            bool inserted = aux.insertRecord(record); // tries to insert
            if(!inserted){ // not inserted

                insertOnOverflow(record, overflowArea, aux.nextBlock, needsUpdate, aux.overflow);

                if(needsUpdate){ // update the current block if necessary
                    aux.overflow = true;
                    fseek(overflowArea.fp, (currentOffset * sizeof(Block)), SEEK_SET);
                    fwrite(&aux, sizeof(Block), 1, overflowArea.fp);
                    needsUpdate = false;
                }

            }else{ // updates the current block with the new record
                fseek(overflowArea.fp, (currentOffset * sizeof(Block)), SEEK_SET);
                fwrite(&aux, sizeof(Block), 1, overflowArea.fp);
            }
            return;
        }
    } else{
        auto newBlockPos = overflowArea.blocksCount + Hashing::HEADER_SIZE; // position of the new block
        Block aux;
        offset = newBlockPos; // sets the previous block new offset

        bool inserted = aux.insertRecord(record);
        if(!inserted){
            std::cout << "Cannot insert record on onverflow" << std::endl;
            return;
        }
        // writes the new block
        fseek(overflowArea.fp, (newBlockPos * sizeof(Block)), SEEK_SET); // modularizar em um funçao
        fwrite(&aux, sizeof(Block), 1, overflowArea.fp);

        // updating the header
        overflowArea.blocksCount++;
        fseek(overflowArea.fp, 0, SEEK_SET);
        fwrite(&overflowArea.blocksCount, HEADER_SIZE, 1, overflowArea.fp);

        needsUpdate = true; // sinalize for previous block update
        return;
    }
}

std::pair<bool, std::pair<Article, unsigned int>> Hashing::findRecord(unsigned int id, Hashing::HashInstance &hash,
                                                                      Hashing::OverflowArea &overflow) {
    unsigned long mappedblock = id % hash.buckets;
    unsigned int passedBlocks = 1;
    Article artAux;
    Block aux;

    fseek(hash.hashingFile, (mappedblock * sizeof(Block) + HEADER_SIZE), SEEK_SET);
    size_t check = fread(&aux, sizeof(Block), 1, hash.hashingFile); // reads a block

    if (check == 1 && aux.verificationMask == MASK_VALID){
        if (aux.lookUpforRecord(id, sizeof(Article), artAux)){
            std::cout << "Found on bucket " << mappedblock << std::endl;
//            std::cout << artAux.toString() << std::endl;
            return std::make_pair(true, std::make_pair(artAux, passedBlocks));

        }else if (aux.overflow){
            std::cout << "Not fount at hashing bucket\nTrying to lookup in overflow area.. "<< std::endl;
            bool found = lookUpForRecordInOverflow(id, artAux, overflow, aux.nextBlock, passedBlocks);
            if(found){
                return std::make_pair(true, std::make_pair(artAux, passedBlocks));
            }
        }
    }

    std::cout << "Record with id " << id << " not found at all :(" << std::endl;
    return std::make_pair(false, std::make_pair(artAux,passedBlocks));
}

bool Hashing::lookUpForRecordInOverflow(unsigned int id, Article &artAux, Hashing::OverflowArea &overflow,
                                        unsigned int &offset, unsigned int &blocksPassed) {

    Block aux;
    fseek(overflow.fp, (offset * sizeof(Block)), SEEK_SET); // seeks the position of the linked block
    size_t a = fread(&aux, sizeof(Block), 1, overflow.fp); // reads the block

    if (a==1 && aux.verificationMask == MASK_VALID) { // if the is valid
        blocksPassed++;
        if (aux.lookUpforRecord(id, sizeof(Article), artAux)){
            std::cout << "Found in a overflow block" << std::endl;
//            std::cout << artAux.toString() << std::endl;
            return true;
        }else if (aux.overflow){
            return lookUpForRecordInOverflow(id, artAux, overflow, aux.nextBlock, blocksPassed);
        }
    }

    return false;
}


Hashing::HashInstance::HashInstance(const char *path) {
    this->hashingFile = fopen(path, "rb+");
    if(this->hashingFile == NULL){
        std::cout << "Cannot open the file " << path << std::endl;
        return;
    }
    fread(&this->buckets, Hashing::HEADER_SIZE, 1, this->hashingFile);
}

Hashing::HashInstance::HashInstance() {}

void Hashing::HashInstance::close() {
    fclose(this->hashingFile);
}

Hashing::OverflowArea::OverflowArea() {}

Hashing::OverflowArea::OverflowArea(const char *path) {
    this->fp = fopen(path, "rb+");
    if(this->fp == NULL){
        std::cout << "Cannot open the file " << path << std::endl;
        return;
    }
    fread(&this->blocksCount, Hashing::HEADER_SIZE, 1, this->fp);
}

void Hashing::OverflowArea::close() {
    fclose(this->fp);
}
