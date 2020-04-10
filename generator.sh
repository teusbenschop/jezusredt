#!/bin/bash


# Compile and run the website generator.
g++ -Wall -Wextra -pedantic -std=c++11 pugixml.cpp generator.cpp -o generator
EXIT_CODE=$?
if [ $EXIT_CODE != 0 ]; then
exit
fi
./generator `realpath contents` `realpath app/src/main/assets`
EXIT_CODE=$?
if [ $EXIT_CODE != 0 ]; then
exit
fi
rm generator

