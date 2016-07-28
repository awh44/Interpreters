#ifndef _TOK_H_
#define _TOK_H_

#include "string_t.h"

typedef enum
{
	TOK_EOF,
	INTEGER,
	PLUS,
	MINUS,
	NEWLINE,
	UNRECOGNIZED,
} tok_type_t;

typedef struct tok_t tok_t;

tok_t *tok_initialize(void);
void tok_uninitialize(tok_t *tok);
tok_type_t tok_get_type(tok_t *tok);
void tok_set_type(tok_t *tok, tok_type_t type);
string_t *tok_get_value(tok_t *tok);
void tok_value_append(tok_t *tok, char c);

#endif
