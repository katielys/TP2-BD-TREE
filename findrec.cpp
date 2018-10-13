#include <iostream>
#include "HashFile.h"
///\autor: Katiely
using namespace std;
bool findReg(int id){

    HashFile hashFile ;

    Article aux;
    aux = hashFile.getArticleFromDisk(id);
    int number = sizeof(aux)/sizeof(Article);
    cout<<"Numero de blocos lidos: "<<number<<endl;
    if(aux.getYear()!=0 && aux.getID()!=0){
        cout<<aux.toString();

        return true;

    }
    return false;

}
int main(int argc, char** argv){
    //TODO : Retornar o numero total de blocos lidos
    //TODO : to esperando resposta do monitor para mudar o hash pq ele disse que o hash perfeito ele ia descontar ponto
    int numberReg =  stoi(argv[1]);
    if(!findReg(numberReg)){
        cout<<"ID nao encontrado "<<endl;
    }

}