/* This file is a part of groinc
 * 
 * Copyright (C) 2006, 2007 Sarzyniec Luc <olbat@xiato.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 * see the COPYING file for more informations */

#include "memory_tools.h"

char *my_memset(char *ma,char c,int size)
{
	char *end = ma;
	while (size--)
	{
		*ma++ = c;
	}
	return end;
}

char *my_memcpy(char *dest,char *src,int size)
{
	char *end;
	end = dest;
	while (size--)
	{
		*dest++ = *src++;
	}
	return end;
}

int my_strlen(char *str)
{
	unsigned int end;
	end = 0;
	while (*str++)
	{
		end++;
	}
	return end;
}

int my_strcmp(char *str1, char *str2)
{
	int end;
	end = 0;
	while ((*str1) &&  (*str2) && (*str1 == *str2))
	{
		str1++;
		str2++;
	}
	if (*str1)
	{
		if (*str2)
		{
			if (*str1 > *str2)
				end = 1;
			else
				end = -1;
		}
		else
		{
			end = 1;
		}
	}
	else
	{
		if (*str2)
			end = -1;
	}
	return end;
}

char to_upper(char c)
{
	return (c & 0xdf); /* 0xdf = !0x20 */
}
char to_lower(char c)
{
	return (c | 0x20);
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
		else /* need to be optimized */
			ptr = filterstr;

		str++;
	}
	if (*ptr)
		end = 0;
	return end;
}

