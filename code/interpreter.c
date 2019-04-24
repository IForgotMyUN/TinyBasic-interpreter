#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "header.h"

void evaluate(struct astnode *node);
int exp_eval(struct astnode *node);
int varloc(struct astnode* node);

int linenumber=0;
int lines=0;
struct astnode **nodes;

int callstack[100];
int sp=0;

int vartable[26];

bool endflag=false;


/*The interpret functions interprets the entire array of AST's.
It will stop when:
	-the last line has been interpreted.
	-an end function has been executed.
	-an empty node has been encountered (fail state).
The lines are stored in the nodes array.*/

void interpret()
{
	while(linenumber<lines&&!endflag)
	{
		if(nodes[linenumber]==NULL)
		{
			printf("nodes==NULL\n");
			break;
		}
		evaluate(nodes[linenumber]);
		linenumber++;
	}
}


/*The evaluate function evaluates a non numerical expression.*/

void evaluate(struct astnode *node)
{
	switch(node->id)
	{
		case line:
			evaluate(node->left);
			break;
		
		case let:
			vartable[varloc(node->left)]=exp_eval(node->right);
			break;
		
		case print:
			printf("%c",exp_eval(node->left));
			break;
		
		case input:
			vartable[varloc(node->left)]=getchar();
			break;
		
		case iftoken:
			if(exp_eval(node->left))
			{
				evaluate(node->right);
			}
			break;
		
		case jump:
			linenumber=jumplocation[exp_eval(node->left)];
			linenumber--;
			break;
		
		case call:
			callstack[sp++]=linenumber;
			linenumber=jumplocation[exp_eval(node->left)];
			linenumber--;
			break;
		
		case ret:
			linenumber=callstack[--sp];
			break;
		
		case end:
			endflag=true;
			break;
	}
}


/*The expression evaluate function evaluates a numerical expression.
The function returns the integer value asssociated with the expression
at the time of evaluation.*/

int exp_eval(struct astnode *node)
{
	int value=0;
	switch(node->id)
	{
		case '+':
			value=exp_eval(node->left)+exp_eval(node->right);
			break;
		
		case '-':
			value=exp_eval(node->left)-exp_eval(node->right);
			break;
		
		case '*':
			value=exp_eval(node->left)*exp_eval(node->right);
			break;
		
		case '/':
			value=exp_eval(node->left)/exp_eval(node->right);
			break;
		
		case '<':
			value=exp_eval(node->left)<exp_eval(node->right);
			break;
		
		case '>':
			value=exp_eval(node->left)>exp_eval(node->right);
			break;
		
		case '=':
			value=exp_eval(node->left)==exp_eval(node->right);
			break;
		
		case var:
			value=vartable[varloc(node)];
			break;
		
		case number:
			value=((struct numbernode*)node)->value;
			break;
	}
	return value;
}


//varloc gives the index in the variable table based on a variable node.
int varloc(struct astnode *node)
{
	char symbol=((struct numbernode*)node)->value;
	symbol=symbol-'A';
	return symbol;
}






