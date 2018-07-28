#include "ast.h"
#include "lex.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Parser *
new_parser(Lexer *lexer)
{
	Parser *parser = malloc(sizeof *parser);
	parser->lexer = lexer;
	parser->token = lexer_lex(lexer);
	return parser;
}

int
parser_accept(Parser *p, const char *expected)
{
	return !strcmp(p->token->symbol, expected);
}

const char *
parser_expect(Parser *p, const char *expected)
{
	if (strcmp(p->token->symbol, expected)) {
		fprintf(stderr, "expected '%s', got '%s' at line %d, column %d\n", expected, p->token->symbol, p->token->line, p->token->column);
		exit(1);
	}
	const char *symbol = p->token->symbol;
	const char *value = p->token->value;
	p->token = lexer_lex(p->lexer);
	return value;
}

Node *
parser_block(Parser *p)
{
	List *statements = new_list();
	while (!parser_accept(p, "eof") && !parser_accept(p, "}")) {
		list_append(statements, parser_statement(p));
	}
	Block *block = malloc(sizeof *block);
	block->statements = statements;
	return new_node(AST_BLOCK, block);
}

Node *
parser_statement(Parser *p)
{
	if (parser_accept(p, "var")) {
		return parser_declaration(p);
	} else if (parser_accept(p, "print")) {
		return parser_print(p);
	} else if (parser_accept(p, "if")) {
		return parser_if_statement(p);
	} else if (parser_accept(p, "while")) {
		return parser_while_statement(p);
	} else if (parser_accept(p, "break")) {
		return parser_break_statement(p);
	} else if (parser_accept(p, "continue")) {
		return parser_continue_statement(p);
	} else if (parser_accept(p, "fn")) {
		return parser_function_statement(p);
	} else if (parser_accept(p, "return")) {
		return parser_return_statement(p);
	} else if (parser_accept(p, "id")) {
		Node *node = NULL;
		const char *id = parser_expect(p, "id");
		if (parser_accept(p, "=")) {
			node = parser_assignment(p, id);
		} else if (parser_accept(p, "(")) {
			node = parser_call_expression(p, id);
		}
		parser_expect(p, ";");
		return node;
	} else {
		parser_expect(p, "var|print|if|while|fn|return");
		return NULL;
	}
}

Node *
parser_declaration(Parser *p)
{
	parser_expect(p, "var");
	const char *id = parser_expect(p, "id");
	parser_expect(p, "=");
	Node *n = parser_boolean_expression(p);
	parser_expect(p, ";");
	return new_declaration_statement(id, n);
}

Node *
parser_print(Parser *p)
{
	parser_expect(p, "print");
	Node *expression = parser_boolean_expression(p);
	parser_expect(p, ";");
	PrintStatement *ps = malloc(sizeof *ps);
	ps->expression = expression;
	return new_node(AST_PRINT_STATEMENT, ps);
}

Node *
parser_if_statement(Parser *p)
{
	parser_expect(p, "if");
	Node *be = parser_boolean_expression(p);
	parser_expect(p, "{");
	Node *b = parser_block(p);
	parser_expect(p, "}");
	return new_if_statement(be, b);
}

Node *
parser_while_statement(Parser *p)
{
	parser_expect(p, "while");
	Node *be = parser_boolean_expression(p);
	parser_expect(p, "{");
	Node *b = parser_block(p);
	parser_expect(p, "}");
	return new_while_statement(be, b);
}

Node *
parser_break_statement(Parser *p)
{
	parser_expect(p, "break");
	parser_expect(p, ";");
	return new_break_statement();
}

Node *
parser_continue_statement(Parser *p)
{
	parser_expect(p, "continue");
	parser_expect(p, ";");
	return new_continue_statement();
}

Node *
parser_function_statement(Parser *p)
{
	List *parameters = new_list();
	parser_expect(p, "fn");
	const char *name = parser_expect(p, "id");
	parser_expect(p, "(");
	if (parser_accept(p, "id")) {
		list_append(parameters, parser_expect(p, "id"));
		while (1) {
			if (!parser_accept(p, ",")) {
				break;
			}
			parser_expect(p, ",");
			list_append(parameters, parser_expect(p, "id"));
		}
	}
	parser_expect(p, ")");
	parser_expect(p, "{");
	Node *block = parser_block(p);
	parser_expect(p, "}");
	return new_function_statement(name, parameters, block);
}

