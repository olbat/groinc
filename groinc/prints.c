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

#include <stdio.h>
#include <netinet/in.h>
#include "prints.h"
#include "network/protocols.h"
#include "globals_filter.h"
#include "globals_args.h"
#include "tools/network_tools.h"
#include "network/parsers.h"

void print_ethernet(int fd, struct ethernet_header *etherh)
{
	fprintf((FILE *)fd,"[Ethernet/ dest mac: %s source mac: %s proto:%#x]",mac_ntoa((__u8 *)etherh->destmac),mac_ntoa((__u8 *)etherh->sourcemac),ntohs(etherh->proto));
}
void print_ip(int fd,struct ipv4_header *iph)
{
	fprintf((FILE *)fd,"[IPv4/ version:%hhd ipheaderlen:%hhd tos:%#x totlen:%d id:%#x fragoffset:%#x ttl:%hhd proto:%hhd checksum:%#x source:%s dest:%s]",iph->version,(iph->iphdrlen*4),iph->tos,ntohs(iph->totlen),ntohs(iph->id),ntohs(iph->fragoffset),iph->ttl,iph->proto,ntohs(iph->ipchecksum),ipv4_ntoa(ntohl(iph->sourceaddr)),ipv4_ntoa(ntohl(iph->destaddr)));
}

void print_tcp(int fd,struct tcp_header *tcph)
{
	fprintf((FILE *)fd,"[TCP/ port src:%hd port dest:%hd seqnum:%#x acknum:%#x tcpheaderlen:%d (res:%d ecn:%d ece:%d urg:%d ack:%d psh:%d rst:%d syn:%d fin:%d) window:%#x %hd tcpcheckdum:%#x urgptr:%#x]\n",ntohs(tcph->sourceport),ntohs(tcph->destport),ntohl(tcph->seqnum),ntohl(tcph->acknum),(tcph->tcphdrlen*4),tcph->res,tcph->ecn,tcph->ece,tcph->urg,tcph->ack,tcph->psh,tcph->rst,tcph->syn,tcph->fin,ntohs(tcph->window),tcph->window,ntohs(tcph->tcpchecksum),ntohs(tcph->urgptr));
}

void print_udp(int fd, struct udp_header *udph)
{
	fprintf((FILE *)fd,"[UDP/ source port:%hd dest port:%hd udpdatalen:%hd checksum:%#x]",ntohs(udph->sourceport),ntohs(udph->destport),ntohs(udph->udpdatalen),udph->udpchecksum);
}

void print_unknown(int fd,char *layer,char *protoname)
{
	if (*protoname)
		fprintf((FILE *)fd,"[%s protocol/%s layer]",protoname,layer);
	else
		fprintf((FILE *)fd,"[Unknown protocol/%s layer]",layer);
}

void print_datalink_layer(int fd, struct protocol_header *datalink_layerph)
{
	switch(datalink_layerph->id)
	{
		case PROTO_ETHER :
			print_ethernet(fd,(struct ethernet_header *)datalink_layerph->header);
			break;
		default :
			print_unknown(fd,"datalink",lookup_protoname(datalink_layerph->id));
			break;
	}
}

void print_network_layer(int fd, struct protocol_header *network_layerph)
{
	switch (network_layerph->id)
	{
		case ETHPROTO_IP :
			print_ip(fd,(struct ipv4_header *)network_layerph->header);
			break;
		default :
			print_unknown(fd,"network",lookup_ethname(network_layerph->id));
			break;
	}
}

void print_transport_layer(int fd, struct protocol_header *transport_layerph)
{
	switch (transport_layerph->id)
	{
		case IPPROTO_TCP :
			print_tcp(fd,(struct tcp_header *)transport_layerph->header);
			break;
		case IPPROTO_UDP :
			print_udp(fd,(struct udp_header *)transport_layerph->header);
			break;
		default :
			print_unknown(fd,"transport",lookup_ipname(transport_layerph->id));
			break;
	}
}

void print_data(int fd,struct data *data)
{
	fwrite((data->data + data->len),sizeof(char),(data->totlen - data->len),(FILE *)fd);
	//write(fd,(data->data + data->len),(data->totlen - data->len));
	//fflush((FILE *)fd);
}

void print_simple(int fd,struct protocol_header *network_layerph,struct protocol_header *transport_layerph)
{
	__u16 sourceport,destport;
	struct ipv4_header *iph;
	get_ports(transport_layerph,&sourceport,&destport);
	if (network_layerph->id == ETHPROTO_IP)
	{
		iph = (struct ipv4_header *) network_layerph->header;
		fprintf((FILE *)fd,"[%s:%hd<-%s:%hd] ",ipv4_ntoa(ntohl(iph->destaddr)),destport,ipv4_ntoa(ntohl(iph->sourceaddr)),sourceport);
	}
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
	//fflush((FILE *)fd);
}

void print_newline(int fd)
{
	fputc('\n',(FILE *)fd);
	//fflush((FILE *)fd);
}

