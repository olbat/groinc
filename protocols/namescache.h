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

#ifndef _NAMESCACHE_H
#define _NAMESCACHE_H

#include <asm/types.h>

#define NCHE_IPV4_SIZE	64

extern __u16	ncacheopt;

enum nche_opt
{
	NCHE_OPT_NORESOLV = 0x1
};

struct ncache_ipv4
{
	char *name;
	__u32 addr;
};

__inline__ void ncache_free();
char *get_ipv4_name(__u32 addr);

#endif
