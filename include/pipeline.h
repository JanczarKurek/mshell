#ifndef __PIPELINE__
#define __PIPELINE__

#define PIPE_START_ERROR 1
int __len(pipeline pip);

void exec_error(char * task_name);

int run_pipeline(pipeline pip,int flags);

#endif /* !__PIPELINE */
