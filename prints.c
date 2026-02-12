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

#include "prints.h"
#include <stdio.h>
#include <stdarg.h>

#define PRINT_HEXA_BYTES_PER_BLOC 8
#define PRINT_HEXA_BYTES_PER_LINE 16
#define PRINT_SEPARATOR "--------"

void print_hexa(FILE *fd, char *data, int size)
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
				fprintf(fd,"\n%04d ",j);
				j = j + PRINT_HEXA_BYTES_PER_LINE;
			}
			else
			{
				fprintf(fd," ");
			}
		}
		fprintf(fd," %02x",(unsigned char)*data++);
		i++;
	}
}


void print_str(FILE *fd, unsigned int nb, char *str, ...)
{
	fputs(str,fd);
	if (nb > 0)
	{
		va_list ap;
		va_start(ap,str);

		while (nb--)
			fputs(va_arg(ap,char *),fd);

		va_end(ap);
	}
}

void print_format(FILE *fd, char *format, ...)
{
	va_list ap;
	va_start(ap,format);
	vfprintf(fd,format,ap);
	va_end(ap);
}

void print_separator(FILE *fd)
{
	fputs(PRINT_SEPARATOR,fd);
}

void print_newline(FILE *fd)
{
	fputc('\n',fd);
	/*fflush(fd);*/
}

void print_error(FILE *fd, int nb, char *str, ...)
{
	va_list ap;
	va_start(ap,str);
	fputs(str,fd);
	while (--nb)
		fputs(va_arg(ap,char *),fd);
	va_end(ap);
}

