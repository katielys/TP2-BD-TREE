#ifndef TP2_BD_TREE_HASHFILE_H
#define TP2_BD_TREE_HASHFILE_H
#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include "Parser.h"

namespace Hashing {

    const unsigned short HEADER_SIZE = sizeof(u_long);

    struct OverflowArea{
        FILE *fp;
        unsigned blocksCount;

        OverflowArea();

        explicit OverflowArea(const char *path);

        void close();

    };

    struct HashInstance{
        FILE *hashingFile;
        unsigned long buckets;

        HashInstance();

        explicit HashInstance(const char *path);

        void close();
    };

    void createHash(unsigned long numberOfRecords, unsigned int recordsPerBucket, const char *path);

    void createOverflow(const char *path);

    unsigned long insertOnHashFile(Article record, HashInstance &hash, OverflowArea &overflow);

    void insertOnOverflow(Article &record, OverflowArea &overflowArea, unsigned int &offset, bool &needsUpdate, bool alreadyLinked=false);

    pair<bool, pair<Article, unsigned int>> findRecord(unsigned int id, HashInstance &hash, OverflowArea &overflow);

    bool lookUpForRecordInOverflow(unsigned int id, Article &artAux,
                                                       OverflowArea &overflow, unsigned int &offset,
                                                       unsigned int &blocksPassed);

};
#endif //TP2_BD_TREE_HASHFILE_H
