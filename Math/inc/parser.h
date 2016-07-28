#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdint.h>
#include <stdio.h>

typedef struct parser_t parser_t;

typedef struct
{
	uint64_t value;
} term_t;

typedef struct
{
	uint64_t (*value)(uint64_t, uint64_t);
} op_t;

typedef struct
{
	term_t *lhs;
	term_t *rhs;
	op_t *op;
} expr_t;


parser_t *parser_initialize(FILE *stream);
void parser_uninitialize(parser_t *parser);
expr_t *parse(parser_t *parser);
uint64_t expr_eval(expr_t *expr);
#endif
