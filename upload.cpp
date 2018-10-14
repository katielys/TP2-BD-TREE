///\file upload.cpp Arquivo responsavel por criar o arquivo hash, o indice primario e indice secundario.
#include <iostream>
#include "HashFile.h"
#include "Block.h"
#include "Parser.h"

using  namespace std;


int main (int argc, char** argv){

    HashFile hash ;

    //hash.createHash(argv);
    for (int i =193; i< 200;i++){
        cout<< hash.getArticleFromDisk(i).toString();
    }

    cout<< hash.getArticleFromDisk(777989).toString();


    return 0;
}


