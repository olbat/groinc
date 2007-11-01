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

#include <asm/types.h>

#include "headers.h"

#ifndef _PRINTERS_H
#define _PRINTERS_H

/*
void print_ether(int fd, struct ethernet_header *etherh);
void print_ipv4(int fd,struct ipv4_header *iph);
void print_tcp(int fd,struct tcp_header *tcph);
void print_udp(int fd, struct udp_header *udph);
*/
void print_unknown(int fd,char *layer,char *protoname);
void print_datalink_layer(int fd, struct protocol_header *datalink_layerph);
void print_network_layer(int fd, struct protocol_header *network_layerph);
void print_transport_layer(int fd, struct protocol_header *transport_layerph);
__inline__ void print_data(int fd,struct data *data);
__inline__ void print_simple(int fd,struct protocol_header *datalink_layerph,struct protocol_header *network_layerph,struct protocol_header *transport_layerph);
void print_datalink_layer_proto(int fd,struct protocol_header *datalink_layerph);
void print_network_layer_proto(int fd,struct protocol_header *datalink_layerph,struct protocol_header *network_layerph);
void print_transport_layer_proto(int fd,struct protocol_header *network_layerph,struct protocol_header *transport_layerph);
__inline__ void print_protoproto(int fd,struct protocol_header *datalink_layerph);
__inline__ void print_ethproto(int fd,struct protocol_header *network_layerph);
__inline__ void print_ipproto(int fd,struct protocol_header *transport_layerph);
__inline__ void print_proto_simple(int fd,struct protocol_header *datalink_layerph,struct protocol_header *network_layerph,struct protocol_header *transport_layerph);
__inline__ void print_packetnb(int fd, int size);

#endif
