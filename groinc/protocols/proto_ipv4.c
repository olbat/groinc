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

/*
 * For further informations about this implementation please take a look
 * to the following RFC :
 * 	RFC 791 - INTERNET PROTOCOL (http://ietf.org/rfc/rfc791.txt)
 */

#include "proto_ipv4.h"
#include "printp.h"
#include "filter_proto.h"
#include "../tools/network_tools.h"
#include "../tools/compiler.h"
#include "namescache.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "../network/protocols.h"

/* buffers for the IP string versions */
static char sourceip[IPV4_STR_MAXSIZE];
static char destip[IPV4_STR_MAXSIZE];

void
scan_ipv4(
	struct data *datagram,
	struct protocol_header *network_layerph,
	struct protocol_header *transport_layerph)
{
	network_layerph->len = 
		(FLAG_VAL(((struct ipv4_header *)network_layerph->header)
			->lenvers,IPV4_HDRLEN,0) * 4);
	transport_layerph->id = 
		((struct ipv4_header *)network_layerph->header)->proto;
}

void
print_ipv4(
	int fd,
	char *datagram)
{
	struct ipv4_header *iph;
	iph = (struct ipv4_header *) datagram;

	print_proto(fd,"[IPv4/ version:%hhd ipheaderlen:%hhd tos:%#x totlen:%d "
		"id:%#x fragoffset:%#x ttl:%hhu proto:%hhd checksum:%#x "
		"source:%s dest:%s]",FLAG_VAL(iph->lenvers,IPV4_VERS,4),
		FLAG_VAL(iph->lenvers,IPV4_HDRLEN,0),iph->tos,
		ntohs(iph->totlen),ntohs(iph->id), ntohs(iph->fragoffset),
		iph->ttl,iph->proto,ntohs(iph->ipchecksum),
		ipv4_ntoa(ntohl(iph->sourceaddr),sourceip),
		ipv4_ntoa(ntohl(iph->destaddr), destip));
}

void
print_ipv4_simple(
	int fd,
	char *datagram,
	__u16 sourceport,
	__u16 destport)
{	
	struct ipv4_header *iph;

	iph = (struct ipv4_header *) datagram;

	print_proto(fd,"[%s:%hu->%s:%hu] ",
		get_ipv4_name(iph->sourceaddr),sourceport,
		get_ipv4_name(iph->destaddr),destport);
}

int
parse_filter_ipv4(
	char *filtername,
	char *val,
	__u8 *buff)
{
	struct proto_filter_parse prf_parse_ipv4[] = {
	{ "hdrlen", PRF_TYPE_BITFIELD, offsetof(struct ipv4_header,lenvers),0,
		4 },
	{ "len", PRF_TYPE_BITFIELD, offsetof(struct ipv4_header,lenvers),0,
		4 },
	{ "version", PRF_TYPE_BITFIELD, offsetof(struct ipv4_header,lenvers),4,
		4 },
	{ "tos", PRF_TYPE_NORMAL, offsetof(struct ipv4_header,tos), 0,
		sizeofm(struct ipv4_header,tos) }, 
	{ "totlen", PRF_TYPE_NORMAL, offsetof(struct ipv4_header,totlen), 0,
		sizeofm(struct ipv4_header,totlen) },
	{ "id", PRF_TYPE_NORMAL, offsetof(struct ipv4_header,id), 0,
		sizeofm(struct ipv4_header,id) },
	{ "fragoffset", PRF_TYPE_NORMAL, offsetof(struct ipv4_header,fragoffset), 0,
		sizeofm(struct ipv4_header,fragoffset) },
	{ "ttl", PRF_TYPE_NORMAL, offsetof(struct ipv4_header,ttl), 0,
		sizeofm(struct ipv4_header,ttl) },
	{ "proto", PRF_TYPE_NORMAL, offsetof(struct ipv4_header,proto), 0,
		sizeofm(struct ipv4_header,proto) },
	{ "ipchecksum", PRF_TYPE_NORMAL, offsetof(struct ipv4_header,ipchecksum),
		0, sizeofm(struct ipv4_header,ipchecksum) },
	{ "sourceaddr", PRF_TYPE_NORMAL, offsetof(struct ipv4_header,sourceaddr),
		0, sizeofm(struct ipv4_header,sourceaddr) },
	{ "destaddr", PRF_TYPE_NORMAL, offsetof(struct ipv4_header,destaddr), 0,
		sizeofm(struct ipv4_header,destaddr) },
	{ "",   0,      0,      0,      0 }
	
	};
	PRT_FLT_PARSE(prf_parse_ipv4);
}

int
filter_ipv4(
		struct protocol_header *dlph,
		struct protocol_header *nlph,
		struct protocol_header *tlph,
		struct data *data,
		__u8 *val)
{
	if ((nlph->len > 0) && (nlph->id == ETHPROTO_IP))
		PRT_FLT_CMP(nlph->header,val);
	else
		return 0;
}

