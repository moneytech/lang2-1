#include "ast.h"
#include "string.h"
#include <stdlib.h>

const char *
str_list_str(List *list)
{
	const char *s = NULL;
	int len;
	ListNode *n;
	for (n = list->head, len = 0; n; n = n->next, len++) {
		if (len != 0) {
			s = sprintf_alloc("%s%s", s, " ");
		}
		s = sprintf_alloc("%s%s", s, node_str(n->value));
	}
	return len != 0 ? s : sprintf_alloc("%s", "nil");
}

const char *
node_str(Node *node)
{
	switch (node->type) {
	case AST_ASSIGNMENT_STATEMENT:
		AssignmentStatement *as = (AssignmentStatement *)node->value;
		sprintf_alloc("(assignment %s %s)", as->id, node_str(as->expression));
	case AST_BLOCK:
		Block *b = (Block *)node->value;
		return sprintf_alloc("(block %s)", str_list_str(b->statements));
	case AST_BOOLEAN_EXPRESSION:
		BooleanExpression *be = (BooleanExpression *)node->value;
		return sprintf_alloc("(bool %s %s %s)", node_str(be->left), be->operator, node_str(be->right));
	case AST_BOOLEAN_LITERAL:
		BooleanLiteral *bl = (BooleanLiteral *)node->value;
		return sprintf_alloc("(%s)", bl->value);
	case AST_BREAK_STATEMENT:
		return sprintf_alloc("(break)");
	case AST_CALL_EXPRESSION:
		CallExpression *ce = (CallExpression *)node->value;
		return sprintf_alloc("(call %s %s)", ce->name, node_str(ce->arguments));
	case AST_CONTINUE_STATEMENT:
		return sprintf_alloc("(continue)");
	case AST_DECLARATION_STATEMENT:
		DeclarationStatement *ds = (DeclarationStatement *)node->value;
		return sprintf_alloc("(decl %s %s)", ds->id, node_str(ds->expression));
	case AST_FUNCTION_STATEMENT:
		FunctionStatement *fs = (FunctionStatement *)node->value;
		return sprintf_alloc("(func %s %s %s)", fs->name, str_list_str(fs->parameters), node_str(fs->block));
	case AST_IDENTIFIER:
		Identifier *i = (Identifier *)node->value;
		return sprintf_alloc("(id %s)", i->value);
	case AST_IF_STATEMENT:
		IfStatement *is = (IfStatement *)node->value;
		return sprintf_alloc("(if %s %s)", node_str(is->booleanExpression), node_str(is->block));
	case AST_LOGICAL_NOT_EXPRESSION:
		LogicalNotExpression *lne = (LogicalNotExpression *)node->value;
		return sprintf_alloc("(not %s)", node_str(lne->booleanExpression));
	case AST_LOGICAL_OPERAND:
		LogicalOperand *lo = (LogicalOperand *)node->value;
		return sprintf_alloc("(logOp %s %s %s)", node_str(lo->left), lo->operator, node_str(lo->right));
	case AST_NUMBER_LITERAL:
		NumberLiteral *nl = (NumberLiteral *)node->value;
		return sprintf_alloc("(num %s)", nl->value);
	case AST_PRINT_STATEMENT:
		PrintStatement *ps = (PrintStatement *)node->value;
		return sprintf_alloc("(print %s)", node_str(ps->expression));
	case AST_RETURN_STATEMENT:
		ReturnStatement *rs = (ReturnStatement *)node->value;
		return sprintf_alloc("(return %s)", node_str(rs->expression));
	case AST_STRING_LITERAL:
		StringLiteral *sl = (StringLiteral *)node->value;
		return sprintf_alloc("(str %s)", sl->value);
	case AST_TERM:
		Term *t = (Term *)node->value;
		return sprintf_alloc("(term %s %s %s)", node_str(t->left), t->operator, node_str(t->right));
	case AST_WHILE_STATEMENT:
		WhileStatement *ws = (WhileStatement *)node->value;
		return sprintf_alloc("(while %s %s)", node_str(is->booleanExpression), node_str(is->block));
	}
	return sprintf_alloc("(unrecognized)");
}