Node *
parser_return_statement(Parser *p)
{
	parser_expect(p, "return");
	if (parser_accept(p, ";")) {
		parser_expect(p, ";");
		return new_return_statement(NULL);
	}
	Node *be = parser_boolean_expression(p);
	parser_expect(p, ";");
	return new_return_statement(be);
}

Node *
parser_assignment(Parser *p, const char *id)
{
	parser_expect(p, "=");
	return new_assignment_statement(id, parser_boolean_expression(p));
}

Node *
parser_call_expression(Parser *p, const char *id)
{
	List *arguments = new_list();
	parser_expect(p, "(");
	while (1) {
		if (parser_accept(p, ")")) {
			break;
		}
		list_append(arguments, parser_boolean_expression(p));
		if (!parser_accept(p, ")")) {
			parser_expect(p, ",");
		}
	}
	parser_expect(p, ")");
	return new_call_expression(id, arguments);
}

Node *
parser_boolean_expression(Parser *p)
{
	Node *b = parser_and_expression(p);
	while (1) {
		if (parser_accept(p, "or")) {
			parser_expect(p, "or");
			b = new_boolean_expression(b, "or", parser_and_expression(p));
		} else {
			return b;
		}
	}
}

Node *
parser_and_expression(Parser *p)
{
	Node *b = parser_condition(p);
	while (1) {
		if (parser_accept(p, "and")) {
			parser_expect(p, "and");
			b = new_boolean_expression(b, "and", parser_condition(p));
		} else {
			return b;
		}
	}
}

Node *
parser_condition(Parser *p)
{
	const char *operator;
	Node *left = parser_logical_operand(p);
	if (parser_accept(p, "==")) {
		operator = parser_expect(p, "==");
	} else if (parser_accept(p, "!=")) {
		operator = parser_expect(p, "!=");
	} else if (parser_accept(p, ">=")) {
		operator = parser_expect(p, ">=");
	} else if (parser_accept(p, ">")) {
		operator = parser_expect(p, ">");
	} else if (parser_accept(p, "<")) {
		operator = parser_expect(p, "<");
	} else if (parser_accept(p, "<=")) {
		operator = parser_expect(p, "<=");
	} else {
		return left;
	}
	return new_boolean_expression(left, operator, parser_logical_operand(p));
}

Node *
parser_logical_operand(Parser *p)
{
	Node *t = parser_term(p);
	while (1) {
		if (parser_accept(p, "+")) {
			parser_expect(p, "+");
			t = new_logical_operand(t, "+", parser_term(p));
		} else if (parser_accept(p, "-")) {
			parser_expect(p, "-");
			t = new_logical_operand(t, "-", parser_term(p));
		} else {
			return t;
		}
	}
}

Node *
parser_term(Parser *p)
{
	Node *t = parser_logical_not_expression(p);
	while (1) {
		if (parser_accept(p, "*")) {
			parser_expect(p, "*");
			t = new_term(t, "*", parser_logical_not_expression(p));
		} else if (parser_accept(p, "/")) {
			parser_expect(p, "/");
			t = new_term(t, "/", parser_logical_not_expression(p));
		} else {
			return t;
		}
	}
}

Node *
parser_logical_not_expression(Parser *p)
{
	if (parser_accept(p, "not")) {
		parser_expect(p, "not");
		return new_logical_not_expression(parser_logical_not_expression(p));
	}
	return parser_atom(p);
}

Node *
parser_atom(Parser *p)
{
	if (parser_accept(p, "id")) {
		const char *id = parser_expect(p, "id");
		if (parser_accept(p, "(")) {
			return parser_call_expression(p, id);
		}
		return new_identifier(id);
	} else if (parser_accept(p, "number")) {
		return new_number_literal(parser_expect(p, "number"));
	} else if (parser_accept(p, "string")) {
		return new_string_literal(parser_expect(p, "string"));
	} else if (parser_accept(p, "true")) {
		parser_expect(p, "true");
		return new_boolean_literal(1);
	} else if (parser_accept(p, "false")) {
		parser_expect(p, "false");
		return new_boolean_literal(0);
	} else if (parser_accept(p, "(")) {
		parser_expect(p, "(");
		Node *be = parser_boolean_expression(p);
		parser_expect(p, ")");
		return be;
	} else {
		parser_expect(p, "id|number|string|true|false");
		return NULL;
	}
}
