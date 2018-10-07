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
    ifstream file(argv[1]);
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
        try {

            string title;
            title = data[1];
            char charTitle[MAX_SIZE_TITLE]; // titulo
            strcpy(charTitle, title.c_str());

            char charAutorlist[MAX_SIZE_AUTOR]; // lista de autores
            auto autorList = data[3];
            strcpy(charAutorlist, autorList.c_str());

            char vSnipped[MAX_SIZE_SNIPPET]; // resumo
            auto snipet = data[6];
            strcpy(vSnipped, snipet.c_str());


            char vDate[MAX_SIZE_DATE]; // data
            auto date = data[5];
            strcpy(vDate, date.c_str());

//        if (data[0].size()<1){
//            data[0] = totalReg;
//           // cout<<totalReg<<endl;
//
//        }
//        if(data[2].size()<3){
//            data[2] = "0";
//        }
//        if(data[4].size()<18){
//            data[4] = "0";
//        }

            int mID = stoi(data[0].c_str());
            unsigned short int mYE = stoi(data[2].c_str());
            int mQ = stoi(data[4].c_str());
//            data.clear();
            totalReg++;
            //Article::Article(int id_, unsigned short int year_, int quote, char title[MAX_SIZE_TITLE] , char autorList[MAX_SIZE_AUTOR] , char snippet[MAX_SIZE_SNIPPET] , char date[ MAX_SIZE_DATE] ) {
            Article *aux = new Article(totalReg, mYE, mQ, charTitle, charAutorlist, vSnipped, vDate);
            cout << aux->toString();
//        cout<<"---------"<<title<<endl;
        }catch (exception &e){
            cout << e.what() << endl;
        }

    }
    cout<< "TOTAL : " << totalReg <<endl;
    file.close();
    return 0;

}

