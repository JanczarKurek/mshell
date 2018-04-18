INC=-Iinclude -Wall -O2
CFLAGS=$(INC)

PARSERDIR=input_parse

SRCS=utils.c mshell.c builtins.c stack_queue.c better_rw.c redir.c pipeline.c shell_signals.c process_tab.c
OBJS:=$(SRCS:.c=.o)

all: mshell 

mshell: $(OBJS) siparse.a
	cc $(CFLAGS) $(OBJS) siparse.a -o $@ 

%.o: src/%.c
	cc $(CFLAGS) -c $<

siparse.a:
	$(MAKE) -C $(PARSERDIR)

clean:
	rm -f mshell siparse.a *.o
