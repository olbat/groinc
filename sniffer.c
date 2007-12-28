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

#include "sniffer.h"

#include <malloc.h>
#include <stdio.h> /* needed for perror */
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <regex.h>

#include "packet_inout.h"
#include "network/parsers.h"
#include "network/printers.h"
#include "prints.h"
#include "filter.h"
#include "report.h"
#include "globals_filter.h"
#include "globals_display.h"
#include "globals_report.h"
#include "globals_error.h"
#include "defaults.h"
#include "tools/linked_list.h"
#include "protocols/namescache.h"

#include <string.h>

#define DATAGRAM_SIZE 4096

/*
static struct data *datagram;
static struct protocol_header *datalink_layerph;
static struct protocol_header *network_layerph;
static struct protocol_header *transport_layerph;
*/

static struct data datagram;
static char datagram_data[DATAGRAM_SIZE];
static struct protocol_header datalink_layerph;
static struct protocol_header network_layerph;
static struct protocol_header transport_layerph;

static int curinputfd, curoutputfd;

#define SNF_OPT_TIMELIMIT 0x1

int start_sniff(int inputfd,int outputfd)
{
	char end, opt;
	int packet_len;
	fd_set readfds;
	struct timeval *timeptr;

	if (inputfd < 0)
		return 1;


	FD_ZERO(&readfds);
	FD_SET(inputfd,&readfds);
	
	curinputfd = inputfd;
	curoutputfd = outputfd;
	
	/* datagram = (struct data *) malloc(sizeof(struct data));  */
	memset((char *)&datagram,0,sizeof(struct data));
	/* datagram.data = (char *) malloc(sizeof(char)*DATAGRAM_SIZE); */
	datagram.data = datagram_data;
	memset(datagram.data,0,sizeof(char)*DATAGRAM_SIZE);
	
	/* datalink_layerph = (struct protocol_header *) malloc(sizeof(struct protocol_header)); */
	/* network_layerph = (struct protocol_header *) malloc(sizeof(struct protocol_header)); */
	/* transport_layerph = (struct protocol_header *) malloc(sizeof(struct protocol_header)); */
	
	end = 0;
	opt = 0;

	if (timelimit.tv_sec)
	{
		opt = opt | SNF_OPT_TIMELIMIT;
		timeptr = &timelimit;
	}
	else
	{
		timeptr = 0;
	}
	
	while ((!sniffer_stop) && (!end) && (llimitnb))
	{
		if ((opt & SNF_OPT_TIMELIMIT) && !(timelimit.tv_sec))
			end = 1;

		if ((!end) && (select(inputfd+1,&readfds,NULL,NULL,timeptr) > 0))
		{
			if ((packet_len = read_packet(inputfd,datagram.data,sizeof(char)*DATAGRAM_SIZE)) <= 0)
			{
				if (packet_len == 0)
				{
					if (*inputfile)
						close(inputfd);
					end = 1;
				}
				else
				{
					perror("read_input");
					return 1;
				}
			}
			else
			{
				datagram.len = 0;
				datagram.totlen = packet_len;
				*(datagram.data + datagram.totlen) = 0;

				datalink_layerph.len = 0;
				network_layerph.len = 0;
				transport_layerph.len = 0;
				
				parse_datalink_layer(&datagram,&datalink_layerph,&network_layerph);
				if (network_layerph.len >= 0)
					parse_network_layer(&datagram,&network_layerph,&transport_layerph);
				if (transport_layerph.len >= 0)
					parse_transport_layer(&datagram,&transport_layerph);
				/*
				if ((datagram.len < datagram.totlen) || (!opt_ndisplayemptyslp))
				{
				*/
				if (filter(&datalink_layerph,&network_layerph,&transport_layerph,&datagram))
				{
					/*
					if ((!timefirstpacket.tv_sec) && (!timefirstpacket.tv_usec))
						gettimeofday(&timefirstpacket,0);
					*/

					if (llimitnb >= 0)
						llimitnb--;

					if (outputfd >= 0)
					{
						if (write_packet(outputfd,datagram.data,packet_len) < 0)
						{
							perror("write_output");
							return 1;
						}
					}
					report(datalink_layerph.id,network_layerph.id,transport_layerph.id);
					display_packet(headerfd,&datalink_layerph,&network_layerph,&transport_layerph,&datagram);
				}
				else
				{
					report(-1,-1,-1);
				}
				/*
				}
				else
				{
					if (opt_displayallpackets)
					{
						print_packetnb(headerfd,datagram.len);
					}
				}
				*/
			}
		}
	}

	if (outputfd >= 0)
		close(outputfd);
	
	stop_sniff(); 
	cleanup_sniff();

	return 0;
}

int stop_sniff()
{
	display_report(headerfd);

	return 0;
}

int cleanup_sniff()
{
	/* free(datagram.data); */
	/* free(datagram); */
	/* free(datalink_layerph); */
	/* free(network_layerph); */
	/* free(transport_layerph); */
	linked_list_free(list_filter);
	linked_list_free(list_display_packet);
	linked_list_free(list_display_report);
	linked_list_free(list_report);
	linked_list_free(list_error);
	ncache_free();
	default_free();
	if (curinputfd >= 0)
		close(curinputfd);
	if (curoutputfd >= 0)
		close(curoutputfd);

	return 0;
}

