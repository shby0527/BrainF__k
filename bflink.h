#ifndef BF_LINK_H
#define BF_LINK_H
#include <stddef.h>

typedef struct BFMemoryNode{
	struct{
		struct BFMemoryNode *prev;
		struct BFMemoryNode *next;
	} ;
	size_t size;
	unsigned char memory[];
} BFMemoryNode;

typedef struct BFMemoryContext{
	BFMemoryNode *current;
	size_t offset;
	long long total_offset;
} BFMemoryContext;

BFMemoryContext *bf_memory_create();
void bf_memory_destroy(BFMemoryContext *context);
unsigned char get_current_data(BFMemoryContext *context);
void set_current_data(BFMemoryContext *context, unsigned char data);
void move_previous(BFMemoryContext *context);
void move_next(BFMemoryContext *context);
#endif
