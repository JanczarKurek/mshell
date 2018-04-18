#include <signal.h>
#include <stdlib.h>
#include "stdio.h"
#include <sys/wait.h>
#include <sys/types.h>
#include "shell_signals.h"
#include "process_tab.h"
#include <errno.h>

int ch_lock(){
    sigset_t s;
    sigemptyset(&s);
    sigaddset(&s,SIGCHLD);
    sigprocmask(SIG_BLOCK,&s,NULL);
    return 0;
}

int ch_ulock(){
    sigset_t s;
    sigemptyset(&s);
    sigaddset(&s,SIGCHLD);
    sigprocmask(SIG_UNBLOCK,&s,NULL);
    return 0;
}

void __SIGCHLD_handler(int _){
    pid_t pid;
    int status;
    //fprintf(stderr,"SIGCHLD catched!\n");
    pid = waitpid(-1,&status,WNOHANG );
    //fprintf(stderr,"pid == %d\n",pid);
    while(pid > 0){
        child_entry * proc = ch_find(pid);
        if(proc->__flags & PR_BACKGROUND){
            rg_bg_proc(pid,status);
        }
        ch_abort(pid);
        //fprintf(stderr,"Child dropped pid :%d\n",pid);
        pid = waitpid(-1,&status,WNOHANG);
    }
    //fprintf(stderr,"Errno : %d check with %d | %d | %d\n",errno,ECHILD,EINTR,EINVAL);
    errno = 0;
    return;
}

void __SIGINT_handler(int _){
    /*for(int i = 0;i < CH_TABLE_SIZE;++i){
        child_entry * chld = get_proc(i);
        if(chld->__pid != -1 && (chld->__flags & PR_FOREGROUND)){
            kill(SIGINT,chld->__pid);
        }
    }*/
}

struct sigaction __raw_sig_c;
struct sigaction __raw_sig_i;

void set_ch_handler(){
    struct sigaction sig;
    sig.sa_flags = SA_RESTART;
    sig.sa_handler = &__SIGCHLD_handler;
    sigaction(SIGCHLD,&sig,&__raw_sig_c);
    struct sigaction sig2;
    sig2.sa_flags = SA_RESTART;
    sig2.sa_handler = &__SIGINT_handler;
    sigaction(SIGINT,&sig2,&__raw_sig_i);
}

void reset_handlers(){
    sigaction(SIGCHLD,&__raw_sig_c,NULL);
    sigaction(SIGINT, &__raw_sig_i,NULL);
}
    
