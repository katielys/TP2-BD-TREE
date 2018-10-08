///\file Article.h
#ifndef ARTICLE_H
#define ARTICLE_H
#pragma once
#include <string>
#include <iostream>
#include <sstream>
#define MAX_SIZE_TITLE 300
#define MAX_SIZE_AUTOR 150
#define MAX_SIZE_SNIPPET 1024
#define MAX_SIZE_DATE 19

///\autor Katiely
using namespace std;

class Article{
	public:
    /// Constructor without param
		Article ();
		Article (int,unsigned short int,int,char[MAX_SIZE_TITLE],char[MAX_SIZE_AUTOR],char[MAX_SIZE_SNIPPET],char[MAX_SIZE_DATE]); ///Constructor deafult
		~Article (); //Destrutor
		string toString();/// Function to return all information from a instance at this class
	private :
		unsigned int id; /// Codigo identificador do artigo
		unsigned short int year ;/// ano da publicacaoo do artigo
		int quotes ; /// numero de vezes que o artigo foi citado
		char title[MAX_SIZE_TITLE]; ///nome do artigo
		char autorList[MAX_SIZE_AUTOR]; ///lista dos autores
		char snippet[MAX_SIZE_SNIPPET] ; ///resumo textual
		char date[MAX_SIZE_DATE]; ///data e hora das atualiza��es
		/** Setters and getters from object article*/
		void setYear(int);
		void setQuotes(int);
		void setID(int);
		void setTitle(char[MAX_SIZE_TITLE]);	
		void setSnippet(char[MAX_SIZE_SNIPPET]);
		void setAutors(char[ MAX_SIZE_AUTOR]);
		void setDate(char [ MAX_SIZE_DATE]);
		unsigned short int getYear();
		int getID();
		int getQuotes();
		char* getTitle();
		char* getAutors();
		char* getSnipped();
		char* getDate();
};

#endif
