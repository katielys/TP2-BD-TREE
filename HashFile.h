#ifndef TP2_BD_TREE_HASHFILE_H
#define TP2_BD_TREE_HASHFILE_H
#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include "Article.h"
using namespace std;
class HashFile{
 public:
     int hash_size;
     void createHash(char** argv );
     Article getArticleFromDisk(int id);

};
#endif //TP2_BD_TREE_HASHFILE_H
