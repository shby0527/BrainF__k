#ifndef _BF_STACK_H
#define _BF_STACK_H

#include <stddef.h>

/* A growable stack of size_t values. */
typedef struct BFStackContext {
	size_t offset;   /* number of elements currently stored */
	size_t capacity; /* number of elements allocated */
	size_t *addr;    /* backing storage */
} BFStackContext;

BFStackContext *bf_stack_create(size_t initial_capacity);
void bf_stack_destroy(BFStackContext *stack);

/* All return 0 on success, -1 on failure (NULL stack, bad argument,
 * underflow or allocation failure). */
int bf_stack_push(BFStackContext *stack, size_t addr);
int bf_stack_pop(BFStackContext *stack, size_t *value);
int bf_stack_peek(const BFStackContext *stack, size_t *value);

size_t bf_stack_count(const BFStackContext *stack);

#endif
