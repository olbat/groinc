# This file is a part of groinc 
# Copyright 2006 Sarzyniec Luc <olbat@xiato.com>
# This software is released under GPL the license
# see the COPYING file for more informations

BINDIR= $(DESTDIR)/usr/bin
MANDIR= $(DESTDIR)/usr/share/man/man1
CFLAGS= -Wall -O3 -ansi -g
CC= gcc ${CFLAGS} 
SNAME= groinc
SRC= main.c usage.c misc.c parse_options.c check_options.c filter.c prints.c packet_inout.c events.c sniffer.c tools/memory_tools.c tools/math_tools.c tools/network_tools.c network/protocols.c network/parsers.c network/printers.c 
OBJ= $(SRC:.c=.o)

all : ${SNAME} clean
	@echo success making ${SNAME}
obj : ${SNAME}
	@echo success making ${SNAME}
${SNAME} : ${OBJ} protos
	@echo making ${SNAME}
	@${CC} -o $@ ${OBJ} protocols/*.o
%.o : %.c
	@echo -n 'compiling $< ... '
	@${CC} -o $@ -c $<
	@echo done
protos :
	@(cd protocols/; make;)
install :
	cp -f ${SNAME} ${BINDIR}
	chmod 755 ${BINDIR}/${SNAME}
	cp -f ${SNAME}.1 ${MANDIR}
	gzip -9 -f ${MANDIR}/${SNAME}.1
clean :
	@echo cleaning object files
	@rm -f ${OBJ}
	@(cd protocols; make clean;)
cleanall : clean
	@echo cleaning executable file
	@rm -f ${SNAME}
