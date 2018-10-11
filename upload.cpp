///\file upload.cpp Arquivo responsavel por criar o arquivo hash, o indice primario e indice secundario.
#include <iostream>
#include "HashFile.h"
using  namespace std;


int main (int argc, char** argv){

    HashFile p ;
    p.createHash(argv);
    //TODO a create hash ainda nao ta top pq tem 1 artigo por bloco
    //TODO : O bloco ainda falta eu terminar pq eu nao sei como validar essa cuceta
    //TODO essa getArticleFromDisk ta zoada nao usar . eu vou ajeitar isso depois
   // cout<< p.getArticleFromDisk(0).toString();

    return 0;
}


