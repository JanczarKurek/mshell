#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "siparse.h"
#include "redir.h"
#include "pipeline.h"
#include <string.h>
#include <errno.h>
#include <better_rw.h>
#include <stdio.h>
#include <signal.h>
#include "shell_signals.h"
#include "process_tab.h"
void exec_error(char * task_name){
    char error[1024];
    //printf("%d %d %d\n",errno,ENOENT,EACCES);
    error[0] = '\0';
    if(errno == ENOENT){
        strcat(error,task_name);
        strcat(error,": no such file or directory\n");
        writeline(2,error);
    }else if(errno == EACCES){
        strcat(error,task_name);
        strcat(error,": permission denied\n");
        writeline(2,error);
    }/*else{
        strcat(error,task_name);
        strcat(error,": ");
        strcat(error,EXEC_ERROR);
        writeline(2,error);
    }*/
    errno = 0;
}
int __len(pipeline pip){
    int i = 0;
    while(pip[i]){
        i++;
    } return i;
}

int run_pipeline(pipeline pip,int flags){
    if(__len(pip) == 1){
        //Trivial case.
        int f = fork();
        if(f == 0){
            if(flags & PR_BACKGROUND){
                //drop_io();
                setsid();
            }
            reset_handlers();
            if(redirect((*pip)->redirs) & REDIR_ERROR){
                _exit(EXIT_FAILURE);
            }
            if(execvp((*pip)->argv[0],(*pip)->argv) < 0){
                exec_error((*pip)->argv[0]);
                _exit(EXIT_FAILURE);
            }
        }else{
            ch_lock();
            ch_add(f,flags,(*pip)->argv[0]);
            ch_ulock();
            bring_out_your_deads();
        }
    }else{
        int pipes[2];
        int read_pipe = -1;
        int res = pipe(pipes);
        int write_pipe = pipes[1];
        while(*pip){
            //fprintf(stderr,"Counting loops!\n");
            int f = fork();
            //fprintf(stderr,"FORKED %d\n",f);
            if(f == 0){
                if(flags & PR_BACKGROUND){
                    //drop_io();
                    setsid();
                }
                reset_handlers();
                if(write_pipe != -1){
                    dup2(write_pipe,1);
                    close(write_pipe);
                }if(read_pipe != -1){
                    dup2(read_pipe,0);
                    close(read_pipe);
                }
                close(pipes[0]);
                if(redirect((*pip)->redirs) & REDIR_ERROR){
                    _exit(EXIT_FAILURE);
                }
                if(execvp((*pip)->argv[0],(*pip)->argv) < 0){
                    exec_error((*pip)->argv[0]);
                    _exit(EXIT_FAILURE);
                }
            }else{
                ch_lock();
                ch_add(f,flags,(*pip)->argv[0]);
                ch_ulock();
                if(read_pipe != -1) close(read_pipe);
                if(write_pipe != -1) close(write_pipe);
                if(*(pip+1) != NULL){
                    read_pipe = pipes[0];
                    if(*(pip+2) != NULL){
                        res = pipe(pipes);
                        write_pipe = pipes[1];
                    }else write_pipe = -1;
                }
            }
            //fprintf(stderr,"%d %d\n",close(pipes[0]),close(pipes[1]));
            pip++;
        }
        //fprintf(stderr,"Start waiting for kids\n");
        //fflush(stderr);
        bring_out_your_deads();
    }
    return 0;
}
