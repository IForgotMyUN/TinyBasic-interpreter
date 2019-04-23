#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "header.h"

void testarg(const char *str);

int main(int argc, char *argv[]) 
{  
	if(argc>1)
	{
		if((file = fopen(argv[argc-1],"r")) == NULL)
		{
			perror(argv[argc-1]);
			exit(argc-1);
		}
		if(argc>2)
		{
			for(int i=argc-1;--i;i>0)
			{
				printf("testing argument %s\n",argv[i]);
				testarg(argv[i]);
			}
		}
	}
	else {printf("please specify a fle");exit(0);}
	
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
	
	rewind(file);
	printf("\nlexing:\n\n");
	//printf("%d\n",lex());
	enum token curtoken=lex();
	while(curtoken!=EOF)
	{
		printtoken(curtoken,1);
		curtoken=lex();
	}
	
	rewind(file);
	printf("\nparsing:\n\n");
	parse();
	
	printf("\ninterpret:\n\n");
	interpret();
	
	return 0;
}

void testarg(const char *str)
{
	return;
}