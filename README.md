# basic interpreter
This is a simple basic interpreter with comments for anyone who is interested how an interpreter works.<br>
I have chosen to generate a simplified version of Tiny Basic.<br>
The package includes:
  * The source code for the interpreter (hopefully clearly commented)
  * The grammar of this implementation in BNF
  * A few example programs in BASIC
  * A batch script to compile the program
  * A bash script (same function; hopefully working, but I can't test it)

If the script doesn't work one can try:
```
gcc main.c func.c lexer.c parser.c interpreter.c -obasic.exe
```
