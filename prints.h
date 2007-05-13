/* This file is a part of groinc
 * Copyright 2006 Sarzyniec Luc <olbat@xiato.com>
 * This software is released under GPL the license
 * see the COPYING file for more informations */

#ifndef _PRINTS_H
#define _PRINTS_H

void print_hexa(int fd, char *data, int size);
void print_str(int fd, unsigned int nb, char *str, ...);
void print_format(int fd, char *format, ...);
void print_separator(int fd);
void print_newline(int fd);

#endif

