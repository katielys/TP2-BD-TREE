cmake_minimum_required(VERSION 3.5)
project(TP2-BD-TREE)

set(CMAKE_CXX_STANDARD 11)

add_executable(upload1 SecondIndexBtree.cpp SecondIndexBtree.h PrimaryIndexBtree.h PrimaryIndexBtree.cpp HashFile.h HashFile.cpp Article.h Article.cpp Block.h Block.cpp Parser.h Parser.cpp Parser.h upload.cpp)
add_executable(testable HashFile.h HashFile.cpp Article.h Article.cpp Block.h Block.cpp Parser.cpp Parser.h testable.cpp)
add_executable(findrec HashFile.h HashFile.cpp Article.h Article.cpp Block.h Block.cpp Parser.cpp Parser.h findrec.cpp)
add_executable(seek1 PrimaryIndexBtree.h PrimaryIndexBtree.cpp HashFile.h HashFile.cpp Article.h Article.cpp Block.h Block.cpp Parser.cpp Parser.h seek1.cpp)
add_executable(seek2 SecondIndexBtree.h SecondIndexBtree.cpp HashFile.h HashFile.cpp Article.cpp Article.h Block.cpp seek2.cpp )
