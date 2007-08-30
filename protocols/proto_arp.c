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
 * 	RFC 826 - An Ethernet Address Resolution Protocol (http://ietf.org/rfc/rfc826.txt)
 * 	RFC 903 - A Reverse Address Resolution Protocol (http://ietf.org/rfc/rfc903.txt)
 * 	RFC 2390 - Inverse Address Resolution Protocol (http://ietf.org/rfc/rfc2390.txt)
 */

#include "proto_arp.h"
#include "printp.h"
#include <netinet/in.h>
#include "../network/my_types.h"
#include "../tools/network_tools.h"
#include "../tools/memory_tools.h"

/* buffers for the MAC string versions */
static char sourcehardmac[MAC_STR_SIZE];
static char desthardmac[MAC_STR_SIZE];

/* buffers for the IP string versions */
static char sourceprotoip[IPV4_STR_MAXSIZE];
static char destprotoip[IPV4_STR_MAXSIZE];

void scan_arp(struct data *datagram, struct protocol_header *network_layerph, struct protocol_header *transport_layerph)
{
	struct arp_header *arph;
	arph = (struct arp_header *) network_layerph->header;
	
	network_layerph->len = sizeof(struct arp_header) + (arph->hardlen * 2) + (arph->protolen * 2);

	transport_layerph->len = -1;
}

/* the implementation of this function need to be optimized */
void print_arp(int fd, char *datagram)
{
	struct arp_header *arph;
	arph = (struct arp_header *) datagram; 
	print_proto(fd,"[ARP/ hardtype:%#x prototype:%#x hardlen:%hhd protolen:%hhd opcode:%hd",ntohs(arph->hardtype),ntohs(arph->prototype),arph->hardlen,arph->protolen,ntohs(arph->opcode));
	
	datagram = datagram + sizeof(struct arp_header);
	if (ntohs(arph->hardtype) == PROTOARP_HARD_ETHERNET)
	{
		print_proto(fd," sourcehard:%s",mac_ntoa((__u8 *)datagram,sourcehardmac));
	}
	else
	{
		__extension__ char tmph[arph->hardlen];
		my_memcpy(tmph,datagram,arph->hardlen);
		print_proto(fd," sourcehard:%#x",tmph);
	}
	
	datagram = datagram + arph->hardlen;
	if (ntohs(arph->prototype) == PROTOARP_PROTO_IP)
	{
		__u32 tmpp;
		my_memcpy((char *)&tmpp,datagram,sizeof(__u32));
		print_proto(fd," sourceproto:%s",ipv4_ntoa(ntohl(tmpp),sourceprotoip));
	}
	else
	{
		__extension__ char tmpp[arph->protolen];
		my_memcpy(tmpp,datagram,arph->protolen);
		print_proto(fd," sourceproto:%#x",tmpp);
	}

	datagram = datagram + arph->protolen;
	if (ntohs(arph->hardtype) == PROTOARP_HARD_ETHERNET)
	{
		print_proto(fd," desthard:%s",mac_ntoa((__u8 *)datagram,desthardmac));
	}
	else
	{
		__extension__ char tmph[arph->hardlen];
		my_memcpy(tmph,datagram,arph->hardlen);
		print_proto(fd," desthard:%#x",tmph);
	}

	datagram = datagram + arph->hardlen;
	if (ntohs(arph->prototype) == PROTOARP_PROTO_IP)
	{
		__u32 tmpp;
		my_memcpy((char *)&tmpp,datagram,sizeof(__u32));
		print_proto(fd," destproto:%s",ipv4_ntoa(ntohl(tmpp),destprotoip));
	}
	else
	{
		__extension__ char tmpp[arph->protolen];
		my_memcpy(tmpp,datagram,arph->protolen);
		print_proto(fd," destproto:%#x",tmpp);
	}

	print_proto(fd,"]");
}

