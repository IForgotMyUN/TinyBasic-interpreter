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

extern _Bool parseflag;
extern _Bool lexflag;
extern int linenumber;
extern int lines;
extern struct astnode **nodes;
int jumplocation[9999];
FILE *file;

void interpret();

void parse(void);

enum token lex(void);
enum token test_input(void);
enum token test_input_advance(void);

void printast(struct astnode *node);
void printtoken(enum token token,int flag);


