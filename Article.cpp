#include "Article.h"
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
using namespace std;
Article::Article(){
//	return NULL;
}

Article::~Article(){
	
}
void Article::setYear(int year_){
	year = year_;
}
void Article::setID(int i){
	id = i;
}
void Article::setQuotes(int q){
	quotes = q;
}
void Article::setTitle(char title_[MAX_SIZE_TITLE]){
	strcpy(this->title,title_);
}
void Article::setSnippet(char snip[MAX_SIZE_SNIPPET]){
	strcpy(snippet,snip);
}
void Article::setDate(char d[MAX_SIZE_DATE]){
	strcpy(date,d);
}

void Article::setAutors(char autor[ MAX_SIZE_AUTOR]){
	strcpy(autorList,autor);
}
Article::Article(int id_,int year_, int quote, char title[MAX_SIZE_TITLE] , char autorList[MAX_SIZE_AUTOR] , char snippet[MAX_SIZE_SNIPPET] , char date[ MAX_SIZE_DATE] ) {
	setID(id_);
	setYear(year_);
	setTitle(title);
	setQuotes(quote);
	setSnippet(snippet);
	setDate(date);
	setAutors(autorList);
	
}
int Article::getYear(){
	return year;
}
int Article::getID(){
	return id;
}
int Article::getQuotes(){
	return quotes;
}
std::string Article::toString(){
	string ret ;
	ostringstream str1; 
    str1 << getID(); 
  	ostringstream st; 
  	ostringstream s; 
	ret.append("\nID: ");
	ret.append(str1.str() );
	
	ret.append("\nTitle: ");
	ret.append(title);
	ret.append("\nYear: ");
	st<< getYear();
	ret.append(st.str());
	ret.append("\nQuotes: ");
	s<< getQuotes();
	ret.append(s.str());
	ret.append("\nAutors: ");
	ret.append(autorList);
	ret.append("\nSnipped: ");
	ret.append(snippet);
	ret.append("\nDate:");
	ret.append(date);
	return ret;
}

int main (){
	Article *teste ;
	teste = new Article(1,6,10,"tidtu","ALTIGRAN,NAKAMURA ","melhor aritgo do unibverso","10.09.09 29/32" ) ;
	cout << teste->toString() ;
}


