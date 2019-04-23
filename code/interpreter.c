#include <string.h>
#include <stdlib.h>
#include "header.h"
#include <stdbool.h>

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

void interpret()
{
	while(linenumber<lines&&!endflag)
	{
		printf("interpret\n");
		if(nodes[linenumber]==NULL)
		{
			printf("nodes==NULL\n");
			break;
		}
		evaluate(nodes[linenumber]);
		linenumber++;
	}
}

void evaluate(struct astnode *node)
{
	switch(node->id)
	{
		case line:
			evaluate(node->left);
			break;
		
		case let:
			printf("let\n");
			vartable[varloc(node->left)]=exp_eval(node->right);
			break;
		
		case print:
			printf("print\n");
			printf("%c\n",exp_eval(node->left));
			break;
		
		case input:
			printf("input\n");
			vartable[varloc(node->left)]=getchar();
			break;
		
		case iftoken:
			printf("if\n");
			if(exp_eval(node->left))
			{
				evaluate(node->right);
			}
			break;
		
		case jump:
			printf("jump\n");
			linenumber=jumplocation[exp_eval(node->left)];
			linenumber--;
			break;
		
		case call:
			printf("call\n");
			callstack[sp++]=linenumber;
			linenumber=jumplocation[exp_eval(node->left)];
			linenumber--;
			break;
		
		case ret:
			printf("ret\n");
			linenumber=callstack[--sp];
			break;
		
		case end:
			printf("end\n");
			endflag=true;
			break;
	}
}

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

int varloc(struct astnode *node)
{
	char symbol=((struct numbernode*)node)->value;
	symbol=symbol-'A';
	return symbol;
}






