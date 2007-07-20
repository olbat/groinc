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

#ifndef _PROTO_ICMP_H
#define _PROTO_ICMP_H

#include "../network/my_types.h"
#include "../network/headers.h"

/*
 * For further informations about this implementation please take a look to the following RFC :
 * 	RFC 792 - INTERNET CONTROL MESSAGE PROTOCOL (http://ietf.org/rfc/rfc792.txt)
 */

struct icmp_header
{
	__u8 type;
	__u8 code;
	__u16 icmpchecksum;
	__u32 field;
};

void scan_icmp(struct data *datagram, struct protocol_header *transport_layerph);
void print_icmp(int fd, char *datagram);

#endif
