///\file Article.h Lib from class Article
///@author Katiely dos santos
#ifndef ARTICLE_H
#define ARTICLE_H
#include <string>
#include <iostream>
#include <sstream>
#include <cstring>
#define MAX_SIZE_TITLE 300 //!< Tamanho maximo de um titulo de artigo
#define MAX_SIZE_AUTOR 150 //!<Tamanho maximo da lista de artigo
#define MAX_SIZE_SNIPPET 1024//!<tamanho maximo de um resumo
#define MAX_SIZE_DATE 19//!<tamanho maximo de uma ultima autizacao de data e hora,este valor foi definido atraves de testes no arquivo artigo.csv

using namespace std;

class Article{
	public:
    /// Constructor without param
		Article ();
		Article (int id,unsigned short int year,int,char[MAX_SIZE_TITLE],char[MAX_SIZE_AUTOR],char[MAX_SIZE_SNIPPET],char[MAX_SIZE_DATE]); ///Constructor deafult
		~Article ();
		string toString();//!<Function to return all information from a instance at this class
		unsigned int getID();
        void setID(int id);
	    unsigned short int getYear();
	    char* getTitle();
	private :
		unsigned int id; /// Codigo identificador do artigo
		unsigned int year ;/// ano da publicacaoo do artigo
		int quotes ; /// numero de vezes que o artigo foi citado
		char title[MAX_SIZE_TITLE]; ///nome do artigo
		char autorList[MAX_SIZE_AUTOR]; ///lista dos autores
		char snippet[MAX_SIZE_SNIPPET] ; ///resumo textual
		char date[MAX_SIZE_DATE]; ///data e hora das atualiza��es
		/** Setters and getters from object article*/

    /**
* Set a year to article
* @author Katiely Santos
* @param  a year
* @return void
*/
		void setYear(int);

    /**
* Set a total quotes
* @author Katiely Santos
* @param  number of citations
* @return void
*/
		void setQuotes(int);

    /**
* Set title from a article
* @author Katiely Santos
* @param  the title
* @return void
*/

		void setTitle(char[MAX_SIZE_TITLE]);


    /**
* Set a snipped from a article
* @author Katiely Santos
* @param  the snipped from article
* @return void
*/
		void setSnippet(char[MAX_SIZE_SNIPPET]);

    /**
* Set the authors to a article
* @author Katiely Santos
* @param  the authors
* @return void
*/
		void setAutors(char[ MAX_SIZE_AUTOR]);
		void setDate(char [ MAX_SIZE_DATE]);
		int getQuotes();
		char* getAutors();
		char* getSnipped();
		char* getDate();
};

#endif
