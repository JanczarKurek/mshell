#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include "stack_queue.h"
struct Stack * NewStack(int size){
	struct Stack * ret = (struct Stack*)malloc(sizeof(struct Stack));
	ret->content = (char*)malloc(sizeof(char)*size);
	ret->content_end = ret->content;
	ret->allocated_end = ret->content + size;
	return ret;
}

int lenS(struct Stack * a){
	return a->content_end - a->content;
}

int capacityS(struct Stack * a){
	return a->allocated_end - a->content;
}

void increaseCapS(struct Stack * a){
	int temp = lenS(a);
	int tem2 = capacityS(a);
	a->content = (char*)realloc(a->content,sizeof(char)*tem2*2);
	a->content_end = a->content + temp;
	a->allocated_end = a->content + tem2*2;
}

void pushS(struct Stack * sc,char val){
	if(lenS(sc) == capacityS(sc)){
		increaseCapS(sc);
	}
	*(sc->content_end) = val;
	sc->content_end += 1;
    //fprintf(stderr,"After push stack len is: %d\n",lenS(sc));
}
char topS(struct Stack * sc){
	//fprintf(stderr,"Test %d %d %d\n",sc->content,sc->content_end,sc->allocated_end);
	return *((sc->content_end)-1);
}

int popS(struct Stack * sc){
	if(lenS(sc) == 0){
		return -1;
	}else{
		sc->content_end -=1;
		return 1;
	}
}

/*void test_print_content(struct Stack * sc){
	for(int i = 0; i < len(sc);++i){
		printf("%c",(sc->content)[i]);
	}
	printf("\n");
}*/

struct Queue * NewQueue(int size){
	struct Queue * q = (struct Queue *)malloc(sizeof(struct Queue));
	q->in = NewStack(size);
	q->out = NewStack(size);
	return q;
}

void pushQ(struct Queue * q,char val){
    //fprintf(stderr,"Pushing val = %c\n",val);
	pushS(q->in,val);
}

void popQ(struct Queue * q){
	if(lenS(q->out)){
		popS(q->out);
	}else{
		while(lenS(q->in)){
			pushS(q->out,topS(q->in));
			popS(q->in);
		}
	}
}

int lenQ(struct Queue * q){
	return lenS(q->in)+lenS(q->out);
}

char frontQ(struct Queue * q){
	if(!lenS(q->out)){
		while(lenS(q->in)){
			pushS(q->out,topS(q->in));
			popS(q->in);
		}
	}
	return topS(q->out);
}
void purgeS(struct Stack * s){
	free(s->content);
	free(s);
}

void purgeQ(struct Queue * q){
	purgeS(q->in);
	purgeS(q->out);
	free(q);
}
