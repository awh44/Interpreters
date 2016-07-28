#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

#include "lex.h"
#include "string_t.h"
#include "tok.h"

struct lex_t
{
	FILE *stream;
	char next;
};

static void lex_skip_whitespace(lex_t *lex);
static void lex_get_integer(lex_t *lex, tok_t *tok);
static void lex_get_character_tok(lex_t *lex, tok_t *tok, tok_type_t type);
static void lex_get_plus(lex_t *lex, tok_t *tok);
static void lex_get_minus(lex_t *lex, tok_t *tok);
static void lex_get_newline(lex_t *lex, tok_t *tok);
static void lex_get_unrecognized(lex_t *lex, tok_t *tok);

/*
	AHA! So here's the thing - THIS creates tok_t's that are just for the
	tokens themselves - e.g., INTEGER, PLUS, MINUS - while the PARSER creates
	the tokens/nodes for the larger things like terms or exprs

	In that lambda language I found, that guy doesn't check that the operators
	are valid until the evalaution stage...that seems like a mistake. I think
	what I'm going to want to do is make the instructions just part of the
	language.
*/

lex_t *lex_initialize(FILE *stream)
{
	lex_t *lex = malloc(sizeof *lex);
	if (lex == NULL)
	{
		return NULL;
	}

	lex->stream = stream;
	return lex;
}

void lex_uninitialize(lex_t *lex)
{
	free(lex);
}

tok_t *lex_get_next_token(lex_t *lex)
{
	tok_t *tok = tok_initialize();
	if (tok == NULL)
	{
		goto exit0;
	}

	lex->next = getc(lex->stream);
	lex_skip_whitespace(lex);

	if (lex->next == EOF)
	{
		tok_set_type(tok, TOK_EOF);
		goto exit0;
	}
	
	if (isdigit(lex->next))
	{
		lex_get_integer(lex, tok);
		goto exit0;
	}

	if (lex->next == '+')
	{
		lex_get_plus(lex, tok);
		goto exit0;
	}

	if (lex->next == '-')
	{
		lex_get_minus(lex, tok);
		goto exit0;
	}

	if (lex->next == '\n')
	{
		lex_get_newline(lex, tok);
		goto exit0;
	}

	lex_get_unrecognized(lex, tok);

exit0:
	return tok;
}

static void lex_skip_whitespace(lex_t *lex)
{
	while (lex->next == ' ' || lex->next == '\t')
	{
		lex->next = getc(lex->stream);
	}
}

static void lex_get_integer(lex_t *lex, tok_t *tok)
{
	tok_set_type(tok, INTEGER);
	do
	{
		tok_value_append(tok, lex->next);
		lex->next = getc(lex->stream);
	} while (isdigit(lex->next));

	//retrieved one character that is not a digit - put
	//it back in the stream
	ungetc(lex->next, lex->stream);
}

static void lex_get_character_tok(lex_t *lex, tok_t *tok, tok_type_t type)
{
	tok_set_type(tok, type);
	tok_value_append(tok, lex->next);
}

static void lex_get_plus(lex_t *lex, tok_t *tok)
{
	lex_get_character_tok(lex, tok, PLUS);
}

static void lex_get_minus(lex_t *lex, tok_t *tok)
{
	lex_get_character_tok(lex, tok, MINUS);
}

static void lex_get_newline(lex_t *lex, tok_t *tok)
{
	lex_get_character_tok(lex, tok, NEWLINE);
}

static void lex_get_unrecognized(lex_t *lex, tok_t *tok)
{
	lex_get_character_tok(lex, tok, UNRECOGNIZED);
}
