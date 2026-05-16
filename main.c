#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "config.h"
#include "bflink.h"
#include "bfstack.h"
#include <complex.h>

char* bf_source = NULL;

BFLINK* memory = NULL;
BFSTACK* stack = NULL;

char* read_source(int fd)
{
	int char_size = 256;
	bf_source = (char*)malloc(sizeof(char) * char_size);
	int padded = 0;
	ssize_t readed = 0;
	do {
		if (padded + 255 > char_size ) {
			char_size = char_size * 2;
			bf_source = (char*)realloc(bf_source, sizeof(char) * char_size);
		}
		readed = read(fd, bf_source + padded, 255);
		padded += readed;
	} while(readed >= 255);
	return bf_source;
}

void run()
{
	memory = init_link();
	stack = init_stack();
	int pc = 0;
	while(*(bf_source + pc) != 0) {
		char now = *(bf_source + pc);
		switch(now){
			case '+':
				memory->data++;
				break;
			case '-':
				memory->data--;
				break;
			case '>':
				memory = move_to_next(memory); 
				break;
			case '<':
				memory = move_to_prev(memory);
				break;
			case ',':
				memory->data = getchar();
				break;
			case '.':
				putchar(memory->data);
				break;
			case '[':
				push_stack(&stack, pc);
				break;
			case ']':
				if (memory->data == 0) {
					pop_stack(&stack);
				} else {
					pc = get_top(&stack);
				}
				break;
			default:
				break;
		}
		pc++;
	}
	destory_link(memory);
	destory_stack(&stack);
}

int main(int argc, char** args)
{
	if (argc <= 1) {
	    printf("version is %d.%d \n\n", BF_VERSION_MAJOR, BF_VERSION_MINOR);
		printf("bf filename if is - read from stdin \n");
		return 1;
	}
	char* filename = args[1];
	int fd = 0;
	if (strcmp(filename, "-") == 0) {
		fd = STDIN_FILENO;
	} else {
		fd = open(filename, O_RDONLY);
	}
	if (fd < 0) {
		printf("failure to open file\n");
		return -4;
	}
	read_source(fd);
	close(fd);
	run();
	free(bf_source);
	return 0;
}
