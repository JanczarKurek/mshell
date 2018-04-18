#ifndef BETTER_RW
#define BETTER_RW
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "stack_queue.h"
#define MAX_INPUT_LEN 1024
#define BUFF_SIZE 2048

int readline(int,char *,int);

int writeline(int,const char *);
#endif
