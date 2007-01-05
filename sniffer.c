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

#include <malloc.h>
#include <stdio.h> /* needed for perror */
#include <unistd.h>

#include "tools/memory_tools.h"
#include "prints.h"
#include "packet_inout.h"
#include "network/parsers.h"
#include "filter.h"
#include "globals_filter.h"
#include "globals_option.h"

#define DATAGRAM_SIZE 4096

int sniff(int inputfd,int outputfd)
{
	if (inputfd < 0)
		return 1;

	char end;
	int packet_len;
	struct data *datagram;
	struct protocol_header *datalink_layerph;
	struct protocol_header *network_layerph;
	struct protocol_header *transport_layerph;
	
	datagram = (struct data *) malloc(sizeof(struct data)); 
	my_memset((char *)datagram,0,sizeof(struct data));
	datagram->data = (char *) malloc(sizeof(char)*DATAGRAM_SIZE);
	my_memset(datagram->data,0,sizeof(char)*DATAGRAM_SIZE);
	
	datalink_layerph = (struct protocol_header *) malloc(sizeof(struct protocol_header));
	network_layerph = (struct protocol_header *) malloc(sizeof(struct protocol_header));
	transport_layerph = (struct protocol_header *) malloc(sizeof(struct protocol_header));
	
	end = 0;

	while (!end)
	{
		if ((packet_len = read_packet(inputfd,datagram->data,sizeof(char)*DATAGRAM_SIZE)) < 0)
		{
			perror("read_input");
			return 1;
		}
		else
		{
			
			if (packet_len > 0)
			{
				datagram->len = 0;
				datagram->totlen = packet_len;
				
				parse_datalink_layer(datagram,datalink_layerph,network_layerph);
				parse_network_layer(datagram,network_layerph,transport_layerph);
				parse_transport_layer(datagram,transport_layerph);

				if ((datagram->len < datagram->totlen) || (!opt_ndisplayemptyslp)) /* if the session layer is empty */
				{
					if (filter(datalink_layerph,network_layerph,transport_layerph,datagram) != 0)
					{
						if (outputfd >= 0)
						{
							if (write_packet(outputfd,datagram->data,packet_len) < 0)
							{
								perror("write_output");
								return 1;
							}
						}
						if (opt_displaypackets)
						{
							print_packetnb(headerfd,datagram->len);
						}
						if (opt_displaynlproto)
						{
							print_network_layer_proto(headerfd,datalink_layerph,network_layerph);
						}
						if (opt_displaydlproto)
						{
							print_datalink_layer_proto(headerfd,datalink_layerph);
						}
						if (opt_displaytlproto)
						{
							print_transport_layer_proto(headerfd,network_layerph,transport_layerph);
						}
						if (opt_simpledisplay)
						{
							print_simple(headerfd,network_layerph,transport_layerph);
						}
						else
						{
							if (opt_displayheader)
							{
								print_datalink_layer(headerfd,datalink_layerph);
								print_network_layer(headerfd,network_layerph);
								print_transport_layer(headerfd,transport_layerph);
							}
						}
						if (opt_displaydata)
						{
							print_data(datafd,datagram);
						}
						if (!opt_displaypackets && !opt_simpledisplay && !opt_displayheader && !opt_displaydata)
						{
							print_ipproto(headerfd,transport_layerph);
							print_simple(headerfd,network_layerph,transport_layerph);
							print_data(datafd,datagram);
						}
						print_newline(headerfd);
					}
				}
				else
				{
					if (opt_displayallpackets)
					{
						print_packetnb(headerfd,datagram->len);
					}
				}
			}
			else
			{
				if (*inputfile)
					close(inputfd);
				end = 1;
			}
		}
	}
	close(outputfd);
	free(datalink_layerph);
	free(network_layerph);
	free(transport_layerph);
	return 0;
}

