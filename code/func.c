#include <string.h>
#include <stdlib.h>
#include "header.h"
#include <stdbool.h>

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
}

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
}

void  freenode(struct astnode *node)
{
	switch(node->id)
	{
		case '*':
		case '/':
		case '+':
		case '-':
		freenode(node->right);
		freenode(node->left);
		
		case var:
		case number:
		free(node);
	}
}