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
//    Article *curArticle;

    ifstream file(argv[1]);
    if(!file.is_open()){
        cout<<"ERROR CANT OPEN THE FILE"<<endl;
    }
    //vector <Article*> v ;
    int totalReg = 1;
    string snipped;
    //1.021.443
    while(getline(file, snipped)) {

        if (snipped.empty()){
            continue;
        }

        string dump,cell;

        stringstream lineStream(snipped);
        vector<string> data;
        if (snipped.find(';') != std::string::npos) {
            while (std::getline(std::getline(lineStream, dump, '"'), cell, '"')) {
//                cout << cell <<endl;
                // tratando inconsistencias (nao sei explicar essa parte, foi tentativa e erro)
                if(dump == ";;"){
                    data.emplace_back("NULL");
                    dump = ";";
                    data.push_back(cell);
                }else {
                    if (data.size() >  1) {
                        if (!cell.empty() && dump == ";") {
                            data.push_back(cell);
                        }
                    } else {
                        data.push_back(cell);
                    }
                }
                // fim do tratamento de inconsistencias
            }
        }
        try {

            size_t registerSize = data.size();
            string title;
            char charTitle[MAX_SIZE_TITLE]; // titulo
            char charAutorlist[MAX_SIZE_AUTOR]; // lista de autores
            char vSnipped[MAX_SIZE_SNIPPET]; // resumo
            char vDate[MAX_SIZE_DATE]; // data
            int mQ; // citacoes
            unsigned short mYE; // ano de publicacao

            if (registerSize > 4){
                mQ = stoi(data[4].c_str());
            }

            if (registerSize > 2){
                mYE =  static_cast<unsigned short>(stoi(data[2].c_str()));
            }

            if (registerSize > 1) {
                title = data[1].substr(0,MAX_SIZE_TITLE);
                strcpy(charTitle, title.c_str());
            }

            if(registerSize > 3) {
                auto autorList = data[3].substr(0,MAX_SIZE_AUTOR);
                strcpy(charAutorlist, autorList.c_str());
            }

            if (data.size() > 6) {
                auto snipet = data[6].substr(0,MAX_SIZE_SNIPPET);
                strcpy(vSnipped, snipet.c_str());
            }

            if (registerSize > 5) {
                auto date = data[5].substr(0,MAX_SIZE_DATE);
                strcpy(vDate, date.c_str());
            }

            totalReg++;
            //Article::Article(int id_, unsigned short int year_, int quote, char title[MAX_SIZE_TITLE] , char autorList[MAX_SIZE_AUTOR] , char snippet[MAX_SIZE_SNIPPET] , char date[ MAX_SIZE_DATE] ) {
            Article current = Article(totalReg, mYE, mQ, charTitle, charAutorlist, vSnipped, vDate);
            cout << current.toString();

        }catch (const char *e){
            throw e;
        }
    }
    cout<< "\n\nTOTAL : " << totalReg <<endl;
    return 0;
}

