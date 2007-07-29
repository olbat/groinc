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

/*
 * For further informations about this implementation please take a look to the following RFC :
 * 	RFC 793 - TRANSMISSION CONTROL PROTOCOL (http://ietf.org/rfc/rfc793.txt)
 */

#include "proto_tcp.h"
#include "printp.h"
#include <netinet/in.h>

void scan_tcp(struct data *datagram, struct protocol_header *transport_layerph, struct protocol_header *application_layerph)
{
	transport_layerph->len = (((struct tcp_header *)transport_layerph->header)->tcphdrlen * 4);
}

void print_tcp(int fd, char *datagram)
{
	struct tcp_header *tcph;
	tcph = (struct tcp_header *) datagram;
	print_proto(fd,"[TCP/ port src:%hu port dest:%hu seqnum:%#x acknum:%#x tcpheaderlen:%d (res:%d ecn:%d ece:%d urg:%d ack:%d psh:%d rst:%d syn:%d fin:%d) window:%#x checksum:%#x urgptr:%#x]",ntohs(tcph->sourceport),ntohs(tcph->destport),ntohl(tcph->seqnum),ntohl(tcph->acknum),(tcph->tcphdrlen*4),tcph->res,tcph->ecn,tcph->ece,tcph->urg,tcph->ack,tcph->psh,tcph->rst,tcph->syn,tcph->fin,ntohs(tcph->window),tcph->window,ntohs(tcph->tcpchecksum),ntohs(tcph->urgptr));
}

