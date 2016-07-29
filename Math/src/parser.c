#ifndef _PARSE_H_
#define _PARSE_H_

#include <stdint.h>

#include "parser.h"
#include "lex.h"
#include "status.h"
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

static status_t parser_next(parser_t *parser)
{
	tok_uninitialize(parser->next);
	return lex_get_next_token(parser->lex, &parser->next);
}

status_t parse_term(parser_t *parser, term_t **term)
{
	status_t error = SUCCESS;
	if (tok_get_type(parser->next) == INTEGER)
	{
		*term = malloc(sizeof **term);
		if (*term == NULL)
		{
			error = OUT_OF_MEM;
			goto exit0;
		}

		(*term)->value = string_to_uint64(tok_get_value(parser->next));
		error = parser_next(parser);
		if (error)
		{
			goto error_exit0;
		}

		goto exit0;
	}

	*term = NULL;
	goto exit0;

error_exit0:
	free(term);

exit0:
	return error;
}

uint64_t plus(uint64_t a, uint64_t b)
{
	return a + b;
}

uint64_t minus(uint64_t a, uint64_t b)
{
	return a - b;
}

status_t parse_op(parser_t *parser, op_t **op)
{
	status_t error = SUCCESS;

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
		*op = NULL;
		goto exit0;
	}

	*op = malloc(sizeof **op);
	if (*op == NULL)
	{
		error = OUT_OF_MEM;
		goto exit0;
	}

	(*op)->value = op_func;
	error = parser_next(parser);
	if (error)
	{
		goto error_exit0;
	}

	goto exit0;

error_exit0:
	free(op);

exit0:
	return error;
}

status_t parse_expr(parser_t *parser, expr_t **expr)
{
	status_t error = SUCCESS;

	term_t *lhs;
	error = parse_term(parser, &lhs);
	if (error)
	{
		goto exit0;
	}

	if (lhs == NULL)
	{
		*expr = NULL;
		goto exit0;
	}

	op_t *op;
	error = parse_op(parser, &op);
	if (error)
	{
		goto error_exit0;
	}

	if (op == NULL)
	{
		*expr = NULL;
		goto error_exit0;
	}

	term_t *rhs;
	error = parse_term(parser, &rhs);
	if (error)
	{
		goto error_exit1;
	}

	if (rhs == NULL)
	{
		*expr = NULL;
		goto error_exit1;
	}

	if (tok_get_type(parser->next) != NEWLINE)
	{
		*expr = NULL;
		goto error_exit2;
	}

	*expr = malloc(sizeof **expr);
	if (*expr == NULL)
	{
		error = OUT_OF_MEM;
		goto error_exit2;
	}

	(*expr)->lhs = lhs;
	(*expr)->rhs = rhs;
	(*expr)->op = op;
	goto exit0;

error_exit2:
	free(rhs);
error_exit1:
	free(op);
error_exit0:
	free(lhs);

exit0:
	return error;
}

status_t parse(parser_t *parser, expr_t **expr)
{
	status_t error;
	
	error = lex_get_next_token(parser->lex, &parser->next);
	if (error)
	{
		goto exit0;
	}

	error = parse_expr(parser, expr);

exit0:
	return error;
}

void expr_uninitialize(expr_t *expr)
{
	term_uninitialize(expr->lhs);
	op_uninitialize(expr->op);
	term_uninitialize(expr->rhs);
	free(expr);
}

void term_uninitialize(term_t *term)
{
	free(term);
}

void op_uninitialize(op_t *op)
{
}
#endif
