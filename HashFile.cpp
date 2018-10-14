#include "HashFile.h"
#include "Article.h"
#include "Block.h"
#include <vector>
#include <stdio.h>
#include <string.h>
using namespace std;

void HashFile::createHash(char** argv){
    this->hash_size= 0;
    ifstream file(argv[1]);
    if(!file.is_open()){
        cout<<"ERROR CANT OPEN THE FILE"<<endl;
    }
    vector <Article> v ;
    int totalReg = 1;
    string snipped;
    while(getline(file, snipped)) {

        if (snipped.empty()){
            continue;
        }

        string dump,cell;

        stringstream lineStream(snipped); //autor : Tiago
        vector<string> data;
        if (snipped.find(';') != std::string::npos) {
            while (std::getline(std::getline(lineStream, dump, '"'), cell, '"')) {
//                cout << cell <<endl;
                // tratando inconsistencias (nao sei explicar essa parte, foi tentativa e erro)
                /// Tratamento de incosistencias de artigos que ha ausencia de algum campo
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
            int id ;
            unsigned short mYE; // ano de publicacao
            //id = stoi(data[0].c_str());
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
            Article current = Article(totalReg, mYE, mQ, charTitle, charAutorlist, vSnipped, vDate);
           // cout << current.toString();
            v.push_back(current);
        }catch (const char *e){
            throw e;
        }
    }
    file.close();
    cout<< "\n\nTOTAL : " << totalReg <<endl;

    FILE * pFile;
    pFile = nullptr;
    this->hash_size = totalReg;
    Block current_block ;
    //Article artAux;
    pFile =fopen("fileHash.bin", "wb");
    //cout<< pFile;
    for (int i = 0;i < v.size();i++ ){
        current_block.insertArticleInTheBlock(v[i]);
        //cout<<current_block.article.toString()<<endl;
        int position = v[i].getID() ;

        fseek(pFile, position*sizeof(Block), SEEK_SET);
        fwrite(&current_block, sizeof(Block), 1, pFile);
        this->hash_size = this->hash_size +1;

    }
    //this->hash_size  = totalReg;
    v.clear();
    fclose(pFile);


}

Article HashFile::getArticleFromDisk(int id) {
    FILE *fp;
    fp = fopen("/home/katiely/Documents/BD/TP2-BD-TREE/fileHash.bin", "rb");
    if (fp==NULL){
        cout<<"Cant open the file"<<endl<<endl;
        return Article();
    }

    Block aux;
    Article top;
    fseek(fp,id*sizeof(Block),SEEK_SET);
    //cout<< (ftell(fp)/sizeof(Block)) <<endl;
    size_t  a =fread(&aux, sizeof(Block), 1, fp);
    top = aux.article;
    fseek(fp,0,SEEK_END);
    cout<<"Numero total de blocos  "<< ftell(fp)/ sizeof(Block)<<endl;
    if(a ==1 ){

        return top;
    }
    cout<< "Error : cant read the file" <<endl;
    fclose(fp);
    return Article();

}



