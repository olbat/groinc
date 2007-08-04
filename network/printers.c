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

#include <stdio.h>
#include <stdarg.h>
#include <netinet/in.h>
#include "printers.h"
#include "protocols.h"
#include "../globals_filter.h"
#include "../globals_args.h"
#include "../tools/network_tools.h"
#include "parsers.h"
#include "../protocols/print_proto.h"

void print_unknown(int fd,char *layer,char *protoname)
{
	if (*protoname)
		fprintf((FILE *)fd,"[%s protocol/%s layer]",protoname,layer);
	else
		fprintf((FILE *)fd,"[Unknown protocol/%s layer]",layer);
}

void print_datalink_layer(int fd, struct protocol_header *datalink_layerph)
{
	if (datalink_layerph->len >= 0)
	{
		(*lookup_protoprint(datalink_layerph->id))(fd,datalink_layerph->header);
		/* >>> TODO: make the print_unknown work with the new structure */

		/*
		switch(datalink_layerph->id)
		{
			case PROTO_ETHER :
				print_ether(fd,datalink_layerph->header);
				break;
			default :
				print_unknown(fd,"datalink",lookup_protoname(datalink_layerph->id));
				break;
		}
		*/
	}
}

void print_network_layer(int fd, struct protocol_header *network_layerph)
{
	if (network_layerph->len >= 0)
	{
		(*lookup_ethprint(network_layerph->id))(fd,network_layerph->header);
		/* >>> TODO: make the print_unknown work with the new structure */

		
		/*
		switch (network_layerph->id)
		{
			case ETHPROTO_IP :
				print_ipv4(fd,network_layerph->header);
				break;
			case ETHPROTO_ARP :
				print_arp(fd,network_layerph->header);
				break;
			default :
				print_unknown(fd,"network",lookup_ethname(network_layerph->id));
				break;
		}
		*/
	}
}

void print_transport_layer(int fd, struct protocol_header *transport_layerph)
{
	if (transport_layerph->len >= 0)
	{
		(*lookup_ipprint(transport_layerph->id))(fd,transport_layerph->header);
		/* >>> TODO: make the print_unknown work with the new structure */

		/*
		switch (transport_layerph->id)
		{
			case IPPROTO_ICMP :
				print_icmp(fd,transport_layerph->header);
				break;
			case IPPROTO_TCP :
				print_tcp(fd,transport_layerph->header);
				break;
			case IPPROTO_UDP :
				print_udp(fd,transport_layerph->header);
				break;
			default :
		>>>		print_unknown(fd,"transport",lookup_ipname(transport_layerph->id));
				break;
		}
		*/
	}
}

void print_data(int fd,struct data *data)
{
	fwrite((data->data + data->len),sizeof(char),(data->totlen - data->len),(FILE *)fd);
	/*write(fd,(data->data + data->len),(data->totlen - data->len));
	fflush((FILE *)fd);*/
}

void print_simple(int fd,struct protocol_header *network_layerph,struct protocol_header *transport_layerph)
{
	if (network_layerph->id == ETHPROTO_IP)
		print_ipv4_simple(fd,network_layerph->header,get_source_port(transport_layerph),get_dest_port(transport_layerph));
}

void print_datalink_layer_proto(int fd,struct protocol_header *datalink_layerph)
{
	if (*protoname)
		fprintf((FILE *)fd,"[%s %d]",protoname,datalink_layerph->len);
	else
		fprintf((FILE *)fd,"[%s %d]",lookup_protoname(datalink_layerph->id),datalink_layerph->len);
}

void print_network_layer_proto(int fd,struct protocol_header *datalink_layerph,struct protocol_header *network_layerph)
{
	switch (datalink_layerph->id)
	{
		case PROTO_ETHER :
			print_ethproto(fd,network_layerph);
			break;
	}
}
void print_transport_layer_proto(int fd,struct protocol_header *network_layerph,struct protocol_header *transport_layerph)
{
	switch (network_layerph->id)
	{
		case ETHPROTO_IP :
			print_ipproto(fd,transport_layerph);
			break;
	}
}

void print_protoproto(int fd,struct protocol_header *datalink_layerph)
{
	if (*protoname)
		fprintf((FILE *)fd,"[%s %d]",protoname,datalink_layerph->len);
	else
		fprintf((FILE *)fd,"[%s %d]",lookup_protoname(datalink_layerph->id),datalink_layerph->len);
}

void print_ethproto(int fd,struct protocol_header *network_layerph)
{
	if (*ethprotoname)
		fprintf((FILE *)fd,"[%s %d]",ethprotoname,network_layerph->len);
	else
		fprintf((FILE *)fd,"[%s %d]",lookup_ethname(network_layerph->id),network_layerph->len);
}

void print_ipproto(int fd,struct protocol_header *transport_layerph)
{
	if (*ipprotoname)
		fprintf((FILE *)fd,"[%s %d]",ipprotoname,transport_layerph->len);
	else
		fprintf((FILE *)fd,"[%s %d]",lookup_ipname(transport_layerph->id),transport_layerph->len);
}

void print_packetnb(int fd, int size)
{
	fprintf((FILE *)fd,"[%d]",size);
	/*fflush((FILE *)fd);*/
}

