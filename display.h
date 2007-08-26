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

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "network/headers.h"

struct linked_list_dsp_pkt_value
{
	void (*func_dsp_pkt)(int, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram);
};

struct linked_list_dsp_rpt_value
{
	void (*func_dsp_rpt)(int, __u8 *);
	__u8 *val;
};


void display_packet(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram);
void display_report(int fd);

__inline__ void dsp_pkt_packets(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram);
__inline__ void dsp_pkt_dlproto(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram);
__inline__ void dsp_pkt_nlproto(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram);
__inline__ void dsp_pkt_tlproto(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram);
__inline__ void dsp_pkt_simple(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram);
__inline__ void dsp_pkt_header(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram);
__inline__ void dsp_pkt_data(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram);
__inline__ void dsp_pkt_hexa(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram);
__inline__ void dsp_pkt_allpackets(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram);
void dsp_rpt_timetot(int fd, __u8 *);

#endif
