#!/bin/bash
flex main/lexical.l
bison -d main/synt.y
gcc -g lex.yy.c synt.tab.c include/*.c -ly -lfl -o compiler
