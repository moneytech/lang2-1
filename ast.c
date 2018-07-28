#include "ast.h"
#include "list.h"
#include <stdlib.h>

Node *
new_node(AstType type, const void *value)
{
	Node *node = malloc(sizeof *node);
	node->type = type;
	node->value = value;
	return node;
}

Node *
new_assignment_statement(const char *id, Node *expression)
{
	AssignmentStatement *as = malloc(sizeof *as);
	as->id = id;
	as->expression = expression;
	return new_node(AST_ASSIGNMENT_STATEMENT, as);
}

Node *
new_block(List *statements)
{
	Block *b = malloc(sizeof *b);
	b->statements = statements;
	return new_node(AST_BLOCK, b);
}

Node *
new_boolean_expression(Node *left, const char *operator, Node *right)
{
	BooleanExpression *be = malloc(sizeof *be);
	be->left = left;
	be->operator = operator;
	be->right = right;
	return new_node(AST_BOOLEAN_EXPRESSION, be);
}

Node *
new_boolean_literal(int value)
{
	BooleanLiteral *bl = malloc(sizeof *bl);
	bl->value = value;
	return new_node(AST_BOOLEAN_LITERAL, bl);
}

Node *
new_break_statement(void)
{
	BreakStatement *bs = malloc(sizeof *bs);
	return new_node(AST_BREAK_STATEMENT, bs);
}

Node *
new_call_expression(const char *name, List *arguments)
{
	CallExpression *ce = malloc(sizeof *ce);
	ce->name = name;
	ce->arguments = arguments;
	return new_node(AST_CALL_EXPRESSION, ce);
}

Node *
new_continue_statement(void)
{
	ContinueStatement *cs = malloc(sizeof *cs);
	return new_node(AST_CONTINUE_STATEMENT, cs);
}

Node *
new_declaration_statement(const char *id, Node *expression)
{
	DeclarationStatement *ds = malloc(sizeof *ds);
	ds->id = id;
	ds->expression = expression;
	return new_node(AST_DECLARATION_STATEMENT, ds);
}

// ExpressionVisitor interface {
// 	String() string
// 	visitExpression(scope *variableScope) *expression
// }

Node *
new_function_statement(const char *name, List *parameters, Node *block)
{
	FunctionStatement *fs = malloc(sizeof *fs);
	fs->name = name;
	fs->parameters = parameters;
	fs->block = block;
	return new_node(AST_FUNCTION_STATEMENT, fs);
}

Node *
new_identifier(const char *value)
{
	Identifier *i = malloc(sizeof *i);
	i->value = value;
	return new_node(AST_IDENTIFIER, i);
}

Node *
new_if_statement(Node *booleanExpression, Node *block)
{
	IfStatement *is = malloc(sizeof *is);
	is->booleanExpression = booleanExpression;
	is->block = block;
	return new_node(AST_IF_STATEMENT, is);
}

Node *
new_logical_not_expression(Node *booleanExpression)
{
	LogicalNotExpression *lne = malloc(sizeof *lne);
	lne->booleanExpression = booleanExpression;
	return new_node(AST_LOGICAL_NOT_EXPRESSION, lne);
}

Node *
new_logical_operand(Node *left, const char *operator, Node *right)
{
	LogicalOperand *lo = malloc(sizeof *lo);
	lo->left = left;
	lo->operator = operator;
	lo->right = right;
	return new_node(AST_LOGICAL_OPERAND, lo);
}

Node *
new_number_literal(const char *value)
{
	NumberLiteral *nl = malloc(sizeof *nl);
	nl->value = value;
	return new_node(AST_NUMBER_LITERAL, nl);
}

Node *
new_print_statement(Node *expression)
{
	PrintStatement *ps = malloc(sizeof *ps);
	ps->expression = expression;
	return new_node(AST_PRINT_STATEMENT, ps);
}

Node *
new_return_statement(Node *expression)
{
	PrintStatement *ps = malloc(sizeof *ps);
	ps->expression = expression;
	return new_node(AST_RETURN_STATEMENT, ps);
}

// StatementVisitor interface {
// 	String() string
// 	visitStatement(scope *variableScope) *statement
// }

Node *
new_string_literal(const char *value)
{
	StringLiteral *sl = malloc(sizeof *sl);
	sl->value = value;
	return new_node(AST_STRING_LITERAL, sl);
}

Node *
new_term(Node *left, const char *operator, Node *right)
{
	Term *t = malloc(sizeof *t);
	t->left = left;
	t->operator = operator;
	t->right = right;
	return new_node(AST_TERM, t);
}

Node *
new_while_statement(Node *booleanExpression, Node *block)
{
	WhileStatement *ws = malloc(sizeof *ws);
	ws->booleanExpression = booleanExpression;
	ws->block = block;
	return new_node(AST_WHILE_STATEMENT, ws);
}
