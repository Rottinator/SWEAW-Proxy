#!/bin/bash

cd ..

rm -rf ./output/

mkdir output

g++ -Wno-write-strings -pthread main.cpp -o "output/SWEAWProxy-Server"

cd output

chmod +x "SWEAWProxy-Server"