#ifndef _BF_STACK_H
#define _BF_STACK_H

typedef struct _bf_stack_ {
	int date;
	struct _bf_stack_* next;
} BFSTACK;


BFSTACK* init_stack();


void push_stack(BFSTACK**, int);


int get_top(BFSTACK**);


int pop_stack(BFSTACK**);


void destory_stack(BFSTACK**);
#endif
