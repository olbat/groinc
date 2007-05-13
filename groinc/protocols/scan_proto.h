/* This file is a part of groinc
 * Copyright 2006 Sarzyniec Luc <olbat@xiato.com>
 * This software is released under GPL the license
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
