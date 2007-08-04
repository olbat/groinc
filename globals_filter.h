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

#ifndef _GLOBALS_FILTER_H
#define _GLOBALS_FILTER_H

#include "network/my_types.h"
#include "tools/network_tools.h"
#include <regex.h>
#include <sys/time.h>

extern	__u16 	ldestport,
		lsourceport,
		lglobalport;

extern __u32	ldestip,
		lsourceip,
		lglobalip;

extern __u8	lsourcemac[MAC_STR_SIZE],
		ldestmac[MAC_STR_SIZE];

extern long int 	llimitnb;
extern unsigned long int 	packetstot, packetsfiltred;

extern char 	*filterstr,
       		*filterregexstr;	

extern	unsigned int 	proto,
			ethproto,
			ipproto;

extern	int 	datafd,
   		headerfd;

extern char	sniffer_stop,
		nofilter,
		filter_datalink,
		filter_network,
		filter_transport;

extern	regex_t filterregex;
extern struct timeval 	timestart,
			timefirstpacket,
			timelimit;

extern struct linked_list *list_filter;

#endif

