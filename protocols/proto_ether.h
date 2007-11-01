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

#ifndef _PROTO_ETHER_H
#define _PROTO_ETHER_H

#include <asm/types.h>

#include "../network/headers.h"

/*
 * For further informations about this implementation please take a look to the following documents :
 * 	Ethernet IEEE 802.3 standard (http://standards.ieee.org/getieee802/802.3.html)
 * 	RFC 894 - A Standard for the Transmission of IP Datagrams over Ethernet Networks (http://ietf.org/rfc/rfc894.txt)
 */

struct ethernet_header
{
	__u8	destmac[6];
	__u8	sourcemac[6];
	__u16	proto;
};

void scan_ether(struct data *datagram, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph);
void print_ether(int fd, char *datagram);
void print_ether_simple(int fd, char *datagram);

#endif
