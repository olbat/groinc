# This file is a part of groinc 
# Copyright 2006 Sarzyniec Luc <olbat@xiato.com>
# This software is released under GPL the license
# see the COPYING file for more informations

CFLAGS= -Wall -ansi -pedantic -O3 -fomit-frame-pointer -g
CC= gcc ${CFLAGS} 
SRC= printp.c namescache.c filter_proto.c
OBJ= $(SRC:.c=.o)

all : ${OBJ} protocols
	@echo success compiling protocols
%.o : %.c
	@echo -n 'compiling $< ... '
	@${CC} -o $@ -c $<
	@echo done
protocols :
	@echo 'start compilation of protocol modules'
	@for proto in proto_*.c ; do  echo -n 'compiling protocol module '$${proto}' ... ' ; ${CC} -o `basename $${proto} .c`.o -c $${proto} ; echo done ; done;
	@echo 'protocol modules compiled'
clean :
	@echo cleaning object files
	@rm -f ${OBJ} 
	@for proto in proto_*.o ; do rm -f $${proto} ; done;

