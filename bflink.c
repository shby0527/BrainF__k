#include <stdlib.h>
#include <string.h>
#include "bflink.h"


BFLINK* move_to_first(BFLINK* node) 
{
	if (node == NULL)
		return NULL;
	while (node->prev != NULL) {
		node = node->prev;
	}
	return node;
}


BFLINK* init_link() 
{
	BFLINK* rtv = NULL;
	rtv = (BFLINK*) malloc(sizeof(BFLINK));
	memset(rtv, 0, sizeof(BFLINK));
	return rtv;
}



void destory_link(BFLINK* node) {
	BFLINK* tmp = move_to_first(node);
	BFLINK* destory = tmp;
	while (tmp != NULL) {
		tmp = tmp->next;
		free(destory);
		destory = tmp;
	}
}



BFLINK* move_to_next(BFLINK* node)
{
	if (node == NULL)
		return NULL;
	if (node->next != NULL)
		return node->next;
	BFLINK* next = (BFLINK*)malloc(sizeof(BFLINK));
	memset(next, 0, sizeof(BFLINK));
	node->next = next;
	next->prev = node;
	return next;
}


BFLINK* move_to_prev(BFLINK* node)
{
	if (node == NULL)
		return NULL;
	if (node->prev != NULL)
		return node->prev;
	BFLINK* prev = (BFLINK*)malloc(sizeof(BFLINK));
	memset(prev, 0, sizeof(BFLINK));
	prev->next = node;
	node->prev = prev;
	return prev;
}
