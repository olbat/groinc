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

#include <netinet/in.h>
#include "parsers.h"
#include "../protocols/scan_proto.h"

void parse_datalink_layer(struct data *datagram,struct protocol_header *datalink_layerph,struct protocol_header *network_layerph)
{
	void (*ptr)(struct data *, struct protocol_header *, struct protocol_header *);
/* >>> TODO: make the datalink layer parsing system work for all protocols */
	datalink_layerph->id = PROTO_ETHER;

	if ((ptr = lookup_protoscan(datalink_layerph->id)))
	{
		datalink_layerph->header = (datagram->data + datagram->len);
		(*ptr)(datagram,datalink_layerph,network_layerph);
		if ((datagram->len + datalink_layerph->len) < datagram->totlen)
			datagram->len = (datagram->len + datalink_layerph->len);
	}
}

void parse_network_layer(struct data *datagram,struct protocol_header *network_layerph,struct protocol_header *transport_layerph)
{
	void (*ptr)(struct data *, struct protocol_header *, struct protocol_header *);

	if ((ptr = lookup_ethscan(network_layerph->id)))
	{
		network_layerph->header = (datagram->data + datagram->len);
		(*ptr)(datagram,network_layerph,transport_layerph);
		if ((datagram->len + network_layerph->len) < datagram->totlen)
			datagram->len = (datagram->len + network_layerph->len);
	}
} 

void parse_transport_layer(struct data *datagram,struct protocol_header *transport_layerph)
{
	void (*ptr)(struct data *, struct protocol_header *, struct protocol_header *);

	if ((ptr = lookup_ipscan(transport_layerph->id)))
	{
		transport_layerph->header = (datagram->data + datagram->len);
		(*lookup_ipscan(transport_layerph->id))(datagram,transport_layerph,0);
		if ((datagram->len + transport_layerph->len) < datagram->totlen)
			datagram->len = (datagram->len + transport_layerph->len);
	}
}

__inline__ __u16 get_source_port(struct protocol_header *transport_layerph)
{
	return ntohs(*((__u16 *)transport_layerph->header));
}

__inline__ __u16 get_dest_port(struct protocol_header *transport_layerph)
{
	return ntohs(*((__u16 *)(transport_layerph->header + sizeof(__u16))));
}
