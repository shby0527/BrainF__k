#include "bflink.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

const size_t BLOCK_SIZE = 4096;

BFMemoryContext* bf_memory_create() {
	BFMemoryContext* context = (BFMemoryContext*)malloc(sizeof(BFMemoryContext));
	if (!context) {
		return NULL;
	}
	context->current = (BFMemoryNode*)malloc(sizeof(BFMemoryNode) + BLOCK_SIZE*sizeof(unsigned char));
	if (!context->current) {
		free(context);
		return NULL;
	}
	context->current->prev = NULL;
	context->current->next = NULL;
	context->current->size = BLOCK_SIZE;
	memset(context->current->memory, 0, BLOCK_SIZE); // Initialize memory to zero
	context->offset = 0;
	context->total_offset = 0;
	return context;
}

void bf_memory_destroy(BFMemoryContext* context) {
	if (!context) {
		return;
	}
	BFMemoryNode* node = context->current;
	// move to the head of the list
	while (node && node->prev) {
		node = node->prev;
	}
	// destroy all nodes
	while (node) {
		BFMemoryNode* next = node->next;
		free(node);
		node = next;
	}
	free(context);
}

void move_previous(BFMemoryContext* context) {
	if (!context || !context->current) {
		fprintf(stderr, "Error: Context or current node is NULL\n");
		return;
	}
	if (context->offset > 0) {
		context->offset--;
		context->total_offset--;
	} else if (context->current->prev) {
		context->current = context->current->prev;
		context->offset = context->current->size - 1;
		context->total_offset--;
	} else {
		// create a new node at the beginning
		BFMemoryNode* new_node = (BFMemoryNode*)malloc(sizeof(BFMemoryNode) + BLOCK_SIZE*sizeof(unsigned char));
		if (!new_node) {
			fprintf(stderr, "Error: Memory allocation failed\n");
			return;
		}
		new_node->prev = NULL;
		new_node->next = context->current;
		new_node->size = BLOCK_SIZE;
		memset(new_node->memory, 0, BLOCK_SIZE);
		context->current->prev = new_node;
		context->current = new_node;
		context->offset = BLOCK_SIZE - 1;
		context->total_offset--;
	}
}

void move_next(BFMemoryContext* context) {
	if (!context || !context->current) {
		fprintf(stderr, "Error: Context or current node is NULL\n");
		return;
	}
	if (context->offset < context->current->size - 1) {
		context->offset++;
		context->total_offset++;
	} else if (context->current->next) {
		context->current = context->current->next;
		context->offset = 0;
		context->total_offset++;
	} else {
		// create a new node at the end
		BFMemoryNode* new_node = (BFMemoryNode*)malloc(sizeof(BFMemoryNode) + BLOCK_SIZE*sizeof(unsigned char));
		if (!new_node) {
			fprintf(stderr, "Error: Memory allocation failed\n");
			return;
		}
		new_node->prev = context->current;
		new_node->next = NULL;
		new_node->size = BLOCK_SIZE;
		memset(new_node->memory, 0, BLOCK_SIZE);
		context->current->next = new_node;
		context->current = new_node;
		context->offset = 0;
		context->total_offset++;
	}
}

void set_current_data(BFMemoryContext* context, unsigned char data) {
	if (!context || !context->current) {
		fprintf(stderr, "Error: Context or current node is NULL\n");
		return;
	}
	context->current->memory[context->offset] = data;
}

unsigned char get_current_data(BFMemoryContext* context) {
	if (!context || !context->current) {
		fprintf(stderr, "Error: Context or current node is NULL\n");
		return 0;
	}
	return context->current->memory[context->offset];
}