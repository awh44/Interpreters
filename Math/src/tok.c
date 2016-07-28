#include <stdlib.h>

#include "tok.h"
#include "string_t.h"

struct tok_t
{
	tok_type_t type;
	string_t value;
};

tok_t *tok_initialize(void)
{
	tok_t *tok = malloc(sizeof *tok);
	if (tok == NULL)
	{
		return NULL;
	}

	string_initialize(&tok->value);
	return tok;
}

void tok_uninitialize(tok_t *tok)
{
	string_uninitialize(&tok->value);
	free(tok);
}

tok_type_t tok_get_type(tok_t *tok)
{
	return tok->type;
}

void tok_set_type(tok_t *tok, tok_type_t type)
{
	tok->type = type;
}

string_t *tok_get_value(tok_t *tok)
{
	return &tok->value;
}

void tok_value_append(tok_t *tok, char c)
{
	char_vector_push_back(&tok->value, c);
}
