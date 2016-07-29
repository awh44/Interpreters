#ifndef _STATUS_H_
#define _STATUS_H_

typedef enum
{
	SUCCESS = 0,
	OUT_OF_MEM,
	UNRECOGNIZED_CHARACTER,
} status_t;

void print_error(status_t error);

#endif
