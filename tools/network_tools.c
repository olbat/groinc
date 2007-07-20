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

#include "network_tools.h"
#include "memory_tools.h"
#include "math_tools.h"
#include <netinet/in.h>

char *ipv4_ntoa(__u32 longip, char *buff)
{
	int i, tot, tmplen;

	tot = 0;
	i = 4;
	while (i--)
	{
		char tmp[tmplen = my_itoa_buffer_size((unsigned char)(longip>>(8*i)),10)];
		my_memcpy((buff + tot),my_itoa((unsigned char)(longip>>(8*i)),10,tmp),(tmplen - 1));
		tot = tot + (tmplen - 1);
		if (i)
			buff[tot++] = '.';
	}
	buff[tot] = 0;

	return buff;
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

__u8 *mac_aton(char *str, __u8 *buff)
{
	__u8 *p;
	char *ptr;
	
	/* 
	end = (__u8 *) malloc(6*sizeof(__u8));
	my_memset((char *)end,0,8);
	*/

	p = buff;
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
	
	return buff;
}

char *mac_ntoa(__u8 *macaddr, char *buff)
{
	char *ptr;
	char tmp[2];
	char i;

	ptr = buff;
	for (i=0;i < 5;i++)
	{
		my_itoa_char(*macaddr++,2,16,'0',tmp);
		*ptr++ = *tmp;
		*ptr++ = *(tmp + 1);
		*ptr++ = ':';
	}

	my_itoa_char(*macaddr,2,16,'0',tmp);
	*ptr++ = *tmp;
	*ptr++ = *(tmp + 1);
	*ptr = 0;

	return buff;
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

