cmake_minimum_required(VERSION 3.5)
project(TP2-BD-TREE)

set(CMAKE_CXX_STANDARD 11)

add_executable(upload PrimaryIndexBtree.h PrimaryIndexBtree.cpp HashFile.h HashFile.cpp Article.cpp Block.cpp upload.cpp Parser.cpp Parser.h)

add_executable(seek1 PrimaryIndexBtree.h PrimaryIndexBtree.cpp HashFile.h HashFile.cpp Article.cpp Block.cpp seek1.cpp Parser.cpp Parser.h)
