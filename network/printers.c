/* This file is a part of groinc
 * 
 * Copyright (C) 2006-2008 Sarzyniec Luc <olbat@xiato.net>
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
#include <unistd.h>
#include <ctype.h>
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
	fprintf((FILE *)fd,"[Unknown protocol/%s layer]",layer);
}

void print_datalink_layer(int fd, struct protocol_header *datalink_layerph)
{
	if (datalink_layerph->len >= 0)
	{
		void (*ptr)(int, char *); 
		if ((ptr = lookup_protoprint(datalink_layerph->id)))
			(*ptr)(fd,datalink_layerph->header);
		else
			print_unknown(fd,"datalink",lookup_protoname(datalink_layerph->id));
	}
}

void print_network_layer(int fd, struct protocol_header *network_layerph)
{
	if (network_layerph->len >= 0)
	{
		void (*ptr)(int, char *); 
		if ((ptr = lookup_ethprint(network_layerph->id)))
			(*ptr)(fd,network_layerph->header);
		else
			print_unknown(fd,"network",lookup_ethname(network_layerph->id));
	}
}

void print_transport_layer(int fd, struct protocol_header *transport_layerph)
{
	if (transport_layerph->len >= 0)
	{
		void (*ptr)(int, char *); 
		if ((ptr = lookup_ipprint(transport_layerph->id)))
			(*ptr)(fd,transport_layerph->header);
		else
			print_unknown(fd,"transport",lookup_ipname(transport_layerph->id));
	}
}

__inline__ void print_data(int fd,struct data *data)
{
	int len;
	char *ptr;

	len = data->totlen - data->len;
	ptr = data->data + data->len;

	while (len--)
	{
		if((!isprint(*ptr)) && (*ptr != '\t') && (*ptr != '\n') && (*ptr != '\r'))
			*ptr = '.';
		ptr++;
	}
	fwrite((data->data + data->len),sizeof(char),(data->totlen - data->len),(FILE *)fd);
	fflush((FILE *)fd);
}

__inline__ void print_simple(int fd,struct protocol_header *datalink_layerph,struct protocol_header *network_layerph,struct protocol_header *transport_layerph)
{
	if ((transport_layerph->len > 0) && (network_layerph->id == ETHPROTO_IP))
		print_ipv4_simple(fd,network_layerph->header,get_source_port(transport_layerph),get_dest_port(transport_layerph));
	/*
	 * changements will be necessary if other protocols than ethernet
	 * protocol for datalink layer is available
	 */
	else
		print_ether_simple(fd,datalink_layerph->header);
}

void print_datalink_layer_proto(int fd,struct protocol_header *datalink_layerph)
{
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

__inline__ void print_protoproto(int fd,struct protocol_header *datalink_layerph)
{
	fprintf((FILE *)fd,"[%s %d]",lookup_protoname(datalink_layerph->id),datalink_layerph->len);
}

__inline__ void print_ethproto(int fd,struct protocol_header *network_layerph)
{
	fprintf((FILE *)fd,"[%s %d]",lookup_ethname(network_layerph->id),network_layerph->len);
}

__inline__ void print_ipproto(int fd,struct protocol_header *transport_layerph)
{
	fprintf((FILE *)fd,"[%s %d]",lookup_ipname(transport_layerph->id),transport_layerph->len);
}

__inline__ void print_proto_simple(int fd,struct protocol_header *datalink_layerph,struct protocol_header *network_layerph,struct protocol_header *transport_layerph)
{
	/*
	 * modifications will be necessary if other than ehternet 
	 * protocol on datalink layer is usable
	 */
	fprintf((FILE *)fd,"[%s]",
		((transport_layerph->len >= 0) 
		&& (network_layerph->id == ETHPROTO_IP)
			? lookup_ipname(transport_layerph->id)
			: (network_layerph->len >= 0 
				? lookup_ethname(network_layerph->id)
				: lookup_protoname(datalink_layerph->id))));
}

__inline__ void print_packetnb(int fd, int size)
{
	fprintf((FILE *)fd,"[%d]",size);
	/*fflush((FILE *)fd);*/
}

