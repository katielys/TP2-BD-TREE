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
    //vector <Article*> v ;
    int totalReg = 1;
    string snipped;
    //1.021.443
    while(file.good() && totalReg<1021433) {

        string line,dump,cell;
        getline(file, snipped, '\n');
        stringstream lineStream(snipped);

        vector<string> data;
        if (snipped.find(';') != std::string::npos) {
            while (std::getline(std::getline(lineStream, dump, '"'), cell, '"')) {
//                cout << cell <<endl;
                data.push_back(cell);
            }
        }
        string title ;
        title = data[1].c_str();
        char *charTitle = (char*) malloc(sizeof(char) * MAX_SIZE_TITLE);
       // charTitle = "";
        charTitle = const_cast<char *>(data[1].c_str());
       // char *charTitle = data[1];
        auto *charAutorlist = (char*) malloc(sizeof(char) * MAX_SIZE_AUTOR);
        charAutorlist = const_cast<char *>(data[3].c_str());

        auto *vSnipped = (char*) malloc(sizeof(char) * MAX_SIZE_SNIPPET);
        vSnipped = const_cast<char *>(data[6].c_str());


        auto *vDate = (char*) malloc(sizeof(char) * MAX_SIZE_DATE);
        vDate = const_cast<char *>(data[5].c_str());

        if (data[0].size()<1){
            data[0] = totalReg;
           // cout<<totalReg<<endl;

        }
        if(data[2].size()<3){
            data[2] = "0";
        }
        if(data[4].size()<18){
            data[4] = "0";
        }
        int mID = stoi(data[0].c_str());
        unsigned short int mYE = stoi(data[2].c_str());
        int mQ = stoi(data[4].c_str());
        data.clear();
        totalReg++;
        //Article::Article(int id_, unsigned short int year_, int quote, char title[MAX_SIZE_TITLE] , char autorList[MAX_SIZE_AUTOR] , char snippet[MAX_SIZE_SNIPPET] , char date[ MAX_SIZE_DATE] ) {
        Article *aux =  new Article(totalReg,mYE,mQ,charTitle, charAutorlist,vSnipped,vDate);
        //cout<<aux->toString();
        cout<<"---------"<<title<<endl;




    }
    cout<< "TOTAL : " << totalReg <<endl;
    file.close();
    return 0;

}

