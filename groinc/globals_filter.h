/* This file is a part of groinc
 * Copyright 2006 Sarzyniec Luc <olbat@xiato.com>
 * This software is released under GPL the license
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
		nofilter;

extern	regex_t filterregex;
extern struct timeval 	timestart,
			timefirstpacket,
			timelimit;

#endif

