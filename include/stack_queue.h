#ifndef STQUEUE_YES
#define STQUEUE_YES
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
struct Stack{
	char * content;
	char * content_end;
	char * allocated_end;
};

struct Stack * NewStack(int);

int lenS(struct Stack *);

int capacityS(struct Stack *);

void increaseCapS(struct Stack *);

void pushS(struct Stack *,char);

char topS(struct Stack *);

int popS(struct Stack *);

struct Queue{
	struct Stack* in;
	struct Stack* out;
};

struct Queue * NewQueue(int);

void pushQ(struct Queue *,char);

void popQ(struct Queue *);

int lenQ(struct Queue *);

char frontQ(struct Queue *);

void purgeS(struct Stack *);

void purgeQ(struct Queue *);
#endif

