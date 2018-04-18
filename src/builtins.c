#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include "builtins.h"
#include "better_rw.h"
#include <signal.h>
#include <sys/types.h>

int echo(char*[]);
int undefined(char *[]);
int shell_exit(char *[]);
int cd(char *[]);
int ls(char *[]);
int killall(char *[]);

builtin_pair builtins_table[]={
	{"exit",	&shell_exit},
	{"lecho",	&echo},
	{"lcd",		&cd},
	{"lkill",	&killall},
	{"lls",		&ls},
	{NULL,NULL}
};

int run_builtin(char * name,char *argv[]){
    int i = 0;
    while(builtins_table[i].name){
        if(strcmp(builtins_table[i].name,name) == 0){
            if(builtins_table[i].fun(argv) == BUILTIN_ERROR){
                char error[1024];
                error[0] = '\0';
                strcat(error,"Builtin ");
                strcat(error,name);
                strcat(error," error.\n");
                writeline(2,error);
                return 0;
            }
            return 0;
        }
        i++;
    }
    return NOT_BUILTIN;
}

int killall(char * argv[]){
    pid_t pid = 0;
    int sig = SIGTERM;
    int i = 1;
    if(argv[1] == NULL || argv[2] == NULL){
        return BUILTIN_ERROR;
    }
    while(argv[i]){
        if(argv[i][0] == '-'){
            sig = atoi(argv[i]+1);
        }else{
            pid = atoi(argv[i]);
        }
        i++;
    }
    //fprintf(stderr,"Trying to kill %d with signal %d\n",pid,sig);
    return kill(pid,sig);
}

int
cd(char * argv[]){
    if(argv[1] != NULL && argv[2] != NULL) return BUILTIN_ERROR;
    if(argv[1] == NULL){
        char * path = getenv("HOME");
        chdir(path);
        return 0;
    }
    int status = chdir(argv[1]);
    //fprintf(stderr,"Status %d\n",status);
    return status < 0 ? BUILTIN_ERROR : 0;
}

int
ls(char * argv[]){
    DIR * dir = opendir(".");
    struct dirent * d;
    while(d = readdir(dir)){
        if(d->d_name[0] != '.'){
            writeline(1,d->d_name);
            writeline(1,"\n");
        }
    }
    return 0;
}

int 
echo( char * argv[])
{
	int i =1;
	if (argv[i]) printf("%s", argv[i++]);
	while  (argv[i])
		printf(" %s", argv[i++]);

	printf("\n");
	fflush(stdout);
	return 0;
}

int 
undefined(char * argv[])
{
	fprintf(stderr, "Command %s undefined.\n", argv[0]);
	return BUILTIN_ERROR;
}

int shell_exit(char * argv[]){
    _exit(0);
    return BUILTIN_ERROR;
}
