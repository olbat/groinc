/* This file is a part of groinc
 * 
 * Copyright (C) 2006, 2007 Sarzyniec Luc <olbat@xiato.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 * see the COPYING file for more informations */

#include "filter.h"
#include "globals_filter.h"
#include "network/protocols.h"
#include "network/parsers.h"
#include "tools/memory_tools.h"
#include "tools/network_tools.h"
#include "network/tmp_headers.h"

int filter_string(char *data)
{
	int end;
	end = 1;
	if (*filterstr)
	{
		end = findstr(data,filterstr);
	}
	return end;
}

int filter_regex(char *data)
{
	int end;
	end = 1;
	if (*filterregexstr)
	{
		/* the compilation of the regex been checked in check_options */
		if (regexec(&filterregex,data,0,0,0))
			end = 0;
	}
	return end;
}

int filter(struct protocol_header *datalink_layerph,struct protocol_header *network_layerph,struct protocol_header *transport_layerph,struct data *datagram)
{
	int end;
	char *data;
	
	end = 0;
	
	if (nofilter)
	{
		end = 1;
	}
	else
	{
		if (((ethproto == ETHPROTO_RAW) || (network_layerph->id == ethproto)) && (network_layerph->len > 0))
		{
			if (((datalink_layerph->id == PROTO_ETHER) && ((!mac_null(lsourcemac)) || (!mac_null(ldestmac)))) && (datalink_layerph->len > 0))
			{
				__u8 *sourcemac,*destmac;
				sourcemac = (__u8 *) ((struct ethernet_header *)datalink_layerph->header)->sourcemac;
				destmac = (__u8 *) ((struct ethernet_header *)datalink_layerph->header)->destmac;
				if (!mac_null(lsourcemac))
				{
					end = !mac_cmp(lsourcemac,sourcemac);
					/* end = end && (!mac_cmp(lsourcemac,sourcemac)); */
				}
				if (!mac_null(ldestmac))
					end = end && (!mac_cmp(ldestmac,destmac));
			}
			if (((ipproto == IPPROTO_RAW) || ((transport_layerph->id == ipproto) && (transport_layerph->len > 0)))
			&& ((ethproto == ETHPROTO_RAW) || ((ethproto != ETHPROTO_RAW) && (network_layerph->id == ethproto))))
			{
				end = 1;
			}
			/*
			if (((ethproto != ETHPROTO_RAW) && (network_layerph->id == ethproto)) && (network_layerph->len > 0))
			{
				end = 1;
			}
			*/

			if ((end) 
			&& ((network_layerph->id == ETHPROTO_IP) 
			 && ((lsourceip) || (ldestip) || (ldestport) || (lsourceport) || (lglobalip) || (lglobalport))))
			{
				__u32 sourceip, destip;
				__u16 sourceport,destport;
				sourceip = ((struct ipv4_header *)network_layerph->header)->sourceaddr;
				destip = ((struct ipv4_header *)network_layerph->header)->destaddr;
				/* if (end)
				{ */
					get_ports(transport_layerph,&sourceport,&destport);
					
					if (lglobalip)
					{
						end = end && ((destip == lglobalip) || (sourceip == lglobalip));
					}
					else
					{
						if (ldestip)
							end = end && (ldestip == destip);
						if (lsourceip)
							end = end && (lsourceip == sourceip);
					}
					if (lglobalport)
					{
						end = end && ((destport == lglobalport) || (sourceport == lglobalport));
					}
					else
					{
						if (lsourceport)
							end = end && (lsourceport == sourceport);
						if (ldestport)
							end = end && (ldestport == destport);
					}	
				/* } */
			}
			/*
			else
			{
				printf("ethproto %d ipproto %d\n",ethproto,ipproto);
				if (ethproto == ETHPROTO_RAW)
				{
					if (!(transport_layerph->id == ipproto))
						end = 0;
				}
				else 
				{
					if (!(network_layerph->id == ethproto))
						end = 0;
				}
			}
			*/
		}
	}
	
	data = datagram->data + datagram->len;
	if (end)
		end = filter_string(data);
	if (end)
		end = filter_regex(data);
	return end;
}

