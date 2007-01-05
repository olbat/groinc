/* This file is a part of groinc
 * Copyright 2006 Sarzyniec Luc <olbat@xiato.com>
 * This software is released under GPL the license
 * see the COPYING file for more informations */

#ifndef _PARSERS_H
#define _PARSERS_H

#include "headers.h"
#include "protocols.h"

void parse_datalink_layer(struct data *datagram,struct protocol_header *datalink_layerph,struct protocol_header *network_layerph);
void parse_network_layer(struct data *datagram,struct protocol_header *network_layerph,struct protocol_header *transport_layerph);
void parse_transport_layer(struct data *datagram,struct protocol_header *transport_layerph);
void get_ports(struct protocol_header *transport_layerph, __u16 *sourceport, __u16 *destport);

#endif

