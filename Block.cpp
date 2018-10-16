///\file Block.cpp
#include <cstring>
#include "Block.h"

bool Block::insertRecord(const Article &article) {
    try {
        if (this->getFreeSpace() < sizeof(article)){ // if hasn't free space
            std::cout << "Uncapable to insert article on block, try overflow" << std::endl;
            return false; // ends the operation
        }
        // gets a pointer to the first free space location
        auto *articlePointer = reinterpret_cast<Article *>(&this->data[bytesOccupied]);
        std::memcpy(articlePointer, &article, sizeof(article)); // copy the content of the article in bytes to the free position
        this->bytesOccupied+= sizeof(article); // increments the quantity of occupied bytes
        std::cout << "Sucessfully inserted on block! " << std::endl;
        return true;
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

