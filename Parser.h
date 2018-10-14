#ifndef TP2_BD_TREE_PARSER_H
#define TP2_BD_TREE_PARSER_H

using namespace std;

#include <iostream>
#include <vector>
#include "Article.h"
class Parser {
    public:
        string path;
        vector<Article> readCSV(char ** argv);
        bool isNumber(string str);


};


#endif //TP2_BD_TREE_PARSER_H
