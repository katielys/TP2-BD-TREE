///\file Block.h header from Block
#include "Article.h"
#ifndef TP2_BD_TREE_BLOCK_H
#define TP2_BD_TREE_BLOCK_H

using namespace std;

#define MASK_VALID 6666666666666 //!< MASK_VALID is a flag to know if a block is valid or not
#define RECORD_SIZE 4096 //!< SIZE FROM A BLOCk

class Block {
    public:
      Block();
      bool hasSpace();
      void insertArticleInTheBlock(Article article);
      Article article;
      long int valid;
      string toString();

   // private:
    // *Article getArticle();

};


#endif //TP2_BD_TREE_BLOCK_H
