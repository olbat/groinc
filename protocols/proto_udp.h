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
 * For further informations about this implementation please take a look
 * to the following RFC :
 * 	RFC 768 - User Datagram Protocol (http://ietf.org/rfc/rfc768.txt)
 */

#ifndef _PROTO_UDP_H
#define _PROTO_UDP_H

#include <asm/types.h>

#include "../network/headers.h"

struct udp_header
{
	__u16 sourceport;
	__u16 destport;
	__u16 udpdatalen;
	__u16 udpchecksum;
};

void scan_udp(struct data *datagram, struct protocol_header *transport_layerph,
	struct protocol_header *application_layerph);
void print_udp(int fd,  char *datagram);

#endif
