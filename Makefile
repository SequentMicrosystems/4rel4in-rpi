DESTDIR?=/usr
PREFIX?=/local

ifneq ($V,1)
Q ?= @
endif

CC	= gcc
CFLAGS	= $(DEBUG) -Wall -Wextra $(INCLUDE) -Winline -pipe 

LDFLAGS	= -L$(DESTDIR)$(PREFIX)/lib
LIBS    = -lpthread -lrt -lm -lcrypt

SRC	=	src/4rel4in.c src/comm.c src/thread.c src/cfg485.c src/relay.c src/input.c

OBJ	=	$(SRC:.c=.o)

all:	4rel4in

4rel4in:	$(OBJ)
	$Q echo [Link]
	$Q $(CC) -o $@ $(OBJ) $(LDFLAGS) $(LIBS)

.c.o:
	$Q echo [Compile] $<
	$Q $(CC) -c $(CFLAGS) $< -o $@

.PHONY:	clean
clean:
	$Q echo "[Clean]"
	$Q rm -f $(OBJ) 4rel4in *~ core tags *.bak

.PHONY:	install
install: 4rel4in
	$Q echo "[Install]"
	$Q cp 4rel4in		$(DESTDIR)$(PREFIX)/bin
ifneq ($(WIRINGPI_SUID),0)
	$Q chown root:root	$(DESTDIR)$(PREFIX)/bin/4rel4in
	$Q chmod 4755		$(DESTDIR)$(PREFIX)/bin/4rel4in
endif


.PHONY:	uninstall
uninstall:
	$Q echo "[UnInstall]"
	$Q rm -f $(DESTDIR)$(PREFIX)/bin/4rel4in
	$Q rm -f $(DESTDIR)$(PREFIX)/man/man1/4rel4in.1
