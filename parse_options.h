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

#ifndef _PARSE_OPTIONS_H
#define _PARSE_OPTIONS_H

#include <asm/types.h>

#include "network/headers.h"

#define P_MISC -2 
#define P_ERROR -1
#define PO_NOARG 0
#define PO_ARG 1
#define PO_OPTIONALARGS 2

enum optid
{ 	
	OPT_ERROR = -2,
	OPT_NULL = -1,
	OPT_OK = 0,

	OPT_OUTPUT = 1,
	OPT_OUTPUTDATA,
	OPT_INPUTFILE,
	OPT_OUTPUTFILE,
	
	OPT_DSP_PKT_HELP,
	OPT_DSP_PKT_DISPLAYDATA,
	OPT_DSP_PKT_DISPLAYHEADER,
	OPT_DSP_PKT_DISPLAYPACKETS,
	OPT_DSP_PKT_DISPLAYALLPACKETS,
	OPT_DSP_PKT_SIMPLEDISPLAY,
	OPT_DSP_PKT_DISPLAYTLPROTO,
	OPT_DSP_PKT_DONTDISPLAYEMPTYSL,
	OPT_DSP_PKT_DISPLAYNLPROTO,
	OPT_DSP_PKT_DISPLAYDLPROTO,
	OPT_DSP_PKT_DISPLAYHEXA,
	OPT_DSP_PKT_DONTDISPLAYPACKETS,
	OPT_DSP_PKT_VERSION,
	OPT_DSP_PKT_LICENSE,

	OPT_DSP_RPT_START,
	OPT_DSP_RPT_TIMETOT,
	OPT_DSP_RPT_COUNTPACKETSTOT,
	OPT_DSP_RPT_COUNTPACKETSFILTRED,
	OPT_DSP_RPT_END,
	
	OPT_FLT_DSTPORT,
	OPT_FLT_DSTIP,
	OPT_FLT_SRCPORT,
	OPT_FLT_SRCIP,
	OPT_FLT_FILTERSTR,
	OPT_FLT_IPPROTOCOL,
	OPT_FLT_FILTERREGEX,
	OPT_FLT_ETHPROTOCOL,
	OPT_FLT_PROTOCOL,
	OPT_FLT_SRCMAC,
	OPT_FLT_DSTMAC,
	OPT_FLT_GLOBALIP,
	OPT_FLT_GLOBALPORT,
	OPT_FLT_LIMITNB,
	OPT_FLT_TIMELIMIT,
	OPT_END
};

enum opttype
{
	OPT_TYPE_FLT =  0x1,
	OPT_TYPE_DSP_PKT =  0x2,
	OPT_TYPE_DSP_RPT = 0x4
};

struct opt_dsp_rpt_type
{
	void (*func_dsp_rpt)(int, __u8 *);
	void (*func_rpt)(__u8 *,long int, long int, long int);
};

struct opt_dsp_pkt_type
{
	void (*func_dsp_pkt)(int, struct protocol_header *, struct protocol_header *, struct protocol_header *, struct data *);
};

struct opt_flt_type 
{
	int (*func_flt)(struct protocol_header *, struct protocol_header *, struct protocol_header *, struct data *, __u8 *);
};

struct linked_list_opt_value 
{
	char name_short;
	char *name_long;
	enum optid id;
	int flags;
	int (*func_chk)(char *);
	int (*func_prs)(struct linked_list_opt_value *, char *);
	union optu {
		struct opt_dsp_pkt_type dsp_pkt;
		struct opt_dsp_rpt_type dsp_rpt;
		struct opt_flt_type flt;
	} u;
	enum opttype type;
};

#include "tools/linked_list.h"

int lookup_options(int argc, char **argv, struct linked_list *optlist, struct linked_list_opt_value **elem);
int parse_options(int argc, char **argv);

#endif
