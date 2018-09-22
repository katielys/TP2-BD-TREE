#ifndef ARTICLE_H
#define ARTICLE_H
#pragma once
#include <string>
#include <iostream>
#include <sstream>
#define MAX_SIZE_TITLE 300
#define MAX_SIZE_AUTOR 150
#define MAX_SIZE_SNIPPET 1024
#define MAX_SIZE_DATE 50

using namespace std;

class Article{
	public:
		Article (); // construtor sem parametro
		Article (int,int,int,char[MAX_SIZE_TITLE],char[MAX_SIZE_AUTOR],char[MAX_SIZE_SNIPPET],char[MAX_SIZE_DATE]); //
		~Article (); //Destrutor
		string toString();
	private :
		int id; // Codigo identificador do artigo
		int year ;// ano da publicação do artigo
		int quotes ; // numero de vezes que o artigo foi citado
		char title[MAX_SIZE_TITLE]; //nome do artigo
		char autorList[MAX_SIZE_AUTOR]; //lista dos autores
		char snippet[MAX_SIZE_SNIPPET] ; //resumo textual
		char date[MAX_SIZE_DATE]; //data e hora das atualizações
		void setYear(int);
		void setQuotes(int);
		void setID(int);
		void setTitle(char[MAX_SIZE_TITLE]);	
		void setSnippet(char[MAX_SIZE_SNIPPET]);
		void setAutors(char[ MAX_SIZE_AUTOR]);
		void setDate(char [ MAX_SIZE_DATE]);
		int getYear();
		int getID();
		int getQuotes();
		char* getTitle();
		char* getAutors();
		char* getSnipped();
		char* getDate();
};

#endif
