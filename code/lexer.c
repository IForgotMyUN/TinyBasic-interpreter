#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "header.h"
#include <ctype.h>

void getvalue(char c);
enum token lexp(void);
enum token lexg(void);
enum token lexi(void);
enum token lext(void);
enum token lexr(void);
enum token lexl(void);
enum token lexe(void);

enum token lex(void)
{
	//printf("start lex\n");
	enum token token;
	char c;
	do
	{
		c=fgetc(file);
	}while(c==' '||c=='\t');
	if(c==EOF)
	{
		eofflag=true;
		token=EOF;
	}
	else if(isdigit(c))
	{
		//printf("digit\n");
		token=number;
		getvalue(c);
	}
	else switch(c)
	{
		case '*':
		case '/':
		case '+':
		case '-':
		case '(':
		case ')':
		case '=':
		case '>':
		case '<':
			token=c;
			break;
			
		case '\n':
			token=eol;
			break;
		
		case 'P':
			token=lexp();
			break;
		case 'G':
			token=lexg();
			break;
		case 'I':
			token=lexi();
			break;
		case 'T':
			token=lext();
			break;
		case 'R':
			token=lexr();
			break;
		case 'L':
			token=lexl();
			break;
		case 'E':
			token=lexe();
			break;
		default:
			if(c>='A'&&c<='Z')
			{
				token=var;
				tokenvalue.num=c;
			}
			else token=none;
	}
	return token;
}

void getvalue(char c)
{
	int temp=c-'0';
	while(isdigit(c=fgetc(file)))
	{
		temp*=10;
		temp+=c-'0';
	}
	ungetc(c,file);
	tokenvalue.num=temp;
}

enum token lexp(void)
{
	enum token token=none;
	char str[5]="RINT";
	char readc[5];
	for(int i=0;i<4;i++)
	{
		readc[i]=fgetc(file);
		if(str[i]!=readc[i])
		{
			unread(readc,i);
			token=var;
			tokenvalue.num='P';
			break;
		}
	}
	if(token==none)
	{
		token=print;
	}
	return token;
}

enum token lexl(void)
{
	enum token token=none;
	char str[3]="ET";
	char readc[3];
	for(int i=0;i<2;i++)
	{
		readc[i]=fgetc(file);
		if(str[i]!=readc[i])
		{
			unread(readc,i);
			token=var;
			tokenvalue.num='L';
			break;
		}
	}
	if(token==none)
	{
		token=let;
	}
	return token;
}

enum token lexr(void)
{
	enum token token=none;
	char str[6]="ETURN";
	char readc[6];
	for(int i=0;i<5;i++)
	{
		readc[i]=fgetc(file);
		if(str[i]!=readc[i])
		{
			unread(readc,i);
			token=var;
			tokenvalue.num='R';
			break;
		}
	}
	if(token==none)
	{
		token=ret;
	}
	return token;
}

enum token lexe(void)
{
	enum token token=none;
	char str[3]="ND";
	char readc[3];
	for(int i=0;i<2;i++)
	{
		readc[i]=fgetc(file);
		if(str[i]!=readc[i])
		{
			unread(readc,i);
			token=var;
			tokenvalue.num='E';
			break;
		}
	}
	if(token==none)
	{
		token=end;
	}
	return token;
}

enum token lext(void)
{
	enum token token=none;
	char str[4]="HEN";
	char readc[4];
	for(int i=0;i<3;i++)
	{
		readc[i]=fgetc(file);
		if(str[i]!=readc[i])
		{
			unread(readc,i);
			token=var;
			tokenvalue.num='T';
			break;
		}
	}
	if(token==none)
	{
		token=then;
	}
	return token;
}

enum token lexi(void)
{
	enum token token=none;
	char str[5]="NPUT";
	char readc[5];
	readc[0]=fgetc(file);
	if(readc[0]=='F')
	{
		token=iftoken;
	}
	else
	{
		ungetc(readc[0],file);
		for(int i=0;i<4;i++)
		{
			readc[i]=fgetc(file);
			if(str[i]!=readc[i])
			{
				unread(readc,i);
				token=var;
				tokenvalue.num='I';
				break;
			}
		}
		if(token==none)
		{
			token=input;
		}
	}
	return token;
}

bool checkgoto(void)
{
	bool flag=true;
	char str[4]="OTO";
	char readc[4];
	for(int i=0;i<3;i++)
	{
		readc[i]=fgetc(file);
		if(str[i]!=readc[i])
		{
			unread(readc,i);
			flag=false;
			break;
		}
	}
	return flag;
}

bool checkgosub(void)
{
	bool flag=true;
	char str[5]="OSUB";
	char readc[5];
	for(int i=0;i<4;i++)
	{
		readc[i]=fgetc(file);
		if(str[i]!=readc[i])
		{
			unread(readc,i);
			flag=false;
			break;
		}
	}
	return flag;
}

enum token lexg(void)
{
	enum token token;
	if(checkgoto())
	{
		token=jump;
	}
	else if(checkgosub())
	{
		token=call;
	}
	else
	{
		token=var;
		tokenvalue.num='G';
	}
	
	return token;
}

void printtoken(enum token token)
{
	switch(token)
	{
		case eol:
			printf("end of line\n\n");
			break;
		case number:
			printf("number: %d\n",tokenvalue.num);
			break;
			
		case var:
			printf("var: %c\n",tokenvalue.num);
			break;
			
		case print:
			printf("print\n");
			break;
			
		case let:
			printf("let\n");
			break;
		
		case ret:
			printf("ret\n");
			break;
		
		case end:
			printf("end\n");
			break;
		
		case iftoken:
			printf("if\n");
			break;
			
		case then:
			printf("then\n");
			break;
		
		case input:
			printf("input\n");
			break;
		
		case jump:
			printf("jump\n");
			break;
		
		case call:
			printf("call\n");
			break;
		
		case none:
		case empty:
			printf("empty\n");
			break;
			
		default:
			printf("%c\n",token);
			break;
	}
}

void unread(char *str,int num)
{
	for(int i=num;i>=0;i--)
	{
		ungetc(str[i],file);
	}
}

int tokencount;
enum token tokenbuffer;

enum token test_input(void)
{
	//printf("testing input\n");
	if(tokencount==0)
	{
		tokencount++;
		tokenbuffer=lex();
	}
	return tokenbuffer;
}

enum token test_input_advance(void)
{
	//printf("testing input with step\n");
	if(tokencount==0)
	{
		tokenbuffer=lex();
	}
	else 
		tokencount--;
	return tokenbuffer;
}











