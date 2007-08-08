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

#ifndef _FILTER_H
#define _FILTER_H

#include "network/my_types.h"
#include "network/headers.h"

#define FLT_ERROR -1

struct linked_list_flt_value
{
	int (*func_flt)(struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, __u8 *val);
	__u8 *val;
};

int filter_string(char *data);
int filter(struct protocol_header *datalink_layerph,struct protocol_header *network_layerph,struct protocol_header *transport_layerph,struct data *datagram);

int filter_string(char *data);
int filter_regex(char *data);
__inline__ int flt_dl_mac_src(struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, __u8 *flt_val);
__inline__ int flt_dl_mac_dst(struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, __u8 *flt_val);
__inline__ int flt_nl_ip_src(struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, __u8 *flt_val);
__inline__ int flt_nl_ip_dst(struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, __u8 *flt_val);
__inline__ int flt_nl_ip_global(struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, __u8 *flt_val);
__inline__ int flt_tl_port_src(struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, __u8 *flt_val);
__inline__ int flt_tl_port_dst(struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, __u8 *flt_val);
__inline__ int flt_tl_port_global(struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, __u8 *flt_val);
__inline__ int flt_dl_protocol(struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, __u8 *flt_val);
__inline__ int flt_nl_protocol(struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, __u8 *flt_val);
__inline__ int flt_tl_protocol(struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, __u8 *flt_val);

#endif
