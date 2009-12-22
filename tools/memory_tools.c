/* This file is a part of groinc
 * 
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

#include "memory_tools.h"

char to_upper(char c)
{
	return (c & 0xdf); /* 0xdf == ~0x20 */
}
char to_lower(char c)
{
	return (c | 0x20);
}
int strccount(char *str, char c)
{
	int i;
	i= 0;
	while (*str)
	{
		if (*str++ == c)
			i++;
	}
	return i;
}
char *strupr(char *str)
{
	char *end;
	end = str;
	while((*str++ = to_upper(*str)));
	return end;
}
char *strlwr(char *str)
{
	char *end;
	end = str;
	while((*str++ = to_lower(*str)));
	return end;
}

int findstr(char *str,char *filterstr)
{
	int end;
	char *ptr;
	end = 1;
	ptr = filterstr;
	while ((end) && (*str) && (*ptr))
	{
		if (*str == *ptr)
			ptr++;
		else if (ptr != filterstr)
			ptr = filterstr;
		str++;
	}
	if (*ptr)
		end = 0;
	return end;
}

