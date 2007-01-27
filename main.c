/* Groinc
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

#include <sys/socket.h>
#include <linux/if_ether.h>
#include <fcntl.h>
#include <stdio.h>
#include <regex.h>
#include <netinet/in.h>

#include "network/protocols.h"
#include "network/my_types.h"
#include "parse_options.h"
#include "check_options.h"
#include "usage.h"
#include "sniffer.h"
#include "tools/network_tools.h" /* to be removed */

#define OUTPUT_HEADER_FILE_DEFAULT stdout
#define OUTPUT_DATA_FILE_DEFAULT stdout


/* declaration of the globals */

__u16 	ldestport,
	lsourceport,
	lglobalport;
__u32	lsourceip,
	ldestip,
	lglobalip;
__u8 	*lsourcemac,
	*ldestmac;
char	*fsourcemac,
	*fdestmac,
 	*fsourceip,
	*fdestip,
	*fglobalip,
	*fsourceport,
	*fdestport,
	*fglobalport,
	*outputdata,
	*output,
	*po_error,
	*co_error,
	*filterstr,
	*filterregexstr,
	*protoname,
	*ethprotoname,
 	*ipprotoname,
	*inputfile,
	*outputfile;
regex_t filterregex;
unsigned char proto,ethproto,ipproto;
int datafd,
    headerfd;

char	opt_displaydlproto,
	opt_displaynlproto,
	opt_displaytlproto,
	opt_simpledisplay,
	opt_displaydata,
	opt_displayheader,
	opt_displaypackets,
	opt_displayallpackets,
	opt_output,
	opt_outputdata,
	opt_ndisplayemptyslp;
	

int main(int argc, char **argv)
{
	int inputfd,outputfd;
	
	/* initialization of the options var */
	lsourcemac = mac_aton("00:00:00:00:00:00");
	ldestmac = mac_aton("00:00:00:00:00:00");
	fdestmac = "";
	fsourcemac = "";
	ldestport = 0;
	fdestip = "";
	ldestip = 0;
	fglobalip = "";
	lglobalip = 0;
	fdestport = "";
	fglobalport = "";
	lglobalport = 0;
	fsourceport = "";
	lsourceport = 0;
	fsourceip = "";
	lsourceip = 0;
	outputdata = "";
	output = "";
	po_error = "";
	co_error = "";
	filterstr = "";
	filterregexstr = "";
	proto = PROTO_ETHER;
	protoname = "";
	ethproto = ETHPROTO_IP;
	ethprotoname = "";
	ipproto = IPPROTO_RAW;
	ipprotoname = "";
	inputfile = "";
	outputfile = "";
	
	opt_displaydlproto = 0;
	opt_displaynlproto = 0;
	opt_displaytlproto = 0;
	opt_simpledisplay = 0;
	opt_displaydata = 0;
	opt_displayheader = 0;
	opt_displaypackets = 0;
	opt_displayallpackets = 0;
	opt_output = 0;
	opt_outputdata = 0;
	opt_ndisplayemptyslp = 0;
	
	if (argc <= 1)
	{
		print_usage(argv[0]);
		return 1;
	}
	if (parse_options(argc-1,argv) == P_ERROR)
	{
		if (*po_error != 0)
		{
			print_error_opt(po_error);
			print_usage(argv[0]);
			return 1;
		}
		else
		{
			print_usage(argv[0]);
			return 0;
		}
	}
	if (check_options())
	{
		if (*co_error)
		{
			print_error_setting(co_error);
			print_usage(argv[0]);
			return 1;
		}
	}
	
	if (opt_output != 0)
	{
		headerfd = open(output,O_CREAT|O_WRONLY);
	}
	else
	{
		headerfd = (int)OUTPUT_HEADER_FILE_DEFAULT;
	}
	if (opt_outputdata != 0)
	{
		datafd = open(outputdata,O_CREAT|O_WRONLY);
	}
	else
	{
		datafd = (int)OUTPUT_DATA_FILE_DEFAULT;
	}
	
	/* if ((inputfd = socket(AF_INET,SOCK_RAW,proto)) < 0) */
	if (*outputfile)
	{
		outputfd = open(outputfile,O_CREAT|O_WRONLY|O_TRUNC);
	}
	else
		outputfd = -1;
	if (*inputfile)
		inputfd = open(inputfile,O_RDONLY);
	else
		inputfd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	if (inputfd < 0)
	{
		perror("socket/open");
		return 1;
	}
	else
	{
		sniff(inputfd,outputfd);
	}
	return 0;
}

