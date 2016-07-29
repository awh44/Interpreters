#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "status.h"


int main(void)
{
	status_t error = SUCCESS;
	parser_t *parser = parser_initialize(stdin);
	expr_t *expr;
	
	error = parse(parser, &expr);
	while (!error && expr != NULL)
	{
		printf("%lu\n", expr_eval(expr));
		expr_uninitialize(expr);
		
		error = parse(parser, &expr);
	}

exit1:
	parser_uninitialize(parser);
exit0:
	return error;
}
