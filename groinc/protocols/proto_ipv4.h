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

/*
 * For further informations about this implementation please take a look to the following RFC :
 * 	RFC 791 - INTERNET PROTOCOL (http://ietf.org/rfc/rfc791.txt)
 */

#ifndef _PROTO_IPV4_H
#define _PROTO_IPV4_H

#include "../network/my_types.h"
#include "../network/headers.h"

struct ipv4_header
{
__extension__ __u8 iphdrlen:4,
	     version:4;
	__u8 tos;
	__u16 totlen;
	__u16 id;
	__u16 fragoffset;
	__u8 ttl;
	__u8 proto;
	__u16 ipchecksum;
	__u32 sourceaddr;
	__u32 destaddr;
};

void scan_ipv4(struct data *datagram, struct protocol_header *network_layerph, struct protocol_header *transport_layerph);
void print_ipv4(int fd, char *datagram);
void print_ipv4_simple(int fd, char *datagram, __u16 sourceport, __u16 destport);

#endif

