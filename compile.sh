#!/bin/bash
clear
clear
rm mgr
g++ src/*cpp -o mgr -pthread -std=c++11 -lm -lSDL -lGL -lGLEW -Wall
