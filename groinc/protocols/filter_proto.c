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

#include "filter_proto.h"
#include "../parse_options.h"

#include "proto_tcp.h"
#include "proto_ipv4.h"

#include <string.h>
#include <strings.h>
#include <regex.h>

#define PRT_FLT_BUFFERSIZE 64

#define LOOKUP_PROTOFILTER(T,N) __extension__ \
({ \
	struct proto_filter *t; \
	t = T; \
	while ((t->id != _PROTOFLT_MAX) && (strcmp(t->name,N))) \
		t++; \
	if (t->id == _PROTOFLT_MAX) \
		t = 0; \
	t; \
})
		
int
check_filter(
	char *filtername,
	char *val)
{
	regex_t tmp;
	regcomp(&tmp,"^\\(0x\\)\\?[0-9]\\+$",REG_NOSUB);
	if (regexec(&tmp,val,0,0,0))
	{
		regfree(&tmp);
		return PRT_FLT_ERROR;
	}
	else
	{
		regfree(&tmp);
		return PRT_FLT_OK;
	}
}

/* protofilterstr : protoname-protooptname */
__inline__ int proto_filter(char *protofilterstr, char *val)
{
	char *ptr;

	struct proto_filter pflist[] = {
		{ PROTOFLT_ETHER,	"ether", 	0, 0, 0, 0 },
		{ PROTOFLT_ARP,		"arp",		0, 0, 0, 0 },
		{ PROTOFLT_IP,		"ip",		check_filter,
			parse_filter_ipv4, 0, filter_ipv4 },
		{ PROTOFLT_ICMP,	"icmp",		0, 0, 0, 0 },
		{ PROTOFLT_UDP,		"udp",		0, 0, 0, 0 },
		{ PROTOFLT_TCP,		"tcp",		check_filter,
			parse_filter_tcp, 0, filter_tcp },
		{ _PROTOFLT_MAX,	"",		0, 0, 0, 0 }
	};

	if ((ptr = index(protofilterstr,'-')))
	{
		register struct proto_filter *pfptr;
		__u8 buff[PRT_FLT_BUFFERSIZE];
		int flt_size;

		flt_size = 0;
		*ptr = 0;
		ptr++;
		if ((pfptr = LOOKUP_PROTOFILTER(pflist,protofilterstr)))
		{
			/* >>> TODO: display help by protocol */
			if (*ptr)
			{
				if (pfptr->func_check)
					if ((pfptr->func_check)(ptr,val) 
					!= PRT_FLT_OK)
						goto err;
				if ((pfptr->func_parse) 
				&& ((flt_size = (pfptr->func_parse)(ptr,val,buff))))
					goto out;
			}
			goto err;
out:
			PRT_LKD_FLT_ADD(buff,flt_size,pfptr->func_flt,
				pfptr->func_free);
			return OPT_OK;
		}
	}
err:
	*(ptr-1) = '-';
	return OPT_ERROR;
}

