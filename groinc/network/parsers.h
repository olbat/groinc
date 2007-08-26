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

#ifndef _PARSERS_H
#define _PARSERS_H

#include "headers.h"
#include "protocols.h"

void parse_datalink_layer(struct data *datagram,struct protocol_header *datalink_layerph,struct protocol_header *network_layerph);
void parse_network_layer(struct data *datagram,struct protocol_header *network_layerph,struct protocol_header *transport_layerph);
void parse_transport_layer(struct data *datagram,struct protocol_header *transport_layerph);
__inline__ __u16 get_source_port(struct protocol_header *transport_layerph);
__inline__ __u16 get_dest_port(struct protocol_header *transport_layerph);

#endif

