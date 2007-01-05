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

#include "math_tools.h"
#include "memory_tools.h"
#include <malloc.h>

static const char *lbase = { "0123456789abcdefghijklmnopqrstuvwxyz" };
static const char *ubase = { "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
static const char nbase = 36;

int my_ctoi(char c)
{
	char end = -1;
	char *ptr,*base;
	base = (char *) lbase;
	ptr = base;
	while((*ptr) && (*ptr != c))
		ptr++;
	
	if (!*ptr)
	{
		base = (char *) ubase;
		ptr = base;
		while ((*ptr) && (*ptr != c))
			ptr++;
	}
	
	if (*ptr)
	{
		end = ptr - base;
	}
		
	return end;	
}

char my_itoc(unsigned int number)
{
	return ubase[number % nbase];
}

unsigned int my_pow(unsigned int n, unsigned int p)
{
	unsigned int end;
	end = 1;
	/* WARNING : risk of overflow */
	while (p > 0)
	{
		if (p % 2)
		{
			p--;
			end = end * n;
		}
		p >>= 1;
		n *= n;
	}
	return end;
}

char *my_itoa(unsigned int number, unsigned char base)
{
	char *end;
	unsigned int order,tmp,pow;

	order = 1;
	tmp = number;
	pow = 1;

	/* getting the order of the number */
	while (pow * base <= number)
	{
		order++;
		pow = pow * base;
	}

	/* initialize the returned string */	
	end = (char *) malloc(sizeof(char)*(order + 1));
	end[order] = '\0';

	/* reuse of the variable order */
	order = 0;

	while(pow)
	{
		tmp = number / pow; 
		end[order++] = my_itoc(tmp);
		number = number - (tmp * pow);
		pow /= base;
	}
	return end;
}

char *my_itoa_zero(unsigned int number, unsigned char zeros,unsigned char base)
{
	char *end,*tmp;
	unsigned int len;
	
	tmp = my_itoa(number,base);
	len = my_strlen(tmp); 
	if (len < zeros)
		zeros = zeros - len;
	else
		zeros = 0;

	end = (char *) malloc((len + zeros) * sizeof(char));
	if (zeros)
		my_memset(end,'0',zeros);
	my_memcpy(end + zeros,tmp,len);
	
	return end;
}
int my_atoi(char *str, unsigned char base)
{
	int end,nb,pow;
	nb = 0;
	
	while ((*str) && (my_ctoi(*str) >= 0))
	{	nb++;
		str++;
	}
	
	pow = 1;
	end = 0;
	while (nb--)
	{
		end = end + ((my_ctoi(*--str) % base) * pow);
		pow *= base;
	}
	return end;
}

int my_atoi_len(char *str, unsigned int len, unsigned char base)
{
	int end,nb,pow;
	
	nb = len;
	while ((*str) && (my_ctoi(*str) >= 0))
	{
		str++;
		len--;
	}
	nb = nb - len;
	pow = 1;
	end = 0;
	while (nb--)
	{
		end = end + ((my_ctoi(*--str) % base) * pow);
		pow *= base;
	}
	return end;
}

