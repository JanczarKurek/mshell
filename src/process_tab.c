#include <sys/types.h>
#include <string.h>
#include "better_rw.h"
#include "shell_signals.h"
#include "process_tab.h"
#include <stdio.h>

#define BG_MAX_SIZE 1024

child_entry ch_table[CH_TABLE_SIZE];

typedef struct bg_stat{
    pid_t __pid;
    int __status;
} bg_stat;

bg_stat bg_stats[BG_MAX_SIZE];

int volatile bg_num = 0;

int rg_bg_proc(pid_t pid,int status){
    //writeline(2,"reg!\n");
    bg_stats[bg_num].__pid = pid;
    bg_stats[bg_num].__status = status;
    if(bg_num < BG_MAX_SIZE) bg_num++;
    return 0;
}

int flush_stats(){
    //writeline(1,"Start flushing!\n");
    int num = bg_num;
    //fprintf(stderr,"bg_num = %d\n",num);
    bg_num = 0;
    /*sigset_t sgs,old;
    sigemptyset(&sgs);
    sigaddset(&sgs,SIGCHLD);
    sigaddset(&sgs,SIGINT);
    sigprocmask(SIG_BLOCK,&sgs,&old);*/
    char msg[1024];
    char nums[64];
    for(int i = 0;i < num;++i){
        msg[0] = 0;
        //fprintf(stderr,"I = %d\n",i);
        if(WIFEXITED(bg_stats[i].__status)){
            strcat(msg,":^()Background process ");
            sprintf(nums,"%d",bg_stats[i].__pid);
            strcat(msg,nums);
            strcat(msg," terminated. (exited with status ");
            sprintf(nums,"%d",WEXITSTATUS(bg_stats[i].__status));
            strcat(msg,nums);
            strcat(msg,")\n");
            writeline(1,msg);
        }else if(WIFSIGNALED(bg_stats[i].__status)){
            strcat(msg,"X^()Background process ");
            sprintf(nums,"%d",bg_stats[i].__pid);
            strcat(msg,nums);
            strcat(msg," terminated. (killed by signal ");
            sprintf(nums,"%d",WTERMSIG(bg_stats[i].__status));
            strcat(msg,nums);
            strcat(msg,")\n");
            writeline(1,msg);
        }
    }
    //sigprocmask(SIG_UNBLOCK,&sgs,NULL);
    return 0;
}
    

child_entry * get_proc(int i){
    return ch_table+i;
}

int prepare_chtable(){
    for(int i = 0;i < CH_TABLE_SIZE;++i){
        ch_table[i].__pid = -1;
    }
    return 0;
}

int __set_entry(child_entry * c,pid_t pid,int flags,const char *  name){
    c->__pid = pid;
    c->__flags = flags;
    strcpy(c->__name,name);
    return 0;
}

char ch_add(pid_t pid,int flags,const char * name){
    for(int i = 0;i < CH_TABLE_SIZE;++i){
        if(ch_table[i].__pid == -1){
            __set_entry(ch_table+i,pid,flags,name);
            //fprintf(stderr,"Process %s with pid %d registered \n",name,pid);
            return 0;
        }
    }
    __set_entry(ch_table,pid,flags,name);
    return CHILD_DROPPED;
}


child_entry * ch_find(pid_t pid){
    for(int i = 0;i < CH_TABLE_SIZE;++i){
        if(ch_table[i].__pid == pid){
            return ch_table+i;
        }
    }
    return NULL;
}

char ch_abort(pid_t pid){
    for(int i = 0;i < CH_TABLE_SIZE;++i){
        if(ch_table[i].__pid == pid){
            ch_table[i].__pid = -1;
            //fprintf(stderr,"Process %s with pid %d deleted\n",ch_table[i].__name,pid);
            return 0;
        }
    }
    return -1;
}

int ch_len(){
    int ret = 0;
    for(int i = 0;i < CH_TABLE_SIZE;++i){
        if(ch_table[i].__pid  != -1 && (ch_table[i].__flags & PR_FOREGROUND)){
            ret++;
        }
    }
    //fprintf(stderr,"Len is %d\n",ret);
    return ret;
}

int bring_out_your_deads(){
    //fprintf(stderr,"waiting starded\n");
    //fflush(stderr);
    sigset_t sgs,old;
    sigemptyset(&sgs);
    sigaddset(&sgs,SIGCHLD);
    sigaddset(&sgs,SIGINT);
    //fprintf(stderr,"To dziala\n");
    sigprocmask(SIG_BLOCK,&sgs,&old);
    while(ch_len()){
        //fprintf(stderr,"chlen %d\n",ch_len());
        //fflush(stderr);
        sigsuspend(&old);
    }
    sigprocmask(SIG_UNBLOCK,&sgs,NULL);
    //fprintf(stderr,"Ended!\n");
    return 0;
}
