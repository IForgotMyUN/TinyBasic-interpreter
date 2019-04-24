#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "header.h"

struct astnode *newnode(enum token token,struct astnode *left,struct astnode *right);
struct numbernode *newconstant(enum token token,int value);
void  freenode(struct astnode *node);

struct astnode *parse_line();
struct astnode *parse_statement();
struct astnode *parse_expression();
struct astnode *parse_term();
struct astnode *parse_factor();
struct astnode *parse_relexp();

bool eofflag=false;

/*The parsing is implemented as a recursive descent parser.
NULL is passed when either the parsing fails or an option isn't present.
NULL statements are not passed to the interpreter
The parsed grammar is represented in BNF in the grammar.bnf file*/


/*The parse function parses the entire file.
While the end of the file hasn't been reached each line will be parsed.
If the line is valid it will get added to an array of lines to later interpret.
If the line contains a linenumber then the line is stored in an array.
This array will later be used as a jumptable*/

void parse(void)
{
	eofflag=false;
	while(!eofflag)
	{
		struct astnode *node=parse_line();
		if(node)
		{
			if(parseflag)
			{
				printast(node);
				printf("\n");
			}
			nodes[linenumber]=node;
			if(node->right)
			{
				jumplocation[((struct numbernode *)(node->right))->value]=linenumber;
			}
			linenumber++;
		}
	}
	linenumber=0;
}


/*The parse line function parses one individual line.
If the line is empty a NULL pointer will be returned indicating no valid return.
If the line contains a line number this is parsed,
In any case it will check for a valid statement*/

struct astnode *parse_line(void)
{
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
			right=(struct astnode *)newconstant(number,tokenvalue.num);
			test_input_advance();
		}
		
		left=parse_statement();
		if(left&&(test_input()==eol||test_input()==EOF))
		{
			test_input_advance();
			return newnode(line,left,right);
		}
		else return NULL;
	}
}


/*The parse statement function will check for a valid statement.
The options are:
	-print: 		takes an expression to print.
	-if: 			takes a relative expression and a following statement.
	-goto: 			takes a number to jump to.
	-input: 		takes a variable which will be loaded with the input.
	-assignement:	takes a variable and an expression.
	-gosub:			takes a number to call.
	-return:		takes no arguments and returns to previous function.
	-end:			takes no arguments and ends the program.*/
	
struct astnode *parse_statement(void)
{
	
	struct astnode *left=NULL;
	struct astnode *right=NULL;
	enum token token=test_input_advance();

	switch(token)
	{
		case print:
			left=parse_expression();
			if(left)
			{
				return newnode(print,left,NULL);
			}
			else
			{
				fprintf(stderr,"expected number after expression\n");
				return NULL;
			}
		
			break;
			
		case iftoken:
			left=parse_relexp();
			if(left)
			{
				token=test_input_advance();
				if(token==then)
				{
					right=parse_statement();
					if(right)
					{
						return newnode(iftoken,left,right);
					}
					else return NULL;
				}
				else
				{
					fprintf(stderr,"expected then after if\n");
					return NULL;
				}
			}
			else 
			{
				fprintf(stderr,"expected comparison after if\n");
				return NULL;
			}
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
			
		case EOF:
			return NULL;
			break;
			
		default:
			fprintf(stderr,"expected statement at line\n");
			break;
	}
	return NULL;
}


/*The parse expression function parses a numerical expression.
An expression either contains: 
	-a factor.
	-a factor followed by an add sign followed by an other expression.
	-a factor followed by an subtract sign followed by an other expression.
Invalid expressions fail if there is no valid factor,
otherwise this left factor is returned instead.*/

struct astnode *parse_expression(void)
{
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
				return newnode(token,left,right);
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


/*The parse factor function parses a numerical factor.
A factor either contains: 
	-a term.
	-a term followed by multiply followed by an other factor.
	-a term followed by division followed by an other factor.
Invalid factors fail if there is no valid term,
otherwise this left term is returned instead.*/

struct astnode *parse_factor(void)
{
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
				return newnode(token,left,right);
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


/*The parse term function parses a numerical term.
An term either contains: 
	-a number.
	-a variable.
	-an expression between parentheses.
Any invalidities in the brackets results in a failure*/

struct astnode *parse_term(void)
{
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

/*The parse relative expression function parses a comparison.
An relative expression either contains: 
	-an expression followed by an equal sign followed by an other expression.
	-an expression followed by an larger sign followed by an other expression.
	-an expression followed by an smaller sign followed by an other expression.
Any invalidities in either expressionresults in a failure*/

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

















