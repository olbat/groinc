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

#include <stdio.h>
#include "network/headers.h"

struct linked_list_dsp_pkt_value
{
	void (*func_dsp_pkt)(FILE *, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram);
};

struct linked_list_dsp_rpt_value
{
	void (*func_dsp_rpt)(FILE *, __u8 *);
	__u8 *val;
};


void display_packet(FILE *fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram);
void display_report(FILE *fd);

void dsp_pkt_packets(FILE *fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram);
void dsp_pkt_dlproto(FILE *fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram);
void dsp_pkt_nlproto(FILE *fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram);
void dsp_pkt_tlproto(FILE *fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram);
void dsp_pkt_simple(FILE *fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram);
void dsp_pkt_header(FILE *fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram);
void dsp_pkt_data(FILE *fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram);
void dsp_pkt_hexa(FILE *fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram);
void dsp_pkt_allpackets(FILE *fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram);
void dsp_rpt_default(FILE *fd, __u8 *val);
void dsp_rpt_timetot(FILE *fd, __u8 *);
void dsp_rpt_countpacketstot(FILE *fd, __u8 *val);
void dsp_rpt_countpacketsfiltred(FILE *fd, __u8 *val);

#endif
