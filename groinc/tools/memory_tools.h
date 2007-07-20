/*
 * Copyright (C) 2006, 2007 Sarzyniec Luc <olbat@xiato.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
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

