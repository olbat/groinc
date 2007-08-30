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

#ifndef _TMP_HEADERS_H
#define _TMP_HEADERS_H

#include "my_types.h"

/* All in this file is in network-byte-order */

struct ethernet_header
{
	__u8	destmac[6];
	__u8	sourcemac[6];
	__u16	proto;
};

struct ipv4_header
{
__extension__	__u8 iphdrlen:4,
	     	     version:4;
	__u8 tos;
	__u16 totlen;
	__u16 id;
	__u16 fragoffset;
	__u8 ttl;
	__u8 proto;
	__u16 ipchecksum;
	__u32 sourceaddr;
	__u32 destaddr;
};

struct arp_header
{
	__u16 hardtype;
	__u16 prototype;
	__u8 hardlen;
	__u8 protolen;
	__u16 operation;
	char *sourceaddrhard;
	char *sourceaddrproto;
	char *destaddrhard;
	char *destaddrproto;
};	

struct tcp_header
{
	__u16 sourceport;
	__u16 destport;
	__u32 seqnum;
	__u32 acknum;
	__extension__ __u16 res:4,
			    tcphdrlen:4,
	      		    fin:1,
	      		    syn:1,
	      		    rst:1,
	      		    psh:1,
	      	  	    ack:1,
	      		    urg:1,
	      		    ece:1,
	      		    ecn:1;
	__u16 window;
	__u16 tcpchecksum;
	__u16 urgptr;
};

struct udp_header
{
	__u16 sourceport;
	__u16 destport;
	__u16 udpdatalen;
	__u16 udpchecksum;
};

struct pseudo_header 
{
 	__u32 sourceaddr;
	__u32 destaddr;
	__u8 zero;
	__u8 proto;
	__u16 tcplen;
};

#endif
