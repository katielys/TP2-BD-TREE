
#pragma once
#ifndef TP2_BD_TREE_PARSER_H
#define TP2_BD_TREE_PARSER_H

#include <vector>
#include <fstream>
#include "Block.h"

class Parser {
    public:
        vector<Article> readCSV(char ** argv);
        bool isNumber(string str);


    Parser();

};


#endif //TP2_BD_TREE_PARSER_H
