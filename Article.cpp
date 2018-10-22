///\file Article.cpp classe para representar o artigo
#include "Article.h"

using namespace std;
///\param id_ its a integer to represent a id from a article
///\param year_ its a short integer to represent a year that article has been publicated
///\param quote its a integer that represent number of citations from article
///\param title[MAX_SIZE_TITLE] its a char's array to represent a title from a  article
///\param autorList char array to represent a autor's list from a article
///\param snippet[MAX_SIZE_SNIPPED] char array to represent a resume from a article.
///\param date[MAX_SIZE] char array to represent the last update date from a article.
Article::Article(int id_, unsigned short int year_, int quote, char title[MAX_SIZE_TITLE] , char autorList[MAX_SIZE_AUTOR] , char snippet[MAX_SIZE_SNIPPET] , char date[ MAX_SIZE_DATE] ) {
    setID(id_);
    setYear(year_);
    setTitle(title);
    setQuotes(quote);
    setSnippet(snippet);
    setDate(date);
    setAutors(autorList);

}
Article::Article(){
//* Deafult constructor from class article*/
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

unsigned short Article::getYear(){
	return year;
}
unsigned int Article::getID(){
	return id;
}
int Article::getQuotes(){
	return quotes;
}

char* Article::getTitle(){
	return title;
}
char* Article::getAutors(){
	return autorList;
}
char* Article::getDate(){
	return date;
}

char* Article::getSnipped(){
	return snippet;
}
/**Function to return a string which contains all data from Article's object*/
std::string Article::toString(){
	string ret ;
	ostringstream str1;
    ostringstream st;
    ostringstream s;
    //These variables from class ostringstream are used to convert int to string =D
    str1 << getID();
	ret.append("\nID: ");
	ret.append(str1.str() );
	ret.append("\nTitle: ");
	ret.append(getTitle());
	ret.append("\nYear: ");
	st<< getYear();
	ret.append(st.str());
	ret.append("\nQuotes: ");
	s<< getQuotes();
	ret.append(s.str());
	ret.append("\nAutors: ");
	ret.append(getAutors());
	ret.append("\nSnipped: ");
	ret.append(getSnipped());
	ret.append("\nDate:");
	ret.append(getDate());
	return ret;
}



