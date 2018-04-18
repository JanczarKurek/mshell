#include "siparse.h"
#include <sys/stat.h>
#include <unistd.h>
#ifndef __REDIR__
#define __REDIR__

#define REDIRECTED 0
#define REDIR_ERROR 1
#define NO_REDIR 2

int redirect(redirection **);
int drop_io();
#endif /*__REDIR__*/
