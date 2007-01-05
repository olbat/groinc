/* Copyright 2006 Sarzyniec Luc <olbat@xiato.com>
 * This software is released under GPL the license
 * see the COPYING file for more informations */

#ifndef _MEMORY_TOOLS_H
#define _MEMORY_TOOLS_H

char *my_memset(char *ma,char c,int size);
char *my_memcpy(char *dest,char *src,int size);
int my_strcmp(char *stra, char *strb);
int my_strlen(char *str);
char to_upper(char c);
char to_lower(char c);
char *strupr(char *str);
char *strlwr(char *str);
int findstr(char *str,char *filterstr);

#endif

