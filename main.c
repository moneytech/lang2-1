#include "ast.h"
#include "list.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(void)
{
	Lexer *lexer = new_lexer("1.txt");
	Parser *parser = new_parser(lexer);
	Node *node = parser_block(parser);
	const char *parsedebug = getenv("PARSEDEBUG");
	if (parsedebug && strcmp(parsedebug, "")) {
		//fprintf(stderr, "%s\n", node);
	}
	return 0;
}
