cmake_minimum_required(VERSION 3.17)
project(hm4_error)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES
        						//*号对应具体的文件名

        )

add_executable(ChainList main.cpp lexical_analysis.h lexical_analysis.cpp grammer_analysis.h grammer_analysis.cpp)