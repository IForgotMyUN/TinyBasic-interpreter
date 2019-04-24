#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "header.h"


/*The new node function generates a new node in the AST.
First new memory is allocated then the different parts are assigned*/

struct astnode *newnode(enum token token,struct astnode *left,struct astnode *right)
{
	struct astnode *node=malloc(sizeof(struct astnode));
	if(node==NULL)
	{
		printf("out of memory\n");
		exit(0);
	}
	node->id=token;
	node->left=left;
	node->right=right;
	return node;
}


/*The new node function generates a new terminal node with a value in the AST.
First new memory is allocated then the different parts are assigned*/

struct numbernode *newconstant(enum token token,int value)
{
	struct numbernode *node=malloc(sizeof(struct numbernode));
	if(node==NULL)
	{
		printf("out of memory\n");
		exit(0);
	}
	node->id=token;
	node->value=value;
	return node;
}


/*The free node function deallocates a node in the AST.
The freeing of the branches is performed based on the type of node.
The function is implemented as a switch statement with fallthrough.*/

void  freenode(struct astnode *node)
{
	switch(node->id)
	{
		case '*':
		case '/':
		case '+':
		case '-':
		case '<':
		case '=':
		case '>':
		case let:
		case iftoken:
			freenode(node->right);
			
		case line:
		case print:
		case input:
		case jump:
		case call:
			freenode(node->left);
		
		case ret:
		case end:
		case var:
		case number:
			free(node);
			break;
		
		default:
			printf("bad node %d during dealloc\n",node->id);
	}
}


/*The print token function prints a token.
If the flag is set it will also print a newline.*/

void printtoken(enum token token,int flag)
{
	switch(token)
	{
		case EOF:
			printf("EOF");
			break;
			
		case line:
			printf("line");
			break;
			
		case exp:
			printf("exp");
			break;
			
		case factor:
			printf("factor");
			break;
			
		case eol:
			printf("end of line");
			if(flag) 
				printf("\n");
			break;
			
		case number:
			printf("number: %d",tokenvalue.num);
			break;
			
		case var:
			printf("var: %c",tokenvalue.num);
			break;
			
		case print:
			printf("print");
			break;
			
		case let:
			printf("let");
			break;
		
		case ret:
			printf("ret");
			break;
		
		case end:
			printf("end");
			break;
		
		case iftoken:
			printf("if");
			break;
			
		case then:
			printf("then");
			break;
		
		case input:
			printf("input");
			break;
		
		case jump:
			printf("jump");
			break;
		
		case call:
			printf("call");
			break;
		
		case none:
		case empty:
			printf("empty");
			break;
			
		default:
			if(token<128)
				printf("%c",token);
			else
				printf("unknown token %d",token);
			break;
	}
	if(flag) 
		printf("\n");
}


/*The print AST function prints an AST as a lisp style list.
If a node is empty or NULL there will be no effect.
If a node is a variable or a number only it's value will be printed.
Otherwise the type token will be printed followed by the children.*/

void printast(struct astnode *node)
{
	if(node==NULL) return;
	printf("(");
	
	if(node->id!=var&&node->id!=number)
	{
		printtoken(node->id,0);
		printf(" ");
		printast(node->left);
		printf(" ");
		printast(node->right);
	}
	else if(node->id==var)
	{
		printf("var: %c",((struct numbernode*)node)->value);
	}
	else if(node->id==number)
	{
		printf("number: %d",((struct numbernode*)node)->value);
	}
	printf(")");
}





