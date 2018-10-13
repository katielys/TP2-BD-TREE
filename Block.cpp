///\file Block.cpp
#include "Article.h"
#include "Block.h"


Block::Block(){
/* Deafult constructor from class Block*/

}

void Block::insertArticleInTheBlock(Article article){

    valid = MASK_VALID;
    this->article = article;
}
bool Block::hasSpace()
{
    return true;
}

string Block::toString() {

    cout<<article.toString();
}


