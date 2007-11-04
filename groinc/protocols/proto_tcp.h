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
 * 	RFC 793 - TRANSMISSION CONTROL PROTOCOL (http://ietf.org/rfc/rfc793.txt)
 */

#ifndef _PROTO_TCP_H
#define _PROTO_TCP_H

#include <asm/types.h>

#include "../network/headers.h"

struct tcp_header
{
	__u16 sourceport;
	__u16 destport;
	__u32 seqnum;
	__u32 acknum;
__extension__ __u16 res:4,
	      tcphdrlen:4,
	      fin:1,
	      syn:1,
	      rst:1,
	      psh:1,
	      ack:1,
	      urg:1,
	      ece:1,
	      ecn:1;
	__u16 window;
	__u16 tcpchecksum;
	__u16 urgptr;
};
void scan_tcp(struct data *datagram, struct protocol_header *transport_layerph,
	struct protocol_header *application_layerph);
void print_tcp(int fd, char *datagram);

#endif
