///\file Block.h header from Block
///\author Tiago Santos

#include "Article.h"
#ifndef TP2_BD_TREE_BLOCK_H
#define TP2_BD_TREE_BLOCK_H
#pragma once
using namespace std;

#define MASK_VALID 666 //!< MASK_VALID is a flag to know if a block is valid or not
#define BLOCK_SIZE 4096 //!< SIZE FROM A BLOCk
#define DATA_SIZE 4083 //!< 4096 - 13 (header) = 4083 bytes available to alocate records

typedef unsigned char BYTE;



/**
 *  Disk block representation, it's important to say that we've made this structure to serve as block and bucket
 *  due the simplicity that it gave to us. This structure has a special attribute (nextBlock) that serves as pointer to
 *  the next linked block, simulating the scheme of buckets and overflow blocks found in literature.
 */
struct Block{
    /// header 13 bytes
    unsigned int verificationMask;  //!< 4 bytes
    unsigned int bytesOccupied;     //!< 4 bytes
    unsigned int nextBlock;         //!< 4 bytes offset used as reference to find the overflow (or next) block
    bool overflow;                  //!< 1 byte has overflow sequence or not
    BYTE data[DATA_SIZE];           //!< array of bytes to keep the records

    /**
       * tries to insert a record in the current block
       * @author Tiago Santos
       * @param  article object to be inserted
       * @return Returns a pair<bool, unsigned short>, the first value indicates if the
       * record is inserted and the second indicates the logical position (0,1,2,..) of him inside the block
       */
    pair<bool, unsigned short> insertRecord(const Article &article);

    /// returns the quantity of free space in a block
    /// @author Tiago Santos
    unsigned int getFreeSpace();

    /**
       * search for a record with id passed as parameter, returns true if found or returns false if not found
       * @author Tiago Santos
       * @param id Is the identifier number of the record to be searched
       * @param recordSize Is the size  of record that will be searched
       * @param aux Article object to keep the data of the found record, only will be filled if found the record
       * @return Returns a bool to indicate if found or not the record
       */
    bool lookUpforRecord(unsigned int id, size_t recordSize, Article &aux);

    /**
       * search for a record by his offset
       * @author Tiago Santos
       * @param offset is the logical position of the record in a block, i.e. sequential position of record inside the block
       * @param aux Article object to keep the data of the found record, only will be filled if found the record
       */
    void lookUpforRecordByOffset(unsigned short offset, Article &aux);

    /// default constructor
    /// @author Tiago Santos
    Block();
};


#endif //TP2_BD_TREE_BLOCK_H

