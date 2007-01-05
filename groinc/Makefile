# This file is a part of groinc 
# Copyright 2006 Sarzyniec Luc <olbat@xiato.com>
# This software is released under GPL the license
# see the COPYING file for more informations

CFLAGS= -Wall -O3 -g
CC= gcc ${CFLAGS} 
SNAME= groinc
SRC= main.c sniffer.c usage.c parse_options.c check_options.c filter.c prints.c packet_inout.c tools/memory_tools.c tools/math_tools.c tools/network_tools.c network/protocols.c network/parsers.c 
OBJ= $(SRC:.c=.o)

all : ${SNAME} clean
	@echo success making ${SNAME}
obj : ${SNAME}
	@echo success making ${SNAME}
${SNAME} : ${OBJ}
	@echo making ${SNAME}
	@${CC} -o $@ $^
%.o : %.c
	@echo -n 'compiling $< ... '
	@${CC} -o $@ -c $<
	@echo done
install :
	cp -f ${SNAME} /usr/sbin
	chmod 755 /usr/bin/${SNAME}
clean :
	@echo cleaning object files
	@rm -f *.o network/*.o tools/*.o
cleanall : clean
	@echo cleaning executable file
	@rm -f ${SNAME}
targz :
	@echo making compressed archive tar.gz
	@tar -zcvf ${SNAME}.tar.gz  ${SRC} *.h network/*.h tools/*.h Makefile COPYING 
