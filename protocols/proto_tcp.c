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
 * For further informations about this implementation please take a look
 * to the following RFC :
 * 	RFC 793 - TRANSMISSION CONTROL PROTOCOL (http://ietf.org/rfc/rfc793.txt)
 */

#include "proto_tcp.h"
#include "printp.h"
#include "filter_proto.h"
#include "../tools/compiler.h"
#include <netinet/in.h>
#include <string.h>


void
scan_tcp(
	struct data *datagram,
	struct protocol_header *tlph,
	struct protocol_header *alph)
{
	tlph->len = (FLAG_VAL(((struct tcp_header *)tlph->header)->lenres,
		TCP_LEN,4) * 4);
}

void
print_tcp(
	int fd,
	char *datagram)
{
	struct tcp_header *tcph;
	tcph = (struct tcp_header *) datagram;
	print_proto(fd,"[TCP/ port src:%hu port dest:%hu seqnum:%#x acknum:%#x "
		"tcpheaderlen:%hhu (res:%d ecn:%d ece:%d urg:%d ack:%d psh:%d "
		"rst:%d syn:%d fin:%d) window:%#x checksum:%#x urgptr:%#x]",
		ntohs(tcph->sourceport),ntohs(tcph->destport),
		ntohl(tcph->seqnum),ntohl(tcph->acknum),
		FLAG_VAL(tcph->lenres,TCP_LEN,4),
		BFLAG_VAL(tcph->lenres,TCP_RES),BFLAG_VAL(tcph->flags,TCP_ECN),
		BFLAG_VAL(tcph->flags,TCP_ECE),BFLAG_VAL(tcph->flags,TCP_URG),
		BFLAG_VAL(tcph->flags,TCP_ACK),BFLAG_VAL(tcph->flags,TCP_PSH),
		BFLAG_VAL(tcph->flags,TCP_RST),BFLAG_VAL((tcph->flags),TCP_SYN),
		BFLAG_VAL(tcph->flags,TCP_FIN),ntohs(tcph->window),
		ntohs(tcph->tcpchecksum),ntohs(tcph->urgptr));
}

int
parse_filter_tcp(
	char *filtername,
	char *val,
	__u8 *buff)
{
	struct proto_filter_parse prf_parse_tcp[] = {
	
	{ "sourceport", PRF_TYPE_NORMAL, offsetof(struct tcp_header,sourceport),
		0, sizeofm(struct tcp_header,sourceport) },
	{ "destport", PRF_TYPE_NORMAL, offsetof(struct tcp_header,destport),0,
		sizeofm(struct tcp_header,destport) },
	{ "seqnum", PRF_TYPE_NORMAL, offsetof(struct tcp_header,seqnum),0,
		sizeofm(struct tcp_header,seqnum) },
	{ "acknum", PRF_TYPE_NORMAL, offsetof(struct tcp_header,acknum),0,
		sizeofm(struct tcp_header,acknum) },
	{ "window", PRF_TYPE_NORMAL, offsetof(struct tcp_header,window), 0,
		sizeofm(struct tcp_header,window) },
	{ "len", PRF_TYPE_BITFIELD, offsetof(struct tcp_header,lenres), 4, 4 },
	{ "res", PRF_TYPE_BITFIELD, offsetof(struct tcp_header,lenres), 0, 4 },
	{ "fin", PRF_TYPE_BITFIELD, offsetof(struct tcp_header,flags), 0, 1 },
	{ "syn", PRF_TYPE_BITFIELD, offsetof(struct tcp_header,flags), 1, 1 },
	{ "rst", PRF_TYPE_BITFIELD, offsetof(struct tcp_header,flags), 2, 1 },
	{ "psh", PRF_TYPE_BITFIELD, offsetof(struct tcp_header,flags), 3, 1 },
	{ "ack", PRF_TYPE_BITFIELD, offsetof(struct tcp_header,flags), 4, 1 },
	{ "urg", PRF_TYPE_BITFIELD, offsetof(struct tcp_header,flags), 5, 1 },
	{ "ece", PRF_TYPE_BITFIELD, offsetof(struct tcp_header,flags), 6, 1 },
	{ "ecn", PRF_TYPE_BITFIELD, offsetof(struct tcp_header,flags), 7, 1 },
	{ "checksum",PRF_TYPE_NORMAL, offsetof(struct tcp_header,tcpchecksum),
		0, sizeofm(struct tcp_header,tcpchecksum) },
	{ "urgptr",	PRF_TYPE_NORMAL, offsetof(struct tcp_header,urgptr),
		0, sizeofm(struct tcp_header,urgptr) },
	{ "",	0,	0,	0,	0 }

	};

	PRT_FLT_PARSE(prf_parse_tcp);
}

int
filter_tcp(
	struct protocol_header *dlph,
	struct protocol_header *nlph,
	struct protocol_header *tlph,
	struct data *data,
	__u8 *val)
{
	if ((tlph->len > 0) && (tlph->id == IPPROTO_TCP))
		PRT_FLT_CMP(tlph->header,val);
	else
		return 0;
}

