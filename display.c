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
#include "globals_report.h"
#include "globals_filter.h"
#include "tools/compiler.h"
#include "network/headers.h"
#include "network/printers.h"
#include "prints.h"

#include <sys/time.h>

void display_packet(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram)
{
	struct linked_list *ptr = list_display_packet;
	if (likely(ptr->value))
	{
		while (ptr)
		{
			(ptr->value->u.dsp_pkt->func_dsp_pkt)(fd,datalink_layerph,network_layerph,transport_layerph,datagram);
			ptr = ptr->next;
			print_newline(fd);
		}
	}
	else
	{
		dsp_pkt_simple(fd,datalink_layerph,network_layerph,transport_layerph,datagram);
		print_newline(fd);
		dsp_pkt_data(fd,datalink_layerph,network_layerph,transport_layerph,datagram);
		print_newline(fd);
	}
	print_separator(fd);
	print_newline(fd);
}

void display_report(int fd)
{
	struct linked_list *ptr = list_display_report;
	if (likely(ptr->value))
	{
		print_newline(fd);
		while (ptr)
		{
			(ptr->value->u.dsp_rpt->func_dsp_rpt)(fd,ptr->value->u.dsp_rpt->val);
			ptr = ptr->next;
			print_newline(fd);
		}
	}
	else
	{
		/*
		dsp_rpt_countpacketstot(fd,val);
		dsp_rpt_countpacketsfiltred(fd,val);
		print_newline(fd);
		dsp_rpt_timetot(fd,val);
		*/
		print_newline(fd);
		dsp_rpt_default(fd,0);
		print_newline(fd);
	}
	print_newline(fd);
}

__inline__ void dsp_pkt_packets(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram)
{
	print_packetnb(fd,datagram->len);
}

__inline__ void dsp_pkt_dlproto(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram)
{
	print_datalink_layer_proto(fd,datalink_layerph);
}

__inline void dsp_pkt_nlproto(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram)
{
	print_network_layer_proto(fd,datalink_layerph,network_layerph);
}

__inline__ void dsp_pkt_tlproto(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram)
{
	print_transport_layer_proto(fd,network_layerph,transport_layerph);
}

__inline__ void dsp_pkt_simple(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram)
{
	print_ipproto_simple(fd,transport_layerph);
	print_simple(fd,network_layerph,transport_layerph);
}

__inline__ void dsp_pkt_header(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram)
{
	print_datalink_layer(fd,datalink_layerph);
	print_network_layer(fd,network_layerph);
	print_transport_layer(fd,transport_layerph);
}

__inline__ void dsp_pkt_data(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram)
{
	print_data(datafd,datagram);
}

__inline__ void dsp_pkt_hexa(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram)
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

__inline__ void dsp_pkt_allpackets(int fd, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram)
{
	print_packetnb(fd,datagram->len);
}

void dsp_rpt_timetot(int fd, __u8 *val)
{
	struct timeval timecur,*timesta;
	time_t totsec;
	suseconds_t totusec;
	
	timesta = (struct timeval *) val;
	gettimeofday(&timecur,0);

	totsec = (timecur.tv_sec - timesta->tv_sec);
	if ((totusec = (timecur.tv_usec - timesta->tv_usec)) < 0)
	{
		totsec--;
		totusec = (1000000 - timesta->tv_usec) + timecur.tv_usec;
	}

	print_format(fd, "[Time total: %hds %hdms]",totsec,(totusec / 1000));
}

#define DSP_RPT_COUNT_PACKETS_CAST(V) \
__extension__ \
({ \
	*((unsigned int *) *((unsigned int *) val)); \
})

void dsp_rpt_default(int fd, __u8 *val)
{
	struct timeval timecur;
	time_t totsec;
	suseconds_t totusec;

	gettimeofday(&timecur,0);
	
	totsec = (timecur.tv_sec - default_report.timestart.tv_sec);
	if ((totusec = (timecur.tv_usec - default_report.timestart.tv_usec)) < 0)
	{
		totsec--;
		totusec = (1000000 - default_report.timestart.tv_usec) + timecur.tv_usec;
	}

	print_format(fd,"Time - [total %hds %hdms]\n"
			"Packets - [total %hu packets] [filtred %hu packets]",
			totsec,(totusec / 1000),default_report.countpacketstot,default_report.countpacketsfiltred);
}

void dsp_rpt_countpacketstot(int fd, __u8 *val)
{
	print_format(fd,"[Packets total : %hu packets]",DSP_RPT_COUNT_PACKETS_CAST(val));
}

void dsp_rpt_countpacketsfiltred(int fd, __u8 *val)
{
	print_format(fd,"[Packets filtred : %hu packets]",DSP_RPT_COUNT_PACKETS_CAST(val));
}

