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
 * 	RFC 792 - INTERNET CONTROL MESSAGE PROTOCOL (http://ietf.org/rfc/rfc792.txt)
 * 	RFC 1256 - ICMP Router Discovery Messages (http://ietf.org/rfc/rfc1256.txt)
 */

#include "proto_icmp.h"
#include "printp.h"
#include "../tools/network_tools.h"
#include "netinet/in.h"

/* buffers for the IP string versions */
static char gatewayip[IPV4_STR_MAXSIZE];

void scan_icmp(struct data *datagram, struct protocol_header *transport_layerph)
{
	struct icmp_header *icmph;
	icmph = (struct icmp_header *) transport_layerph->header;
	
	transport_layerph->len = sizeof(struct icmp_header) ;
	if ((icmph->type == 9) && (icmph->code == 0))
		/* RFC 1256 (http://ietf.org/rfc/rfc1256.txt) */
		transport_layerph->len = transport_layerph->len + (((icmph->field&0xff000000)>>24) * (((icmph->field&0xff0000)>>16) * 4));
	else if ((icmph->type == 13) || (icmph->type == 14))
		transport_layerph->len = transport_layerph->len + 12;			
}

void print_icmp(int fd, char *datagram)
{
	struct icmp_header *icmph;
	char *ptr;
	icmph = (struct icmp_header *) datagram;
	ptr = datagram + sizeof(struct icmp_header);
	print_proto(fd,"[ICMP/ type:%hhd code:%hhd checksum:%#x",icmph->type,icmph->code,icmph->icmpchecksum);

	if (icmph->type == 5)
	{
		print_proto(fd," gateway:%s",ipv4_ntoa(icmph->field,gatewayip));
	}
	else if ((icmph->type == 0) || (icmph->type == 8) || (icmph->type == 15) || (icmph->type == 16))
	{
		__u32 field;
		field = ntohl(icmph->field);
		print_proto(fd," (ident:%#x seqnum:%#x)",(field&0xffff0000)>>16,field&0x0000ffff);
	
	}	
	else if ((icmph->type == 9) || (icmph->type == 10))	
	{
		print_proto(fd," (numaddrs:%hhd addrsize:%hhd lifetime:%hd)",(icmph->field&0xff000000)>>24,(icmph->field&0xff0000)>>16,icmph->field&0x0000);
		/* to be done */
	}
	else if (icmph->type == 12)
	{
		print_proto(fd," (pointer:%hhd zero:%#x)",(icmph->field&0xff000000)>>24,icmph->field&0x00ffffff);
	}
	else if ((icmph->type == 13) || (icmph->type == 14))
	{
		__u32 field;
		field = ntohl(icmph->field);
		print_proto(fd," (ident:%#x seqnum:%#x) origtimestamp:%ld rectimestamp:%ld transtimestamp:%ld",(field&0xffff0000)>>16,field&0x0000ffff,ntohl((__u32) ptr),ntohl((__u32) (ptr + 4)),ntohl((__u32) (ptr + 8)));
	}
	else
	{
		print_proto(fd," zero:%#x",icmph->field);
	}
	print_proto(fd,"]\n");
}
