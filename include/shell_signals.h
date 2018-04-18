#include <signal.h>
#ifndef __SHELL_SIGNALS__
#define __SHELL_SIGNALS__

int ch_lock();

int ch_ulock();

void __SIGCHLD_handler(int);

void set_ch_handler();

void reset_handlers();
#endif /*!__SHELL_SIGNALS__*/
