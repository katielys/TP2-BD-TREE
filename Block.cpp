///\file Block.cpp
#include "Article.h"
#include "Block.h"


Block::Block(){
/* Deafult constructor from class Block*/

}
//Article* Block::getArticle() {
//    return nullptr;
//
//}
void Block::insertArticleInTheBlock(Article article){
    //cout<<"INSERT";
    //this->article.setID(article.getID());
    this->valid = MASK_VALID;
    this->article = article;
}
bool Block::hasSpace()
{
    return true;
}

string Block::toString() {

    cout<<article.toString();
}


