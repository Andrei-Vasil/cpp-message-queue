#!/bin/sh

echo "compiling..."
g++ -Wall src/main.cpp -o run

echo "give permissions..."
chmod 755 run 

echo "run!"
./run

echo "finish!"
