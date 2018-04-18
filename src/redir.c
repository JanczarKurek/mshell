#include "siparse.h"
#include "better_rw.h"
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "redir.h"
#include <stdio.h>
#include <errno.h>
int redirect(redirection ** rd){
    int i = 0;
    char * in = NULL;
    char * out= NULL;
    char * aut= NULL;
    while(rd[i] != NULL){
        if(IS_RIN(rd[i]->flags)){
            in = rd[i]->filename;
        }else if(IS_ROUT(rd[i]->flags)){
            out = rd[i]->filename;
        }else if(IS_RAPPEND(rd[i]->flags)){
            aut = rd[i]->filename;
        }
        i++;
    }
    int new_in = 0;
    int new_out= 1;
    if(in){
        new_in = open(in,O_RDONLY);
        if(new_in < 0){
            if(errno == ENOENT){
                char error[1024];
                error[0] = '\0';
                strcat(error,in);
                strcat(error,": no such file or directory\n");
                writeline(2,error);
                errno = 0;
            }else if(errno == EACCES){
                char error[1024];
                error[0] = '\0';
                strcat(error,in);
                strcat(error,": permission denied\n");
                writeline(2,error);
                errno = 0;
            }
        }
    }if(out){
        new_out = open(out,O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP);
        if(new_out < 0){
            if(errno == EACCES){
                char error[1024];
                error[0] = '\0';
                strcat(error,out);
                strcat(error,": permission denied\n");
                writeline(2,error);
                errno = 0;
            }
        }
    }if(aut){
        new_out = open(aut,O_WRONLY | O_APPEND | O_CREAT, S_IRUSR| S_IWUSR | S_IRGRP);
        if(new_out < 0){
            if(errno == EACCES){
                char error[1024];
                error[0] = '\0';
                strcat(error,aut);
                strcat(error,": permission denied\n");
                writeline(2,error);
                errno = 0;
            }
        }
    }
    if(new_in < 0){
        if(new_out > 1){
            close(new_out);
        }
        return REDIR_ERROR | NO_REDIR;
    }if(new_out < 0){
        if(new_in > 0){
            close(new_in);
        }
        return REDIR_ERROR | NO_REDIR;
    }
    dup2(new_in,0);
    dup2(new_out,1);
    if(new_in > 0) close(new_in);
    if(new_out> 1) close(new_out);
    return REDIRECTED;
}

int drop_io(){
    close(0);
    close(1);
    close(2);
    return 0;
}
 
