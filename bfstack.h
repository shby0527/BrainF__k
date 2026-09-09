#ifndef _BF_STACK_H
#define _BF_STACK_H

#include <stddef.h>

typedef struct BFStackContext {
	size_t offset;
	size_t size;
	size_t addr[];
} BFStackContext;

BFStackContext *bf_stack_create(size_t size);
void bf_stack_destroy(BFStackContext *stack);
void push(BFStackContext *stack, size_t addr);
size_t pop(BFStackContext *stack);
size_t peek(BFStackContext *stack);
#endif
