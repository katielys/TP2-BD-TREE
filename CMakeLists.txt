cmake_minimum_required(VERSION 3.5)
project(TP2-BD-TREE)

set(CMAKE_CXX_STANDARD 11)

add_executable(TP2_BD upload.cpp Article.cpp Block.cpp HashFile.cpp HashFile.h findrec.cpp Parser.cpp Parser.h PrimaryIndexBtree.cpp PrimaryIndexBtree.h seek1.cpp )