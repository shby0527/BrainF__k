#ifndef BF_LINK_H
#define BF_LINK_H

typedef struct _bf_link_ {
	unsigned char data;
	struct _bf_link_* prev;
	struct _bf_link_* next;
} BFLINK;


BFLINK* init_link();


void destory_link(BFLINK*);


BFLINK* move_to_next(BFLINK*);


BFLINK* move_to_prev(BFLINK*);


#endif
