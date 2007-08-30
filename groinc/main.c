/* Groinc
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

#include <sys/socket.h>
#include <linux/if_ether.h>
#include <fcntl.h>
#include <stdio.h> /* stdin/stdout */
#include <netinet/in.h>
#include <signal.h>
#include <sys/time.h>

#include "network/protocols.h"
#include "network/my_types.h"
#include "parse_options.h"
#include "check_options.h"
#include "error.h"
#include "defaults.h"
#include "tools/linked_list.h"
#include "sniffer.h"
#include "events.h"
#include "misc.h"

#define OUTPUT_HEADER_FILE_DEFAULT stdout
#define OUTPUT_DATA_FILE_DEFAULT stdout


/* declaration of the globals */

long int	llimitnb;

char	*outputdata,
	*output,
	*inputfile,
	*outputfile;

int 	datafd,
	headerfd;

char	sniffer_stop,
	opt_output,
	opt_outputdata;
	
enum miscno	po_misc;

struct timeval timelimit;

struct linked_list 	*list_filter,
			*list_display_packet,
			*list_display_report,
			*list_report,
			*list_error;

struct default_report default_report;

#define MAIN_CLEANUP() \
__extension__ \
({ \
	linked_list_free(list_filter); \
	linked_list_free(list_display_packet); \
	linked_list_free(list_display_report); \
	linked_list_free(list_report); \
	linked_list_free(list_error); \
	default_free(); \
})

#define MAIN_QUIT(V) \
__extension__ \
({ \
	MAIN_CLEANUP(); \
	return V; \
})

int main(int argc, char **argv)
{
	int inputfd,outputfd;
	
	/* signals settings */
	signal(SIGINT,event_stop);
	signal(SIGQUIT,event_stop);

	signal(SIGHUP,event_kill);
	signal(SIGTSTP,event_kill);
	signal(SIGABRT,event_kill);
	signal(SIGTERM,event_kill);
	signal(SIGFPE,event_kill);
	signal(SIGILL,event_kill);
	signal(SIGSEGV,event_kill);

	sniffer_stop = 0;

	outputdata = "";
	output = "";
	inputfile = "";
	outputfile = "";
	llimitnb = -1;
	
	opt_output = 0;
	opt_outputdata = 0;

	timelimit.tv_sec = 0;

	list_filter = linked_list_init();
	list_display_packet = linked_list_init();
	list_display_report = linked_list_init();
	list_report = linked_list_init();
	list_error = linked_list_init();

	if (opt_output != 0)
		headerfd = open(output,O_CREAT|O_WRONLY);
	else
		headerfd = (int)OUTPUT_HEADER_FILE_DEFAULT;

	if (opt_outputdata != 0)
		datafd = open(outputdata,O_CREAT|O_WRONLY);
	else
		datafd = (int)OUTPUT_DATA_FILE_DEFAULT;
	
	if (parse_options(argc-1,argv) == P_MISC)
	{
		print_misc(po_misc);
		MAIN_QUIT(0);
	}
	
	if (error_display())
		MAIN_QUIT(1);

	/* if ((inputfd = socket(AF_INET,SOCK_RAW,proto)) < 0) */
	if (*outputfile)
		outputfd = open(outputfile,O_CREAT|O_WRONLY|O_TRUNC);
	else
		outputfd = -1;

	if (*inputfile)
		inputfd = open(inputfile,O_RDONLY);
	else
		inputfd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));

	if (inputfd < 0)
	{
		perror("socket/open");
		MAIN_QUIT(1);
	}
	else
	{
		start_sniff(inputfd,outputfd);
	}
	return 0;
}

