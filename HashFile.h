///\file HashFile.h Namespace that group all functions that manipulate the hashing file
///\author Tiago Santos

#ifndef TP2_BD_TREE_HASHFILE_H
#define TP2_BD_TREE_HASHFILE_H
#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include "Parser.h"

/** This namespace group all function that manipulate the hashing,
 * it's important to note that in this work a buckets has the same size of a block (4096),
 * thus the same structure is used as bucket. We decided to create two separated files, the first for the main hashing
 * file that will contain the buckets, and the second to keep the overflow blocks. If a bucket in the main file needs
 * a overflow block, a new overflow block will be allocated and the bucket will be updated to keep the address of his
 * first overflow block. If needed overflow blocks can also keep address of subsequent overflow blocks, forming a
 * logical linked list.
*
 **/
 namespace Hashing {

    /** Indicates the size of header, the header is summarily used
    *   to keep tracking about the number of blocks allocated in hashing/overflow file
    **/
     const unsigned short HEADER_SIZE = sizeof(u_long);

  /**
   *  Struct address is the logical address of a record in the data file, indicates the exact location of an record and
   *  is used in the index trees as "data pointers"
   *  @author Tiago Santos
   **/
    struct Address{

        unsigned int offset;        //!< block offset of respective file (hashing|overflow)
        unsigned short blockOffset; //!< offset of the record inside a block (can be 0 or 1).. change for a char
        bool overflow = false;      //!< is in overflow area or not

        /// @author Tiago Santos
        /// Default constructor
        Address();
    };

    /// This struct is simply used to keep a instance of the overflow area, it keeps the file pointer an a counter of blocks inside the area
    /// @author Tiago Santos
    struct OverflowArea{
        FILE *fp;             //!< file pointer to the overflow file
        unsigned blocksCount; //!< counter of blocks in the overflow area

        /// Default constructor
        /// @author Tiago Santos
        OverflowArea();

        /// Constructor that receives a path file and open the specified file, thus initializing the overflow area.
        /// @author Tiago Santos
        explicit OverflowArea(const char *path);

        /// Closes the file pointer of the overflow area
        /// @author Tiago Santos
        void close();

    };

    /// This struct is simply used to keep a instance of the hashing file, it keeps the file pointer an a counter of buckets (a.k.a. blocks) inside
    struct HashInstance{
        FILE *hashingFile;  //!< file pointer to the hashing file
        unsigned long buckets; //!< buckets quantity in the current hash instance

        /// Deafault constructor
        HashInstance();

        /// Constructor that receives a path file and open the specified file, thus initializing the hashing file.
        /// @author Tiago Santos
        explicit HashInstance(const char *path);

        /// Closes the file pointer the hash instance
        /// @author Tiago Santos
        void close();
    };
     /**
       * Initiates the hash file, allocating empty buckets. The quantity of buckets is determinated by the math:
       * (numberOfRecord/recordsPerBucket) + 1. We also, can use a prime number to avoid collisions.
       *
       * @author Tiago Santos
       * @param numberOfRecords Is used to determinate how many buckets the file must have.
       * @param recordsPerBucket Also used to determinate how many buckets the file must have.
       * @param path Path to the physical file that will keep the buckets.
       */
    void createHash(unsigned long numberOfRecords, unsigned int recordsPerBucket, const char *path);

     /**
       * Initiates the overflow file, allocating just putting the header (quantity of blocks inside the file)
       * that int this moment will be 0
       *
       * @author Tiago Santos
       * @param numberOfRecords Is used to determinate how many buckets the file must have.
       * @param recordsPerBucket Also used to determinate how many buckets the file must have.
       * @param path Path to the physical file that will keep the buckets.
       */
    void createOverflow(const char *path);

     /**
       * Inserts a record in the hash file and returns its address inside the data file.
       *  Uses the hash function (see the implementation please) to get the mapped block and tries to alloc in there.
       *  If succeeds, returns the allocated address immediately. If not, tries to alloc the record in the overflow area.
       *
       *
       * @author Tiago Santos
       * @param record Record that will be allocated.
       * @param hash Current instance of the hashing.
       * @param overflow Current instance of the overflow area.
       */
    Address insertOnHashFile(Article record, HashInstance &hash, OverflowArea &overflow);
     /**
   *   Inserts a record in the overflow file. It's a recursive implementation.
   *
   * @author Tiago Santos
   * @param record Record that will be allocated.
   * @param overflowArea Current instance of the overflow area.
   * @param offset Logical address (0,1,2,4,...) of the overflow block. Used to seek the block.
   * @param needsUpdate Bool that indicates if the previous block in the logical linked list will need to be updated.
   * @param address Address object passed by reference to keep the final address of the allocated record.
   * @param alreadyLinked Indicates if a bucket/block already has a subsequent linked block.
   */
    void insertOnOverflow(Article &record, OverflowArea &overflowArea, unsigned int &offset, bool &needsUpdate,
                          Address &address, bool alreadyLinked=false);

     /// @author Tiago Santos
     pair<bool, pair<Article, unsigned int>> findRecord(unsigned int id, HashInstance &hash, OverflowArea &overflow);

     /// @author Tiago Santos
     bool lookUpForRecordInOverflow(unsigned int id, Article &artAux,
                                   OverflowArea &overflow, unsigned int &offset,
                                   unsigned int &blocksPassed);
    /// @author Tiago Santos
    Article getRecordByAddress(Address address, HashInstance &hash, OverflowArea &overflow);

};
#endif //TP2_BD_TREE_HASHFILE_H
