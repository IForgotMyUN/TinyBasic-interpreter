#!/bin/bash
cd code || exit
gcc lexer.c main.c func.c parser.c interpreter.c -o../basic.exe
read -r -p "Press [Enter] key to start backup..."