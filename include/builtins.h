#ifndef _BUILTINS_H_
#define _BUILTINS_H_

#define BUILTIN_ERROR 2
#define NOT_BUILTIN 1

typedef struct {
	char* name;
	int (*fun)(char**); 
} builtin_pair;

extern builtin_pair builtins_table[];

int run_builtin(char *,char *[]);

#endif /* !_BUILTINS_H_ */
