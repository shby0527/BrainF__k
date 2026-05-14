#include <stdlib.h>
#include <string.h>
#include "bfstack.h"



BFSTACK* init_stack()
{
	BFSTACK* rtv = (BFSTACK*)malloc(sizeof(BFSTACK));
	memset(rtv, 0, sizeof(BFSTACK));
	return rtv;
}



void push_stack(BFSTACK** node, int date) 
{
	if (node == NULL || *node == NULL)
		return;
	BFSTACK* param = *node;
	BFSTACK* top = (BFSTACK*)malloc(sizeof(BFSTACK));
	memset(top, 0, sizeof(BFSTACK));
	top->date = date;
	top->next = param;
	*node = top;
	return;
}


int get_top(BFSTACK** node)
{
	if (node == NULL || *node == NULL)
		return 0;
	return (*node)->date;
}


int pop_stack(BFSTACK** node)
{
	if (node == NULL || *node == NULL)
		return 0;
	BFSTACK* param = *node;
	if (param->next == NULL) {
		// already in stack buttom
		return 0;
	}
	int date = param->date;
	*node = param->next;
	free(param);
	return date;
}


void destory_stack(BFSTACK** node)
{
	if (node == NULL || *node == NULL)
		return;
	BFSTACK* param = *node; // 一定是顶
	while(param->next != NULL) {
		pop_stack(&param);
	}
	free(param);
	return;
}
