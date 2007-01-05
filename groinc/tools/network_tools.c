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

#include "network_tools.h"
#include "memory_tools.h"
#include "math_tools.h"
#include <netinet/in.h>
#include <malloc.h>

char *ipv4_ntoa(__u32 longip)
{
	int i,tot,current;
	char *end,**preend;
	
	preend = (char **) malloc(4*sizeof(char *));
	tot = 0;
	for (i = 0;i < 4;i++)
	{
		tot = tot + my_strlen(preend[i] = my_itoa((unsigned char)(longip>>(8*i)),10));
	}

	end = (char *) malloc(sizeof(char) * (tot + 4)); /* the 3 '.' and the final '\0' */
	
	my_memset(end,0,sizeof(end));
	end[tot + 3] = '\0';
	tot = 0;
	current = 0;
	i = 4;
	
	while (i--)
	{
		my_memcpy((end + tot),preend[i],current = my_strlen(preend[i]));
		if (i)
		{
			tot = tot + current;
			end[tot++] = '.';
		}
	}
	free(preend);
	return end;
}

__u32 ipv4_aton(char *str)
{
	__u32 end;
	char *ptr;
	end = 0;
	ptr = str;
	while (*str)
	{
		if (*str == '.')
		{
			end = end + (unsigned char) my_atoi_len(ptr,(str - ptr),10);
			end = end<<8;
			ptr = str + 1;
		}
		str++;
	}
	end = end + (unsigned char)my_atoi_len(ptr,(str - ptr),10);
	return htonl(end);
}

__u8 *mac_aton(char *str)
{
	__u8 *end, *p;
	char *ptr;
	
	end = (__u8 *) malloc(6*sizeof(__u8));
	my_memset((char *)end,0,8);
	p = end;
	ptr = str;
	while(*str)
	{
		if (*str == ':')
		{
			*p++ = (__u8) my_atoi_len(ptr,(str - ptr),16);
			ptr = str + 1;
		}
		str++;
	}
	*p = (__u8) my_atoi_len(ptr,(str - ptr),16);
	return end;
}

char *mac_ntoa(__u8 *macaddr)
{
	char *end,*ptr,*tmp;
	char i;

	end = (char *) malloc((3 * 6 + 1)*sizeof(char));
	ptr = end;
	for (i=0;i<5;i++)
	{
		tmp = my_itoa_zero(*macaddr++,2,16);
		*ptr++ = *tmp++;
		*ptr++ = *tmp;
		*ptr++ = ':';
	}
	tmp = my_itoa_zero(*macaddr,2,16);
	*ptr++ = *tmp++;
	*ptr++ = *tmp;
	*ptr = '\0';
	return end;
}

int mac_cmp(__u8 *macaddr1, __u8 *macaddr2)
{
	int end;
	char i;

	end = 0;
	i = 5;
	while ((!end) && (i))
	{
		i--;
		end = (*macaddr1++ != *macaddr2++);
	}
	if (i)
	{
		macaddr1--;
		macaddr2--;
	}
	if (*macaddr1 != *macaddr2)
	{
		if (*macaddr1 > *macaddr2)
			end = 1;
		else
			end = -1;
	}
	return end;
}

int mac_null(__u8 *macaddr)
{
	int end;
	unsigned char i;
	
	end = 1;
	i = 6;
	while ((i) && (!*macaddr++))
		i--;
	if (i)
		end = 0;
	return end;
}

