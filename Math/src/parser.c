#ifndef _PARSE_H_
#define _PARSE_H_

#include <stdint.h>

#include "parser.h"
#include "lex.h"
#include "tok.h"

#define ASCII_0 48

/*
	Grammar:
		expr ::= term op term NEWLINE
		term ::= INTEGER
		op   ::= PLUS | MINUS
*/

struct parser_t
{
	lex_t *lex;
	tok_t *next;
};

parser_t *parser_initialize(FILE *stream)
{
	parser_t *parser = malloc(sizeof *parser);
	if (parser == NULL)
	{
		return NULL;
	}

	parser->lex = lex_initialize(stream);
	return parser;
}

void parser_uninitialize(parser_t *parser)
{
	lex_uninitialize(parser->lex);
	free(parser);
}

uint64_t expr_eval(expr_t *expr)
{
	return expr->op->value(expr->lhs->value, expr->rhs->value);
}

static uint64_t string_to_uint64(string_t *str)
{
	uint64_t value = 0;
	char *s = string_c_str(str);
    size_t length = string_length(str);
    size_t power10;
    size_t i;
    for (i = length - 1, power10 = 1; i < SIZE_MAX; i--, power10 *= 10)
    {
		//In this case, we've already determined that the token is a number -
		//don't worry about checking that
        value += (s[i] - ASCII_0) * power10;
    }
	
	return value;
}

static void parser_next(parser_t *parser)
{
	tok_uninitialize(parser->next);
	parser->next = lex_get_next_token(parser->lex);
}

term_t *parse_term(parser_t *parser)
{
	if (tok_get_type(parser->next) == INTEGER)
	{
		term_t *term = malloc(sizeof *term);
		if (term == NULL)
		{
			return NULL;
		}
		term->value = string_to_uint64(tok_get_value(parser->next));
		
		parser_next(parser);
		return term;
	}

	return NULL;
}

uint64_t plus(uint64_t a, uint64_t b)
{
	return a + b;
}

uint64_t minus(uint64_t a, uint64_t b)
{
	return a - b;
}

op_t *parse_op(parser_t *parser)
{
	uint64_t (*op_func)(uint64_t, uint64_t);
	tok_type_t type = tok_get_type(parser->next);
	if (type == PLUS)
	{
		op_func = plus;
	}
	else if (type == MINUS)
	{
		op_func = minus;
	}
	else
	{
		return NULL;
	}

	op_t *op = malloc(sizeof *op);
	if (op == NULL)
	{
		return NULL;
	}
	op->value = op_func;

	parser_next(parser);
	return op;
}

expr_t *parse_expr(parser_t *parser)
{
	term_t *lhs = parse_term(parser);
	if (lhs == NULL)
	{
		return NULL;
	}

	op_t *op = parse_op(parser);
	if (op == NULL)
	{
		free(lhs);
		return NULL;
	}

	term_t *rhs = parse_term(parser);
	if (rhs == NULL)
	{
		free(lhs);
		free(op);
		return NULL;
	}

	if (tok_get_type(parser->next) != NEWLINE)
	{
		free(lhs);
		free(op);
		free(rhs);
		return NULL;
	}

	expr_t *expr = malloc(sizeof *expr);
	if (expr == NULL)
	{
		free(lhs);
		free(op);
		free(rhs);
		return NULL;
	}
	expr->lhs = lhs;
	expr->rhs = rhs;
	expr->op = op;

	return expr;
}

expr_t *parse(parser_t *parser)
{
	parser->next = lex_get_next_token(parser->lex);
	return parse_expr(parser);
}

#endif
