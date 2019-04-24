#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "header.h"

void getvalue(char c);
enum token lexp(void);
enum token lexg(void);
enum token lexi(void);
enum token lext(void);
enum token lexr(void);
enum token lexl(void);
enum token lexe(void);
void unread(char *str,int num);


/*The lex function tokenizes the input string.
Space and tab are ignored.
If a read letter is the start of a keyword it will check for the keyword.
Other letters are returned as variables.
Any punctuation and mathematical symbols are returned as is.
Numbers are returned with their value.
When the end of file is encountered the EOF flag will be set for the parser.*/

enum token lex(void)
{
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


//gets the numerical value of a string of numbers
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


/*The various lex* functions check for keywords.
If the string is a keyword that keyword token will be returned.
Otherwise the string will be unread an interpreted as a variable.*/

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


/*The unread function returns an string to the file buffer.
It returns all characters one by one starting at the last character.*/

void unread(char *str,int num)
{
	for(int i=num;i>=0;i--)
	{
		ungetc(str[i],file);
	}
}


/*The token test function both return a token.
If there is a token in the buffer that will be returned.
Otherwise the function wil get a new token.
The advance function also reduce the tokencount if the buffer was full*/

int tokencount;
int valuebuffer;
enum token tokenbuffer;

enum token test_input(void)
{
	if(tokencount==0)
	{
		tokencount++;
		tokenbuffer=lex();
		valuebuffer=tokenvalue.num;
	}
	else 
		tokenvalue.num=valuebuffer;
	return tokenbuffer;
}

enum token test_input_advance(void)
{
	if(tokencount==0)
	{
		tokenbuffer=lex();
		valuebuffer=tokenvalue.num;
	}
	else 
	{
		tokenvalue.num=valuebuffer;
		tokencount--;
	}
	return tokenbuffer;
}











