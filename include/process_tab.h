#include <sys/types.h>
#include <string.h>
#include "shell_signals.h"

#ifndef __PROCESS_TAB__
#define __PROCESS_TAB__

#define CHILD_DROPPED 2
#define PR_BACKGROUND 1
#define PR_FOREGROUND 2
#define CH_TABLE_SIZE 1024

typedef struct child_entry{
    pid_t __pid;
    int __flags;
    char __name[1024];
}child_entry;

int rg_bg_proc(pid_t,int);

int flush_stats();

child_entry * get_proc(int);

int prepare_chtable();

int __set_entry(child_entry *,pid_t,int,const char *);

char ch_add(pid_t,int,const char*);

child_entry * ch_find(pid_t);

char ch_abort(pid_t);

int ch_len();

int bring_out_your_deads();
#endif /* !__PROCESS_TAB__*/
