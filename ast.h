#ifndef AST_H
#define AST_H 1
#include "list.h"

typedef enum ast_type {
	AST_ASSIGNMENT_STATEMENT,
	AST_BLOCK,
	AST_BOOLEAN_EXPRESSION,
	AST_BOOLEAN_LITERAL,
	AST_BREAK_STATEMENT,
	AST_CALL_EXPRESSION,
	AST_CONTINUE_STATEMENT,
	AST_DECLARATION_STATEMENT,
	AST_FUNCTION_STATEMENT,
	AST_IDENTIFIER,
	AST_IF_STATEMENT,
	AST_LOGICAL_NOT_EXPRESSION,
	AST_LOGICAL_OPERAND,
	AST_NUMBER_LITERAL,
	AST_PRINT_STATEMENT,
	AST_RETURN_STATEMENT,
	AST_STRING_LITERAL,
	AST_TERM,
	AST_WHILE_STATEMENT
} AstType;

typedef struct {
	AstType type;
	const void *value;
} Node;
Node *new_node(AstType type, const void *value);

typedef struct assignment_statement {
	const char *id;
	Node *expression;
} AssignmentStatement;
Node *new_assignment_statement(const char *id, Node *expression);

typedef struct block {
	List *statements;
} Block;
Node *new_block(List *statements);

typedef struct boolean_expression {
	Node *left;
	const char *operator;
	Node *right;
} BooleanExpression;
Node *new_boolean_expression(Node *left, const char *operator, Node *right);

typedef struct boolean_literal {
	int value;
} BooleanLiteral;
Node *new_boolean_literal(int value);

typedef struct break_statement {
} BreakStatement;
Node *new_break_statement(void);

typedef struct call_expression {
	const char *name;
	List *arguments;
} CallExpression;
Node *new_call_expression(const char *name, List *arguments);

typedef struct continue_statement {
} ContinueStatement;
Node *new_continue_statement(void);

typedef struct declaration_statement {
	const char *id;
	Node *expression;
} DeclarationStatement;
Node *new_declaration_statement(const char *id, Node *expression);

typedef struct function_statement {
	const char *name;
	List *parameters;
	Node *block;
} FunctionStatement;
Node *new_function_statement(const char *name, List *parameters, Node *block);

typedef struct identifier {
	const char *value;
} Identifier;
Node *new_identifier(const char *value);

typedef struct if_statement {
	Node *booleanExpression;
	Node *block;
} IfStatement;
Node *new_if_statement(Node *booleanExpression, Node *block);

typedef struct logical_not_expression {
	Node *booleanExpression;
} LogicalNotExpression;
Node *new_logical_not_expression(Node *booleanExpression);

typedef struct logical_operand {
	Node *left;
	const char *operator;
	Node *right;
} LogicalOperand;
Node *new_logical_operand(Node *left, const char *operator, Node *right);

typedef struct number_literal {
	const char *value;
} NumberLiteral;
Node *new_number_literal(const char *value);

typedef struct print_statement {
	Node *expression;
} PrintStatement;
Node *new_print_statement(Node *expression);

typedef struct return_statement {
	Node *expression;
} ReturnStatement;
Node *new_return_statement(Node *expression);

typedef struct string_literal {
	const char *value;
} StringLiteral;
Node *new_string_literal(const char *value);

typedef struct term {
	Node *left;
	const char *operator;
	Node *right;
} Term;
Node *new_term(Node *left, const char *operator, Node *right);

typedef struct while_statement {
	Node *booleanExpression;
	Node *block;
} WhileStatement;
Node *new_while_statement(Node *booleanExpression, Node *block);
#endif
