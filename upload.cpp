#include <cstdlib>
#include <iostream>
#include <fstream>

using  namespace std;

int main (int argc, char** argv){
    ifstream file("C:\\Users\\Katiely\\Documents\\UFAM\\6periodo\\BD\\tp2\\TP2-BD-TREE\\artigo.csv");
    if(!file.is_open()){
        cout<<"ERROR CANT OPEN THE FILE"<<endl;
    }
    //cout<<"teste";
    string id;
    string title;
    string years;
    string autors;
    string att;
    string quotes;
    string snipped;
    int totalReg = 0;
    while(file.good()){
        getline(file,id,';');
        getline(file,title,';');
        getline(file,years,';');
        getline(file,autors,';');
        getline(file,quotes,';');
        getline(file,att,';');
        getline(file,snipped,'\n');
        cout<< "ID : "<<id <<endl;
        cout<< "TITLE : "<< title<<endl;
        cout<<"YEAR :" << years << endl;
        cout<<"Quotes:" << quotes <<endl;
        cout<<"AUTORS :" << autors<<endl;
        cout<<"DATE : "<< att <<endl;
        cout<<"SNIPPED: " << snipped<<endl;
        totalReg++;

    }
    cout<< "TOTAL : " << totalReg <<endl;
    file.close();
    return 0;

}

