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
#include "tools/linked_list.h"
#include "tools/memory_tools.h"
#include "tools/network_tools.h"
#include "network/tmp_headers.h"

/*
enum { FILER_STRING,FILTER_REGEX,FILTER_IPDST,FILTER_IPSRC,FILTER_MACDST,FILTER_MACSRC,FILTER_PORTSRC,FILTER_PORTDST }
*/


int filter(struct protocol_header *datalink_layerph,struct protocol_header *network_layerph,struct protocol_header *transport_layerph,struct data *datagram)
{
	int end;
	char *data;

	end = 1;

	if (!nofilter)
	{
		struct linked_list *ptr = list_filter;
		while ((ptr) && (end))
		{
			end = end && (ptr->value->u.flt->func_flt)(datalink_layerph,network_layerph,transport_layerph,ptr->value->u.flt->val);
			ptr = ptr->next;
		}
			
		if (filter_datalink)
		{
			if (datalink_layerph->len > 0)
			{
				if ((!mac_null(lsourcemac)) || (!mac_null(ldestmac)) || (proto))
				{
					if (proto != PROTO_ETHER)
						end = end && (datalink_layerph->id == proto);
					if ((end) || ((datalink_layerph->id == PROTO_ETHER)))
					{
						struct ethernet_header *ethh = (struct ethernet_header *)datalink_layerph->header;
						if (!mac_null(lsourcemac))
							end = !mac_cmp(lsourcemac,(__u8 *)ethh->sourcemac);
						if (!mac_null(ldestmac))
							end = end && (!mac_cmp(ldestmac,(__u8 *)ethh->destmac));
					}
				}
			}
			else
			{
				end = 0;
			}
		}
		if ((end) && (filter_network))
		{
			if (network_layerph->len > 0)
			{
				if (ethproto != ETHPROTO_RAW)
					end = end && (network_layerph->id == ethproto);
				if ((end) && ((lsourceip) || (ldestip) || (lglobalip)))
				{
					struct ipv4_header *iph = (struct ipv4_header *)network_layerph->header;
					if (lglobalip)
					{
						end = end && ((iph->sourceaddr == lglobalip) || (iph->destaddr == lglobalip));
					}
					else
					{
						if (ldestip)
							end = end && (iph->destaddr == ldestip);
						if (lsourceip)
							end = end && (iph->sourceaddr == lsourceip);
					}
				}
			}
			else
			{
				end = 0;
			}
		}
		if ((end) && (filter_transport))
		{
			if (transport_layerph->len > 0)
			{
				if (ipproto != IPPROTO_RAW)
					end = end && (transport_layerph->id == ipproto);
				if ((end) && ((ldestport) || (lsourceport) || (lglobalport)))
				{
					if (lglobalport)
					{
						end = end && ((get_dest_port(transport_layerph) == lglobalport) || (get_source_port(transport_layerph) == lglobalport));
					}
					else
					{
						if (lsourceport)
							end = end && (get_source_port(transport_layerph) == lsourceport);
						if (ldestport)
							end = end && (get_dest_port(transport_layerph) == ldestport);
					}	
				}

			}
			else
			{
				end = 0;
			}
		}
	}

	data = datagram->data + datagram->len;
	
	if ((end) && (*filterstr))
		end = filter_string(data);
	if ((end) && (*filterregexstr))
		end = filter_regex(data);

	return end;
}

int filter_string(char *data)
{
	int end;
	if (*data)
		end = findstr(data,filterstr);
	else
		end = 1;
	return end;
}

int filter_regex(char *data)
{
	int end;
	/* the compilation of the regex been checked in check_options */
	if (regexec(&filterregex,data,0,0,0))
		end = 0;
	else
		end = 1;
	return end;
}

#define FLT_DL_MAC(HDR,V,E) \
({ \
	struct ethernet_header *ethh = (struct ethernet_header *) HDR; \
	!mac_cmp(V,(__u8 *)ethh->E); \
})

__inline__ int flt_dl_mac_src(struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, __u8 *flt_val)
{
	if (datalink_layerph->id == PROTO_ETHER)
		return FLT_DL_MAC(datalink_layerph->header,flt_val,sourcemac);
	else
		return FLT_ERROR;
}

__inline__ int flt_dl_mac_dst(struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, __u8 *flt_val)
{
	if (datalink_layerph->id == PROTO_ETHER)
		return FLT_DL_MAC(datalink_layerph->header,flt_val,destmac);
	else
		return FLT_ERROR;
}

#define FLT_DL_IP(HDR,V,E) \
({ \
	struct ipv4_header *iph = (struct ipv4_header *) HDR; \
	(iph->V == E); \
})

__inline__ int flt_nl_ip_src(struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, __u8 *flt_val)
{
	if (network_layerph->id == ETHPROTO_IP)
		return FLT_DL_IP(network_layerph->header,sourceaddr,*((__u32 *)flt_val));
	else
		return FLT_ERROR;
}

__inline__ int flt_nl_ip_dst(struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, __u8 *flt_val)
{
	if (network_layerph->id == ETHPROTO_IP)
		return FLT_DL_IP(network_layerph->header,destaddr,*((__u32 *)flt_val));
	else
		return FLT_ERROR;
}

__inline__ int flt_nl_ip_global(struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, __u8 *flt_val)
{
	if (network_layerph->id == ETHPROTO_IP)
		return FLT_DL_IP(network_layerph->header,sourceaddr,*((__u32 *)flt_val)) || FLT_DL_IP(network_layerph->header,destaddr,*((__u32 *)flt_val));
	else
		return FLT_ERROR;
}
