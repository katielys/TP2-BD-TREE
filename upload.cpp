#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include "Article.h"
#include <sstream>
#include <stdio.h>
#include <string.h>
using  namespace std;

int main (int argc, char** argv){
    ifstream file("C:\\Users\\Katiely\\Documents\\UFAM\\6periodo\\BD\\tp2\\TP2-BD-TREE\\artigo.csv");
    if(!file.is_open()){
        cout<<"ERROR CANT OPEN THE FILE"<<endl;
    }
    vector <Article*> v ;
    int totalReg = 0;
    string snipped;
    while(file.good()) {

        string line,dump,cell;
        getline(file, snipped, '\n');
        stringstream lineStream(snipped);

        vector<string> data;
        if (snipped.find(';') != std::string::npos) {
            while (std::getline(std::getline(lineStream, dump, '"'), cell, '"')) {
                cout << cell <<endl;
                data.push_back(cell);
            }
        }
        string title ;
        title = data[1];
        auto *charTitle = (char*) malloc(sizeof(char) * MAX_SIZE_TITLE);
        charTitle = const_cast<char *>(data[1].c_str());

        auto *charAutorlist = (char*) malloc(sizeof(char) * MAX_SIZE_AUTOR);
        charAutorlist = const_cast<char *>(data[3].c_str());

        auto *vSnipped = (char*) malloc(sizeof(char) * MAX_SIZE_SNIPPET);
        vSnipped = const_cast<char *>(data[6].c_str());


        auto *vDate = (char*) malloc(sizeof(char) * MAX_SIZE_DATE);
        vDate = const_cast<char *>(data[5].c_str());


        int mID = stoi(data[0].c_str());
        int mYE = stoi(data[2].c_str());
        int mQ = stoi(data[4].c_str());
        data.clear();
        totalReg++;
//        if (totalReg > 100) {
//            break;
//        }

    }
    cout<< "TOTAL : " << totalReg <<endl;
    file.close();
    return 0;

}

