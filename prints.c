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

#include "prints.h"
#include <stdio.h>
#include <stdarg.h>

#define PRINT_HEXA_BYTES_PER_BLOC 8
#define PRINT_HEXA_BYTES_PER_LINE 16
#define PRINT_SEPARATOR "--------"

void print_hexa(int fd, char *data, int size)
{
	int i,j;
	i = 0;
	j = 0;
	while (i < size)
	{
		if (!(i % PRINT_HEXA_BYTES_PER_BLOC))
		{
			if (!(i % PRINT_HEXA_BYTES_PER_LINE))
			{
				fprintf((FILE *)fd,"\n%04d ",j);
				j = j + PRINT_HEXA_BYTES_PER_LINE;
			}
			else
			{
				fprintf((FILE *)fd," ");	
			}
		}
		fprintf((FILE *)fd," %02x",(unsigned char)*data++);
		i++;
	}
}

void print_str(int fd, unsigned int nb, char *str, ...)
{
	fputs(str,(FILE *)fd);
	if (nb > 0)
	{
		va_list ap;
		va_start(ap,str);
		while (nb--)
			fputs(va_arg(ap,char *),(FILE *)fd);
		va_end(ap);
	}
}

void print_format(int fd, char *format, ...)
{
	va_list ap;
	va_start(ap,format);
	vfprintf((FILE *)fd,format,ap);
	va_end(ap);
}

void print_separator(int fd)
{
	fputs(PRINT_SEPARATOR,(FILE *)fd);
}

void print_newline(int fd)
{
	fputc('\n',(FILE *)fd);
	/*fflush((FILE *)fd);*/
}

