#include "HashFile.h"
#include "Article.h"
#include "Block.h"
#include "Parser.h"
#include "PrimaryIndexBtree.h"
#include <vector>
#include <stdio.h>
#include <string.h>
#include <cctype>
using namespace std;

void HashFile::createHash(char** argv){
    Parser p ;
    vector<Article> v;
    v = p.readCSV(argv);
    FILE * pFile;
    PrimaryIndexBtree *primaryIndexBtree = primaryIndexBtree->initTree() ;
    primaryIndexBtree = primaryIndexBtree->createTree();


    Block current_block;
    pFile =fopen("/home/katiely/Documents/BD/TP2-BD-TREE/fileHash.bin", "wb");
    for (int i = 0;i < v.size();i++ ){
        current_block.insertArticleInTheBlock(v[i]);

        int position = v[i].getID() ;
        int a = primaryIndexBtree->insertBtree(primaryIndexBtree,position);
        cout<< a << " - - " ;
        fseek(pFile, position*sizeof(Block), SEEK_SET);
        fwrite(&current_block, sizeof(Block), 1, pFile);
        this->hash_size = this->hash_size +1;

    }
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



