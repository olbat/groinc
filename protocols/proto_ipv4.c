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
 * 	RFC 791 - INTERNET PROTOCOL (http://ietf.org/rfc/rfc791.txt)
 */

#include "proto_ipv4.h"
#include "printp.h"
#include "../tools/network_tools.h"
#include "namescache.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

/* buffers for the IP string versions */
static char sourceip[IPV4_STR_MAXSIZE];
static char destip[IPV4_STR_MAXSIZE];

void scan_ipv4(struct data *datagram, struct protocol_header *network_layerph, struct protocol_header *transport_layerph)
{
	network_layerph->len = (((struct ipv4_header *)network_layerph->header)->iphdrlen * 4);
	transport_layerph->id = ((struct ipv4_header *)network_layerph->header)->proto;
}

void print_ipv4(int fd, char *datagram)
{
	struct ipv4_header *iph;
	iph = (struct ipv4_header *) datagram;

	print_proto(fd,"[IPv4/ version:%hhd ipheaderlen:%hhd tos:%#x totlen:%d id:%#x fragoffset:%#x ttl:%hhu proto:%hhd checksum:%#x source:%s dest:%s]",iph->version,(iph->iphdrlen*4),iph->tos,ntohs(iph->totlen),ntohs(iph->id),ntohs(iph->fragoffset),iph->ttl,iph->proto,ntohs(iph->ipchecksum),ipv4_ntoa(ntohl(iph->sourceaddr),sourceip),ipv4_ntoa(ntohl(iph->destaddr),destip));
}

#define PROTO_IPV4_DOMAINNAME(S,V,T) __extension__ \
({ \
	struct hostent *tmp; \
	if ((tmp = gethostbyaddr(V,sizeof(*V),AF_INET))) \
		S = tmp->h_name; \
	else \
		S = ipv4_ntoa(ntohl(*V),T); \
})

void print_ipv4_simple(int fd, char *datagram, __u16 sourceport, __u16 destport)
{	
	struct ipv4_header *iph;

	iph = (struct ipv4_header *) datagram;

	print_proto(fd,"[%s:%hu->%s:%hu] ",get_ipv4_name(iph->sourceaddr),sourceport,get_ipv4_name(iph->destaddr),destport);
}
