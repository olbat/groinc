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

#include "usage.h"
#include "globals_option.h"
#include "prints.h"

int print_usage(char *cmdname)
{
	print_format(headerfd,
	"usage : %s [-option] [-DSG ip addr] [-dsg port number] [-oO filename] [-f string] [-F regex] [-pP protocolname] [-l number] [-t time(seconds))n"
	"  -a	--displaydata		Display the received (application layer) data\n"
	"  -A 	--simpledisplay		Enable the simple display\n"
	"  -b 	--displayprototransport	Display the name/size of the transport layer protocol\n"
	"  -B 	--displayprotonetwork	Display the name/size of the network layer protocol\n"
	"  -c 	--displayprotodatalink	Display the name/size of the datalink layer protocol\n"
	"  -d 	--destport 		The destination port number to be scan\n"
	"  -D 	--destip		The destination IP adress to be scan\n"
	"  -f 	--filter		Enable (application layer) filter with a string\n"
	"  -F	--filter-regex		Enable (application layer) filter with a regular expression (POSIX)\n"
	"  -g	--globalport		The source or destination port to be scan\n"
	"  -G	--globalip		The source or destination IP to be scan\n"
	"  -h 	--help			Display the help\n"
	"  -H	--hexa			Display data in hexadecimal\n"
	"  -l	--limitnb		Give the number of packets to display before stop the sniffing\n"
	"  -m	--sourcemac		The MAC adress of the source\n"
	"  -M	--destmac		The MAC adress of the destination\n"
	"  -n 	--displaypackets	Display the length of the received packets\n"
	"  -N 	--displayallpackets	Display the length of all the not filtred received packets\n"
	"  -o 	--outputdata		The output file name (for the data)\n"
	"  -O 	--output		The output file name (for the headers)\n"
	"  -q 	--quiet			Do not display packets\n"
	"  -Q 	--protocol		Specify the datalink layer protocol name\n"
	"  -p 	--ipprotocol		Specify the transport layer (ip)protocol name\n"
	"  -P 	--ethprotocol		Specify the network layer (ethernet)protocol name\n"
	"  -r	--read			Load data from a saved file\n"	
	"  -s 	--sourceport 		The source port number to be scan\n"
	"  -S 	--sourceip		The source IP adress to be scan\n"
	"  -t 	--timelimit 		Give a limit of time to sniff (seconds)\n"
	"  -v 	--verbose		Verbose mode, display the headers of the scanned packets\n"
	"  -w	--write			Save the data in an outputfile\n"
	"  -z	--dontdisplayemptysp	Dont display the empty application layer packets\n"
	"  	--version		Display the version of the software\n"
	"  	--license		Display the copyright informations\n",
	cmdname);
	return 0;
}

void print_error_opt(char *error)
{
	print_format(headerfd,"Error, unknown option '%s'.\n",error);
}

void print_error_setting(char *error)
{
	print_format(headerfd,"Error invalid argument '%s'.\n",error);
}

