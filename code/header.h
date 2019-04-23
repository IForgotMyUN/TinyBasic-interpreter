#include <stdio.h>

extern _Bool eofflag;

union tokendata
{
	int num;
}tokenvalue;

enum token
{
	number=128,
	var,
	print,
	let,
	ret,
	end,
	iftoken,
	then,
	input,
	jump,
	call,
	eol,
	none,
	empty,
	line=256,
	exp,
	factor,
	term,
	relexp,
};

struct astnode
{
	enum token id;
	struct astnode *left;
	struct astnode *right;
};

struct numbernode
{
	enum token id;
	int value;
};


FILE *file;


void parse(void);

struct astnode *newnode(enum token token,struct astnode *left,struct astnode *right);
struct numbernode *newconstant(enum token token,int value);
void  freenode(struct astnode *node);

enum token lex(void);
void unread(char *str,int num);
void printtoken(enum token token);

enum token test_input(void);
enum token test_input_advance(void);

