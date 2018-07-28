#ifndef PARSE_H
#define PARSE_H 1
#include "ast.h"
#include "lex.h"
typedef struct {
	Lexer *lexer;
	Token *token;
} Parser;
Parser *new_parser(Lexer *lexer);
int parser_accept(Parser *p, const char *expected);
Node *parser_and_expression(Parser *p);
Node *parser_assignment(Parser *p, const char *id);
Node *parser_atom(Parser *p);
Node *parser_block(Parser *p);
Node *parser_boolean_expression(Parser *p);
Node *parser_break_statement(Parser *p);
Node *parser_call_expression(Parser *p, const char *id);
Node *parser_condition(Parser *p);
Node *parser_continue_statement(Parser *p);
Node *parser_declaration(Parser *p);
const char *parser_expect(Parser *p, const char *expected);
Node *parser_function_statement(Parser *p);
Node *parser_if_statement(Parser *p);
Node *parser_logical_not_expression(Parser *p);
Node *parser_logical_operand(Parser *p);
Node *parser_print(Parser *p);
Node *parser_return_statement(Parser *p);
Node *parser_statement(Parser *p);
Node *parser_term(Parser *p);
Node *parser_while_statement(Parser *p);
#endif
