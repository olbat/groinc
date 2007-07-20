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

#ifndef _SCAN_PROTO_H
#define _SCAN_PROTO_H

#include "../network/headers.h"

void scan_ether(struct data *datagram, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph);
void scan_ipv4(struct data *datagram, struct protocol_header *network_layerph, struct protocol_header *transport_layerph);
void scan_arp(struct data *datagram, struct protocol_header *network_layerph, struct protocol_header *transport_layerph);
void scan_tcp(struct data *datagram, struct protocol_header *transport_layerph);
void scan_udp(struct data *datagram, struct protocol_header *transport_layerph);
void scan_icmp(struct data *datagram, struct protocol_header *transport_layerph);

#endif
