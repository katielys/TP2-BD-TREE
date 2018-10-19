///\file Block.h header from Block
#include "Article.h"
#ifndef TP2_BD_TREE_BLOCK_H
#define TP2_BD_TREE_BLOCK_H
#pragma once
using namespace std;

#define MASK_VALID 666 //!< MASK_VALID is a flag to know if a block is valid or not
#define BLOCK_SIZE 4096 //!< SIZE FROM A BLOCk
#define DATA_SIZE 4083 // 4096 - 12 - 1 = 4083 bytes available for ocupation

typedef unsigned char BYTE;

struct Block{ // disk block representation
    //header 13 bytes
    unsigned int verificationMask;  // 4 bytes
    unsigned int bytesOccupied;     // 4 bytes
    unsigned int nextBlock;         // 4 bytes offset used as reference to find the overflow block
    bool overflow;                  // 1 byte has overflow sequence or not
    BYTE data[DATA_SIZE];           // array of bytes to keep the records

    // tries to insert a record in the current block
    bool insertRecord(const Article &article);

    // returns the quantity of free space in a block
    unsigned int getFreeSpace();

    // search for a record with id passed as parameter, returns true if found or returns false if not found
    bool lookUpforRecord(unsigned int id, size_t recordSize, Article &aux);

    // default constructor
    Block();
};


#endif //TP2_BD_TREE_BLOCK_H

