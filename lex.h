#ifndef LEX_H
#define LEX_H 1
#include <stdio.h>
typedef struct {
	const char *symbol;
	int line;
	int column;
	const char *value;
} Token;
typedef struct {
	FILE *in, *out;
	int line, column, prev, prevcolumn;
} Lexer;
Lexer *new_lexer(const char *filepath);
void lexer_close(Lexer *lexer);
int lexer_has_more(Lexer *lexer);
Token *lexer_lex(Lexer *lexer);
#endif
