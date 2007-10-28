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

#ifndef _NETWORK_TOOLS_H
#define _NETWORK_TOOLS_H

#include <asm/types.h>

#define MAC_STR_SIZE 19
#define IPV4_STR_MAXSIZE 16

char *ipv4_ntoa(__u32 longip, char *buff);
__u32 ipv4_aton(char *str);
__u8 *mac_aton(char *str, __u8 *buff);
char *mac_ntoa(__u8 *macaddr, char *buff);
int mac_cmp(__u8 *mac1, __u8 *mac2);
int mac_null(__u8 *macaddr);

#endif

