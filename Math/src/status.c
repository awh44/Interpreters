#include <stdio.h>

#include "status.h"

void print_error(status_t error)
{
	if (!error)
	{
		return;
	}

	printf("ERROR: ");
	switch (error)
	{
		case OUT_OF_MEM:
			printf("Out of memory");
			break;
		case UNRECOGNIZED_CHARACTER:
			printf("Unrecognized character in input stream");
			break;
	}

	printf("\n");
}
