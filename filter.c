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

#include "filter.h"
#include "globals_filter.h"
#include "network/protocols.h"
#include "network/headers.h"
#include "network/parsers.h"
#include "tools/memory_tools.h"
#include "tools/linked_list.h"
#include "tools/compiler.h"
#include "tools/network_tools.h"
#include "network/tmp_headers.h"

#include <regex.h>
#include <string.h>

int
filter(
	struct protocol_header *dlph, struct protocol_header *nlph,
	struct protocol_header *tlph, struct data *datagram
)
{
	int end;
	struct linked_list *ptr;

	end = 1;
	ptr = list_filter;

	while ((ptr) && (end)) {
		if (likely(ptr->value)) {
			end = end && 
			(ptr->value->u.flt->func_flt) (
				dlph,nlph,tlph,datagram,ptr->value->u.flt->val
			);
		}
		ptr = ptr->next;
	}

	return end;
}


#define FLT_DL_MAC(HDR,V,E) __extension__ \
({ \
	struct ethernet_header *ethh = (struct ethernet_header *) HDR; \
	!mac_cmp(V,(__u8 *)ethh->E); \
})

int
flt_dl_mac_src(
	struct protocol_header *dlph, struct protocol_header *nlph,
	struct protocol_header *tlph, struct data *datagram, __u8 *val
)
{
	if ((likely(dlph->len > 0)) && (dlph->id == PROTO_ETHER))
		return FLT_DL_MAC(dlph->header,val,sourcemac);
	else
		return FLT_ERROR;
}

int
flt_dl_mac_dst(
	struct protocol_header *dlph, struct protocol_header *nlph,
	struct protocol_header *tlph, struct data *datagram, __u8 *val
)
{
	if ((likely(dlph->len > 0)) && (dlph->id == PROTO_ETHER))
		return FLT_DL_MAC(dlph->header,val,destmac);
	else
		return 0;
}

#define FLT_NL_IP(HDR,V,E) __extension__ \
({ \
	struct ipv4_header *iph = (struct ipv4_header *) HDR; \
	((iph->V & *(E + 1)) == *(E)); \
})

int
flt_nl_ip_src(
	struct protocol_header *dlph, struct protocol_header *nlph,
	struct protocol_header *tlph, struct data *datagram, __u8 *val
)
{
	if ((likely(nlph->len > 0)) && (nlph->id == ETHPROTO_IP))
		return FLT_NL_IP(nlph->header,sourceaddr,((__u32 *)val));
	else
		return 0;
}

int
flt_nl_ip_dst(
	struct protocol_header *dlph, struct protocol_header *nlph,
	struct protocol_header *tlph, struct data *datagram, __u8 *val
)
{
	if ((likely(nlph->len > 0)) && (nlph->id == ETHPROTO_IP))
		return FLT_NL_IP(nlph->header,destaddr,((__u32 *)val));
	else
		return 0;
}

int
flt_nl_ip_global(
	struct protocol_header *dlph, struct protocol_header *nlph,
	struct protocol_header *tlph, struct data *datagram, __u8 *val
)
{
	
	if ((likely(nlph->len > 0)) && (nlph->id == ETHPROTO_IP))
		return (FLT_NL_IP(nlph->header,sourceaddr,((__u32 *)val)) || 
			FLT_NL_IP(nlph->header,destaddr,((__u32 *)val)));
	else
		return 0;
}


int
flt_tl_port_src(
	struct protocol_header *dlph, struct protocol_header *nlph,
	struct protocol_header *tlph, struct data *datagram, __u8 *val
)
{
	if (tlph->len > 0)
		return (get_source_port(tlph) == *((__u16 *)val));
	else
		return 0;
}

int
flt_tl_port_dst(
	struct protocol_header *dlph, struct protocol_header *nlph,
	struct protocol_header *tlph, struct data *datagram, __u8 *val
)
{
	if (tlph->len > 0)
		return (get_dest_port(tlph) == *((__u16 *)val));
	else
		return 0;
}

int
flt_tl_port_global(
	struct protocol_header *dlph, struct protocol_header *nlph,
	struct protocol_header *tlph, struct data *datagram, __u8 *val
)
{
	if (tlph->len > 0)
		return ((get_source_port(tlph) == *((__u16 *)val)) ||
			(get_dest_port(tlph) == *((__u16 *)val)));
	else
		return 0;
}

int
flt_dl_protocol(
	struct protocol_header *dlph, struct protocol_header *nlph,
	struct protocol_header *tlph, struct data *datagram, __u8 *val
)
{
	if (likely(dlph->len > 0))
		return (dlph->id == *((unsigned int *)val));
	else
		return 0;
}

int
flt_nl_protocol(
	struct protocol_header *dlph, struct protocol_header *nlph,
	struct protocol_header *tlph, struct data *datagram, __u8 *val
)
{
	if (likely(nlph->len > 0))
		return (nlph->id == *((unsigned int *)val));
	else
		return 0;
}

int
flt_tl_protocol(
	struct protocol_header *dlph, struct protocol_header *nlph,
	struct protocol_header *tlph, struct data *datagram, __u8 *val
)
{
	if (tlph->len > 0)
		return (tlph->id == *((unsigned int *)val));
	else
		return 0;
}

int
flt_regex(
	struct protocol_header *dlph, struct protocol_header *nlph,
	struct protocol_header *tlph, struct data *datagram, __u8 *val
)
{
	int end;
	end = !(regexec((regex_t *) val,(datagram->data + datagram->len),0,0,0));
	return end;
}

void flt_regex_free(__u8 *flt_val)
{
	regfree((regex_t *)flt_val);
}

int
flt_string(
	struct protocol_header *dlph, struct protocol_header *nlph,
	struct protocol_header *tlph, struct data *datagram, __u8 *val
)
{
	return findstr((datagram->data + datagram->len),(char *)val);
}

int
flt_sl_nempty(
	struct protocol_header *dlph, struct protocol_header *nlph,
	struct protocol_header *tlph, struct data *datagram, __u8 *val
)
{
	return (datagram->totlen > datagram->len);
}

int
flt_dontdisplaypackets(
	struct protocol_header *dlph, struct protocol_header *nlph,
	struct protocol_header *tlph, struct data *datagram, __u8 *val
)
{
	return 0;
}
