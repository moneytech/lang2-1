#include "lex.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Token *lexer_consume(Lexer *lexer, const char *pattern, const char *token);
static Token *lexer_consume_string(Lexer *lexer);
static Token *new_token(Lexer *lexer, const char *symbol, const char *value);

Lexer *
new_lexer(const char *filepath)
{
	Lexer *lexer = malloc(sizeof *lexer);
	lexer->in = fopen(filepath, "r");
	lexer->out = stdout;
	lexer->line = 1;
	lexer->column = 1;
	lexer->prev = '\0';
	return lexer;
}

void
lexer_close(Lexer *lexer)
{
	fclose(lexer->in);
	fclose(lexer->out);
}

int
lexer_has_more(Lexer *lexer)
{
	int c = fgetc(lexer->in);
	if (c != EOF) {
		ungetc(c, lexer->in);
	}
	return c != EOF;
}

static Token *
new_token(Lexer *lexer, const char *symbol, const char *value)
{
	const char *lexdebug = getenv("LEXDEBUG");
	if (lexdebug && strcmp(lexdebug, "")) {
		printf("%s %d %d %s\n", symbol, lexer->line, lexer->column, value);
	}
	Token *token = malloc(sizeof *token);
	token->symbol = symbol;
	token->line = lexer->line;
	token->column = lexer->column;
	token->value = value;
	return token;
}

static Token *
lexer_consume(Lexer *lexer, const char *pattern, const char *symbol)
{
	char *value = malloc(256);
	long offset = ftell(lexer->in);
	int len;
	for (len = 0; pattern[len]; len++) {
		int c = fgetc(lexer->in);
		if (c != pattern[len] || c == EOF) {
			fseek(lexer->in, offset, SEEK_SET);
			return NULL;
		}
		value[len] = c;
	}
	if (value[0]) {
		Token *token = new_token(lexer, symbol, value);
		lexer->column += len;
		return token;
	}
	return NULL;
}

static Token *
lexer_consume_string(Lexer *lexer)
{
	char *value = malloc(8192);
	value[0] = '\0';
	char prev = '\0';
	int c, len;
	for (len = 0; lexer_has_more(lexer); len++) {
		c = fgetc(lexer->in);
		if (prev != '\\' && c == '"') {
			break;
		}
		prev = c;
		value[len] = c;
	}
	Token *token = new_token(lexer, "string", value);
	lexer->column += len;
	return token;
}

static Token *
lexer_consume_id(Lexer *lexer)
{
	char *value = malloc(256);
	value[0] = '\0';
	int c, len;
	for (len = 0; lexer_has_more(lexer); len++) {
		c = fgetc(lexer->in);
		if (!isalpha(c) && c != '_') {
			if (len == 0) {
				ungetc(c, lexer->in);
				return NULL;
			}
			if (!isdigit(c)) {
				break;
			}
		}
		value[len] = c;
	}
	ungetc(c, lexer->in);
	lexer->column += len;
	return new_token(lexer, "id", value);
}

static Token *
lexer_consume_number(Lexer *lexer)
{
	char *value = malloc(256);
	value[0] = '\0';
	int c, len;
	for (len = 0; lexer_has_more(lexer); len++) {
		c = fgetc(lexer->in);
		if (!isdigit(c)) {
			ungetc(c, lexer->in);
			break;
		}
		value[len] = c;
	}
	if (value[0]) {
		lexer->column += len;
		return new_token(lexer, "number", value);
	}
	return NULL;
}

Token *
lexer_lex(Lexer *lexer)
{
	Token *t = NULL;
	if (lexer_has_more(lexer)) {
		if (t == NULL) t = lexer_consume(lexer, "var", "var");
		if (t == NULL) t = lexer_consume(lexer, "print", "print");
		if (t == NULL) t = lexer_consume(lexer, "if", "if");
		if (t == NULL) t = lexer_consume(lexer, "while", "while");
		if (t == NULL) t = lexer_consume(lexer, "break", "break");
		if (t == NULL) t = lexer_consume(lexer, "continue", "continue");
		if (t == NULL) t = lexer_consume(lexer, "fn", "fn");
		if (t == NULL) t = lexer_consume(lexer, "return", "return");
		if (t == NULL) t = lexer_consume(lexer, "true", "true");
		if (t == NULL) t = lexer_consume(lexer, "false", "false");
		if (t == NULL) t = lexer_consume(lexer, "==", "==");
		if (t == NULL) t = lexer_consume(lexer, "!=", "!=");
		if (t == NULL) t = lexer_consume(lexer, ">=", ">=");
		if (t == NULL) t = lexer_consume(lexer, "<=", "<=");
		if (t == NULL) t = lexer_consume(lexer, "not", "not");
		if (t == NULL) t = lexer_consume(lexer, "and", "and");
		if (t == NULL) t = lexer_consume(lexer, "or", "or");
		if (t == NULL) t = lexer_consume_id(lexer);
		if (t == NULL) t = lexer_consume_number(lexer);
		if (t != NULL) return t;

		int ws = 0;
		int c = fgetc(lexer->in);
		while (strchr(" \t\r", c)) {
			if (c != '\r') {
				lexer->column++;
			}
			c = fgetc(lexer->in);
			ws = 1;
		}
		if (ws) {
			ungetc(c, lexer->in);
			return lexer_lex(lexer);
		}
		if (c == '\n') {
			lexer->line++;
			lexer->column = 1;
			return lexer_lex(lexer);
		}
		if (c == '"') {
			t = lexer_consume_string(lexer);
		} else if (c == ';') {
			t = new_token(lexer, ";", ";");
		} else if (strchr("=+-*/(){}<>,", c)) {
			char *s = malloc(2);
			s[0] = c;
			s[1] = '\0';
			t = new_token(lexer, s, s);
		} else {
			fprintf(stderr, "unrecognized char '%c' at line %d, column %d\n", c, lexer->line, lexer->column);
			exit(1);
		}
		lexer->column++;
		return t;
	}
	return new_token(lexer, "eof", "eof");
}
