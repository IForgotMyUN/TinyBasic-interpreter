#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "header.h"

void testarg(const char *str);

bool parseflag=false;
bool lexflag=false;

/*This interpreter first parses the entire file.
After parsing it will interpret the generated array of AST's.*/


int main(int argc, char *argv[]) 
{  
	/*Handle the various arguments. The last argument is the file to open.*/
	if(argc>1)
	{
		
		for(int i=argc-1;i>0;i--)
		{
			testarg(argv[i]);
		}
		if((file = fopen(argv[argc-1],"r")) == NULL)
		{
			perror(argv[argc-1]);
			exit(argc-1);
		}
	}
	else 
	{
		printf("Please specify a file or use flag -h for help\n");
		exit(0);
	}
	
	/*Check the amount of lines in the file. 
	The number of lines is used to allocate enough memory for the
	AST array.*/
	
	char c=getc(file);
	while(c!=EOF)
	{
		if(c=='\n')
			lines++;
		c=getc(file);
	}
	nodes=malloc(sizeof(struct astnode*)*lines);
	if(nodes==NULL)
	{
		printf("out of memory\n");
		exit(0);
	}
	
	//If the lexflag is set print the entire lexed file.
	if(lexflag)
	{
		rewind(file);
		printf("\nlexing:\n\n");
		//printf("%d\n",lex());
		enum token curtoken=lex();
		while(curtoken!=EOF)
		{
			printtoken(curtoken,1);
			curtoken=lex();
		}
	}
	
	//Rewind file then parse the file.
	rewind(file);
	if(parseflag)
		printf("\nparsing:\n\n");
	parse();
	
	//start interpreting the file.
	printf("\ninterpret:\n\n");
	interpret();
	
	return 0;
}


/*The test argument function parses a command line argument.
It will enable printing during lexing and parsing
or will show a help page.
Unrecognized arguments are discarded.*/
void testarg(const char *str)
{
	if(!strcmp(str,"-l"))
		lexflag=true;
	
	else if(!strcmp(str,"-p"))
		parseflag=true;
	
	else if(!strcmp(str,"-h"))
	{
		printf("This basic interpreter interprets a type of Tiny Basic\n\n");
		printf("Usage: basic [options] file\n");
		printf("Options:\n");
		printf(" -l\t print all tokens in porgram before parsing\n");
		printf(" -p\t print a LISP-style tree during parsing\n");
		printf(" -h\t display this help page\n\n");
		exit(0);
	}
	return;
}