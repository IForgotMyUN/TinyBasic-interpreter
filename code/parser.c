#include <string.h>
#include <stdlib.h>
#include "header.h"
#include <stdbool.h>

struct astnode *parse_line();
struct astnode *parse_statement();
struct astnode *parse_expression();
struct astnode *parse_term();
struct astnode *parse_factor();
struct astnode *parse_relexp();

bool eofflag=false;

void parse(void)
{
	eofflag=false;
	while(!eofflag)
	{
		struct astnode *node=parse_line();
		printast(node);
		freenode(node);
	}
}

struct astnode *parse_line(void)
{
	printf("parsing line\n");
	struct astnode *left=NULL;
	struct astnode *right=NULL;
	enum token token=test_input();
	if(token==eol)
	{
		test_input_advance();
		return NULL;
	}
	else
	{
		if(test_input()==number)
		{
			test_input_advance();
			right=(struct astnode *)newconstant(number,tokenvalue.num);
		}
		left=parse_statement();
		if(left&&test_input_advance()==eol)
		{
			return newnode(line,left,right);
		}
		else return NULL;
	}
}

struct astnode *parse_statement(void)
{
	
	struct astnode *left=NULL;
	struct astnode *right=NULL;
	enum token token=test_input_advance();
	printf("parsing statement with token:");
	printtoken(token,1);
	switch(token)
	{
		case print:
			left=parse_expression();
			if(left)
			{
				return left;
			}
			else
			{
				fprintf(stderr,"expected number after expression\n");
				return NULL;
			}
		
			break;
			
		case iftoken:
			fprintf(stderr,"if not implemented\n");
			return NULL;
			break;
			
		case jump:
			if(test_input_advance()==number)
			{
				left=(struct astnode *)newconstant(number,tokenvalue.num);
				return newnode(jump,left,NULL);
			}
			else
			{
				fprintf(stderr,"expected number after goto\n");
				return NULL;
			}
			break;
			
		case input:
			if(test_input_advance()==var)
			{
				left=(struct astnode *)newconstant(var,tokenvalue.num);
				return newnode(input,left,NULL);
			}
			else
			{
				fprintf(stderr,"expected variable after input\n");
				return NULL;
			}
			break;
			
		case let:
			if(test_input_advance()==var)
			{
				left=(struct astnode *)newconstant(var,tokenvalue.num);
				if(test_input_advance()=='=')
				{
					right=parse_expression();
					if(right)
					{
						return newnode(let,left,right);
					}
					else 
					{
						fprintf(stderr,"expected expression after '='\n");
						return NULL;
					}
				}
				else
				{
					fprintf(stderr,"expected '='\n");
					return NULL;
				}
			}
			else 
			{
				fprintf(stderr,"expected variable after let\n");
				return NULL;
			}
			break;
			
		case call:
			if(test_input_advance()==number)
			{
				left=(struct astnode *)newconstant(number,tokenvalue.num);
				return newnode(call,left,NULL);
			}
			else 
			{
				fprintf(stderr,"expected number after gosub\n");
				return NULL;
			}
			break;
			
		case ret:
			return newnode(ret,NULL,NULL);
			break;
			
		case end:
			return newnode(end,NULL,NULL);
			break;
			
		default:
			fprintf(stderr,"expected statement at line\n");
			break;
	}
	return NULL;
}

struct astnode *parse_expression(void)
{
	printf("parsing expression\n");
	struct astnode *left=NULL;
	struct astnode *right=NULL;
	enum token token;
	
	left=parse_factor();
	if(left)
	{
		token=test_input();
		if(token=='+'||token=='-')
		{
			test_input_advance();
			right=parse_expression();
			if(right)
			{
				return newnode(exp,left,right);
			}
			else 
			{
				return left;
			}
		}
		else return left;
	}
	return NULL;
}

struct astnode *parse_factor(void)
{
	printf("parsing factor\n");
	struct astnode *left=NULL;
	struct astnode *right=NULL;
	enum token token;
	
	left=parse_term();
	if(left)
	{
		token=test_input();
		if(token=='*'||token=='/')
		{
			test_input_advance();
			right=parse_factor();
			if(right)
			{
				return newnode(factor,left,right);
			}
			else 
			{
				return left;
			}
		}
		else return left;
	}
	return NULL;
}

struct astnode *parse_term(void)
{
	printf("parsing term\n");
	struct astnode *node;
	enum token token;
	
	token=test_input_advance();
	if(token==var||token==number)
	{
		return (struct astnode *)newconstant(token,tokenvalue.num);
	}
	else if (token=='(')
	{
		node=parse_expression();
		if(node)
		{
			if(test_input()==')')
			{
				test_input_advance();
				return node;
			}
			else 
			{
				freenode(node);
				return NULL;
			}
		}
		else return NULL;
	}
	else return NULL;
}


struct astnode *parse_relexp()
{
	struct astnode *left=NULL;
	struct astnode *right=NULL;
	enum token token;
	
	left=parse_expression();
	if(left)
	{
		token=test_input_advance();
		if(token=='<'||token=='='||token=='>')
		{
			right=parse_expression();
			if(right)
			{
				return newnode(token,left,right);
			}
			else 
			{
				freenode(left);
				return NULL;
			}
		}
		else 
		{
			freenode(left);
			return NULL;
		}
	}
	else return NULL;
}

















