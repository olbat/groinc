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

#include "display.h"
#include "globals_display.h"
#include "tools/compiler.h"
#include "network/headers.h"
#include "network/printers.h"
#include "prints.h"

void display(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram)
{
	struct linked_list *ptr = list_display;
	if (likely(ptr->value))
	{
		while (ptr)
		{
			(ptr->value->u.dsp->func_dsp)(fd,datalink_layerph,network_layerph,transport_layerph,datagram);
			ptr = ptr->next;
			print_newline(fd);
		}
	}
	else
	{
		dsp_simple(fd,datalink_layerph,network_layerph,transport_layerph,datagram);
		print_newline(fd);
		dsp_data(fd,datalink_layerph,network_layerph,transport_layerph,datagram);
		print_newline(fd);
	}
	print_separator(fd);
	print_newline(fd);
}

__inline__ void dsp_packets(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram)
{
	print_packetnb(fd,datagram->len);
}

__inline__ void dsp_dlproto(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram)
{
	print_datalink_layer_proto(fd,datalink_layerph);
}

__inline void dsp_nlproto(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram)
{
	print_network_layer_proto(fd,datalink_layerph,network_layerph);
}

__inline__ void dsp_tlproto(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram)
{
	print_transport_layer_proto(fd,network_layerph,transport_layerph);
}

__inline__ void dsp_simple(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram)
{
	print_ipproto(fd,transport_layerph);
	print_simple(fd,network_layerph,transport_layerph);
}

__inline__ void dsp_header(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram)
{
	print_datalink_layer(fd,datalink_layerph);
	print_network_layer(fd,network_layerph);
	print_transport_layer(fd,transport_layerph);
}

__inline__ void dsp_data(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram)
{
	print_data(datafd,datagram);
}

__inline__ void dsp_hexa(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram)
{
	print_protoproto(fd,datalink_layerph);
	print_hexa(fd,datalink_layerph->header,datalink_layerph->len);
	
	print_newline(fd);
	print_ethproto(fd,network_layerph);
	print_hexa(fd,network_layerph->header,network_layerph->len);
	
	print_newline(fd);
	print_ipproto(fd,transport_layerph);
	print_hexa(fd,transport_layerph->header,transport_layerph->len);
	print_newline(fd);
	
	print_newline(fd);
	print_packetnb(fd,(datagram->totlen - datagram->len));
	print_hexa(fd,(datagram->data + datagram->len),(datagram->totlen - datagram->len));
}

__inline__ void dsp_allpackets(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram)
{
	print_packetnb(fd,datagram->len);
}

