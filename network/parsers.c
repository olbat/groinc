/* This file is a part of groinc
 * 
 * Copyright (C) 2006, 2007 Sarzyniec Luc <olbat@xiato.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 * see the COPYING file for more informations */

#include <netinet/in.h>
#include "parsers.h"

void parse_datalink_layer(struct data *datagram,struct protocol_header *datalink_layerph,struct protocol_header *network_layerph)
{
	datalink_layerph->id = PROTO_ETHER;

	datalink_layerph->header = (datagram->data + datagram->len);
	switch (datalink_layerph->id)
	{
		case PROTO_ETHER :
			datalink_layerph->len = sizeof(struct ethernet_header);
			network_layerph->id = ((struct ethernet_header *)datalink_layerph->header)->proto;
			break;
	}
	datagram->len = (datagram->len + datalink_layerph->len);
}

void parse_network_layer(struct data *datagram,struct protocol_header *network_layerph,struct protocol_header *transport_layerph)
{
	network_layerph->header = (datagram->data + datagram->len);
	switch (network_layerph->id)
	{
		case ETHPROTO_IP :
			network_layerph->len = (((struct ipv4_header *)network_layerph->header)->iphdrlen * 4);
			transport_layerph->id = ((struct ipv4_header *)network_layerph->header)->proto;
			break;
	}
	datagram->len = (datagram->len + network_layerph->len);
} 

void parse_transport_layer(struct data *datagram,struct protocol_header *transport_layerph)
{
	transport_layerph->header = (datagram->data + datagram->len);
	switch (transport_layerph->id)
	{
		case IPPROTO_TCP :
			transport_layerph->len = (((struct tcp_header *)transport_layerph->header)->tcphdrlen * 4);
			break;
		case IPPROTO_UDP :
			transport_layerph->len = sizeof(struct udp_header);
			break;
	}
	datagram->len = (datagram->len + transport_layerph->len);
}
void get_ports(struct protocol_header *transport_layerph, __u16 *sourceport, __u16 *destport)	
{
	*sourceport = ntohs(*((__u16 *)transport_layerph->header));
	*destport = ntohs(*((__u16 *)(transport_layerph->header + sizeof(__u16))));
}

