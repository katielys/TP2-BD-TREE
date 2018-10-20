///\file Block.cpp
#include "Block.h"

std::pair<bool, unsigned short> Block::insertRecord(const Article &article) {
    unsigned short offset;

    try {
        if (this->getFreeSpace() < sizeof(article)){ // if hasn't free space
            std::cout << "Uncapable to insert article on block, try overflow" << std::endl;
            return std::make_pair(false, offset); // ends the operation
        }
        // gets a pointer to the first free space location
        auto *articlePointer = reinterpret_cast<Article *>(&this->data[bytesOccupied]);
        std::memcpy(articlePointer, &article, sizeof(article)); // copy the content of the article in bytes to the free position
        this->bytesOccupied+= sizeof(article); // increments the quantity of occupied bytes
        offset = (this->bytesOccupied / sizeof(article) - 1); // -1 to make offset begin from 0

        std::cout << "Sucessfully inserted on block! " << std::endl;

        return std::make_pair(true, offset);
    }catch (const char *e){
        throw e;
    }
}

unsigned int Block::getFreeSpace() {
    return (DATA_SIZE - this->bytesOccupied);
}

Block::Block() {
    this->verificationMask = MASK_VALID;
    this->bytesOccupied = 0;
    this->overflow = false;
}

bool Block::lookUpforRecord(unsigned int id, size_t recordSize, Article &aux) {
    if (this->bytesOccupied > 0 && (this->bytesOccupied % recordSize) == 0) {
        unsigned int curPos = 0;
        while (curPos < this->bytesOccupied) { // searchs until the last alocated byte is reached
            auto *articlePointer = reinterpret_cast<Article *>(&this->data[curPos]);
            std::memcpy(&aux, articlePointer, recordSize); // gets the article from the block
            if(aux.getID() == id){
                return true;
            }
            curPos+= recordSize;
        }
    }
    return false;
}

void Block::lookUpforRecordByOffset(unsigned short offset, Article &aux) {

    if (this->bytesOccupied > 0 && (this->bytesOccupied % sizeof(Article)) == 0) {
        // seeks by offset passed as parameter
        auto *articlePointer = reinterpret_cast<Article *>(&this->data[sizeof(Article) * offset]);
        std::memcpy(&aux, articlePointer, sizeof(Article)); // gets the article from the block
    }
}
