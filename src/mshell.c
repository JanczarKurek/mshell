#include <stdio.h>
#include <unistd.h>
#include "config.h"
#include "siparse.h"
#include "utils.h"
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "stack_queue.h"
#include "better_rw.h"
#include "builtins.h"
#include <errno.h>
#include "redir.h"
#include "pipeline.h"
#include "process_tab.h"
#include "shell_signals.h"

const char * INPUT_ERROR="Syntax error.\n";
const char * PARSE_ERROR="Parse error.\n";
const char * EXEC_ERROR ="Execution error.\n";
const char * PROMPT = ":^)";

char is_char_dev(){
    struct stat buff;
    fstat(0,&buff);
    return S_ISCHR(buff.st_mode);
}

void prompt_it(){
    if(is_char_dev()){
        sigset_t sgs,old;
        sigemptyset(&sgs);
        sigaddset(&sgs,SIGCHLD);
        sigaddset(&sgs,SIGINT);
        sigprocmask(SIG_BLOCK,&sgs,&old);
        flush_stats();
        writeline(1,PROMPT);
        sigprocmask(SIG_UNBLOCK,&sgs,NULL);
    }
}

char check_line(line * ln){
    //fprintf(stderr,"Im here!\n");
    for(int i = 0;ln->pipelines[i] != NULL;++i){
        //fprintf(stderr,"And here\n");
        if(ln->pipelines[i][0] != NULL && ln->pipelines[i][1] != NULL){
            for(int j = 0;ln->pipelines[i][j] != NULL;++j){
                if(ln->pipelines[i][j]->argv ==  NULL){
                    fprintf(stderr,"i = %d, j = %d\n",i,j);
                    return 0;
                }
            }
        }
    }
    return 1;
}
int main(int argc, char *argv[])
{
    prepare_chtable();
    set_ch_handler();
    char input[MAX_LINE_LENGTH];
    int input_size = 0;
	line * ln;
	command *com;
	while(1){
	    /*if(is_char_dev()){
	        writeline(1,PROMPT);
        }*/
        prompt_it();
	    input_size = readline(0,input,MAX_LINE_LENGTH);
        if(input_size < 0){
            writeline(2,INPUT_ERROR);
            continue;
        }
        if(input_size == 0){
            _exit(0);
        }
        ln = parseline(input);
        //fprintf(stderr,"Parsed!\n");
        if(ln == NULL || !check_line(ln)){
            writeline(2,INPUT_ERROR);
            continue;
        }
        com = pickfirstcommand(ln);
        if(com->argv[0] == NULL){
            continue;
        }
        if(strlen(com->argv[0]) == 0){
            continue;
        }
        //printcommand(com,1);
        if(run_builtin(com->argv[0],com->argv) != NOT_BUILTIN){
            continue;
        }
        int pp = 0;
        while(ln->pipelines[pp]){
            int run_flags = ((ln->flags & LINBACKGROUND) ? PR_BACKGROUND : PR_FOREGROUND);
            run_pipeline(ln->pipelines[pp],run_flags);
            pp++;
        }
    }

}
