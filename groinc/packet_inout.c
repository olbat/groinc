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

#include "globals_option.h"
#include <unistd.h>

int read_packet(int fd,char *buffer, int size)
{
	int end;
	if (*inputfile)
	{
		short tmp;
		end = read(fd,&tmp,sizeof(short));
		if (end > 0)
		{
			if (tmp <= size)
				end = read(fd,buffer,tmp);
			else
				end = read(fd,buffer,size);
		}
	}
	else
	{
		end = read(fd,buffer,size);
	}
	return end;
}

/* The format of the output file : a short integer for the size of the packet data then, the packet data */
int write_packet(int fd,char *buffer,short size)
{
	int end;
	end = 0;
	if (*outputfile)
	{
		end = write(fd,&size,sizeof(short));
		if (end > 0)
			end = write(fd,buffer,size);
	}
	return end;
}

