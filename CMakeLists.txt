cmake_minimum_required(VERSION 3.31)
project(Interpreter)

set(CMAKE_CXX_STANDARD 20)

add_executable(Interpreter main.cpp
        Classes/Tokenizer/Tokenizer.cpp
        Classes/Tokenizer/Tokenizer.h
        Classes/ShuntingYard/ShuntingYard.cpp
        Classes/ShuntingYard/ShuntingYard.h
        Structures/Token/Token.cpp
        Classes/classes.h
        Classes/Calculator/Calculator.cpp
        Classes/Calculator/Calculator.h
)
