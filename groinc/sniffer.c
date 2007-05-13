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

#include "sniffer.h"

#include <malloc.h>
#include <stdio.h> /* needed for perror */
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/time.h>

#include "tools/memory_tools.h"
#include "packet_inout.h"
#include "network/parsers.h"
#include "network/printers.h"
#include "prints.h"
#include "filter.h"
#include "globals_filter.h"
#include "globals_option.h"

#define DATAGRAM_SIZE 4096

static struct data *datagram;
static struct protocol_header *datalink_layerph;
static struct protocol_header *network_layerph;
static struct protocol_header *transport_layerph;
static int curinputfd, curoutputfd;

int start_sniff(int inputfd,int outputfd)
{
	if (inputfd < 0)
		return 1;

	char end;
	int packet_len;
	fd_set readfds;
	struct timeval timecur;

	FD_ZERO(&readfds);
	FD_SET(inputfd,&readfds);
	
	curinputfd = inputfd;
	curoutputfd = outputfd;
	
	datagram = (struct data *) malloc(sizeof(struct data)); 
	my_memset((char *)datagram,0,sizeof(struct data));
	datagram->data = (char *) malloc(sizeof(char)*DATAGRAM_SIZE);
	my_memset(datagram->data,0,sizeof(char)*DATAGRAM_SIZE);
	
	datalink_layerph = (struct protocol_header *) malloc(sizeof(struct protocol_header));
	network_layerph = (struct protocol_header *) malloc(sizeof(struct protocol_header));
	transport_layerph = (struct protocol_header *) malloc(sizeof(struct protocol_header));
	
	end = 0;

	gettimeofday(&timestart,0);

	while ((!sniffer_stop) && (!end) && (llimitnb))
	{
		if (timelimit.tv_sec)
		{
			gettimeofday(&timecur,0);
			if (timecur.tv_sec >= timelimit.tv_sec)
			{
				if (timecur.tv_usec >= timelimit.tv_usec)
					end = 1;
			}
		}
		if ((!end) && (select(inputfd+1,&readfds,NULL,NULL,NULL) > 0))
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
					packetstot++;

					datagram->len = 0;
					datagram->totlen = packet_len;

					datalink_layerph->len = 0;
					network_layerph->len = 0;
					transport_layerph->len = 0;
					
					parse_datalink_layer(datagram,datalink_layerph,network_layerph);
					if (network_layerph->len >= 0)
						parse_network_layer(datagram,network_layerph,transport_layerph);
					if (transport_layerph->len >= 0)
						parse_transport_layer(datagram,transport_layerph);

					if ((datagram->len < datagram->totlen) || (!opt_ndisplayemptyslp)) /* if the session layer is empty */
					{
						if (filter(datalink_layerph,network_layerph,transport_layerph,datagram) != 0)
						{
							if ((!timefirstpacket.tv_sec) && (!timefirstpacket.tv_usec))
								gettimeofday(&timefirstpacket,0);
							packetsfiltred++;
							if (llimitnb >= 0)
							{
								llimitnb--;
							}
							if (outputfd >= 0)
							{
								if (write_packet(outputfd,datagram->data,packet_len) < 0)
								{
									perror("write_output");
									return 1;
								}
							}
							if (!opt_ndisplaypackets)
							{
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
								else if (opt_displayheader)
								{
									if (opt_displayhexa)
									{
										print_protoproto(headerfd,datalink_layerph);
										print_hexa(headerfd,datalink_layerph->header,datalink_layerph->len);
										
										print_newline(headerfd);
										print_ethproto(headerfd,network_layerph);
										print_hexa(headerfd,network_layerph->header,network_layerph->len);
										
										print_newline(headerfd);
										print_ipproto(headerfd,transport_layerph);
										print_hexa(headerfd,transport_layerph->header,transport_layerph->len);
										print_newline(headerfd);
									}
									else
									{
										print_datalink_layer(headerfd,datalink_layerph);
										print_network_layer(headerfd,network_layerph);
										print_transport_layer(headerfd,transport_layerph);
									}
								}
								if (opt_displaydata)
								{
									if (opt_displayhexa)
									{
										print_packetnb(headerfd,(datagram->totlen - datagram->len));
										print_hexa(headerfd,(datagram->data + datagram->len),(datagram->totlen - datagram->len));
									}
									else
									{
										print_data(datafd,datagram);
									}
								}
								if (!opt_displaypackets && !opt_simpledisplay && !opt_displayheader && !opt_displaydata)
								{
									print_ipproto(headerfd,transport_layerph);
									print_simple(headerfd,network_layerph,transport_layerph);
									if (opt_displayhexa)
										print_hexa(headerfd,(datagram->data + datagram->len),(datagram->totlen - datagram->len));
									else
										print_data(datafd,datagram);
								}
								print_newline(headerfd);
								print_separator(headerfd);
								print_newline(headerfd);
							}
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
	}

	close(outputfd);
	
	stop_sniff(); 
	cleanup_sniff();

	return 0;
}

int stop_sniff()
{
	struct timeval timecur;
	time_t totsec, filsec;
	suseconds_t totusec, filusec;

	gettimeofday(&timecur,0);
	
	totsec = (timecur.tv_sec - timestart.tv_sec);
	if ((totusec = (timecur.tv_usec - timestart.tv_usec)) < 0)
	{
		totsec--;
		totusec = (1000000 - timestart.tv_usec) + timecur.tv_usec;
	}
	if ((timefirstpacket.tv_sec) && (timefirstpacket.tv_usec))
	{
		filsec = (timecur.tv_sec - timefirstpacket.tv_sec);
		if ((filusec = (timecur.tv_usec - timefirstpacket.tv_usec)) < 0)
		{
			filsec--;
			filusec = (1000000 - timefirstpacket.tv_usec) + timecur.tv_usec;
		}
	}
	else
	{
		filsec = 0;
		filusec = 0;
	}
	
	print_format(headerfd,  "\nTime - [total: %hds %hdms] [since first filtred packet: %hds %hdms]"
				"\nPackets - [total: %hd packets] [filtred: %hd packets]\n\n",
				totsec,(totusec / 1000),filsec,(filusec / 1000),packetstot,packetsfiltred);

	return 0;
}

int cleanup_sniff()
{
	free(datagram->data);
	free(datagram);
	free(datalink_layerph);
	free(network_layerph);
	free(transport_layerph);
	close(curinputfd);
	close(curoutputfd);

	return 0;
}

