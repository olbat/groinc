/* This file is a part of groinc
 * Copyright 2006 Sarzyniec Luc <olbat@xiato.com>
 * This software is released under GPL the license
 * see the COPYING file for more informations */

#ifndef _GLOBALS_FILTER_H
#define _GLOBALS_FILTER_H

#include "network/my_types.h"
#include <regex.h>

extern	__u16 	ldestport,
		lsourceport,
		lglobalport;
extern __u32	ldestip,
		lsourceip,
		lglobalip;
extern __u8	*lsourcemac,
		*ldestmac;
extern char 	*filterstr,
       		*filterregexstr;	
extern	regex_t filterregex;
extern	unsigned char 	proto,
			ethproto,
			ipproto;
extern	int 	datafd,
   		headerfd;

#endif

