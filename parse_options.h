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

#include "network/headers.h"
#include "network/my_types.h"

#define P_MISC -2 
#define P_ERROR -1
#define PO_NOARG 0
#define PO_ARG 1
#define PO_OPTIONALARGS 2

enum optid
{ 	
	OPT_ERROR,
	OPT_NULL,
	OPT_OK,
	OPT_OUTPUT,
	OPT_OUTPUTDATA,
	OPT_INPUTFILE,
	OPT_OUTPUTFILE,
	OPT_DSP_HELP,
	OPT_DSP_DISPLAYDATA,
	OPT_DSP_DISPLAYHEADER,
	OPT_DSP_DISPLAYPACKETS,
	OPT_DSP_DISPLAYALLPACKETS,
	OPT_DSP_SIMPLEDISPLAY,
	OPT_DSP_DISPLAYTLPROTO,
	OPT_DSP_DONTDISPLAYEMPTYSLP,
	OPT_DSP_DISPLAYNLPROTO,
	OPT_DSP_DISPLAYDLPROTO,
	OPT_DSP_DISPLAYHEXA,
	OPT_DSP_DONTDISPLAYPACKETS,
	OPT_DSP_VERSION,
	OPT_DSP_LICENSE,
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
	OPT_TYPE_DSP =  0x2
};

struct opt_dsp_type
{
	__u32 id;
};

struct opt_flt_type 
{
	int (*func_flt)(struct protocol_header *, struct protocol_header *, struct protocol_header *, __u8 *);
	unsigned int flt_size;
};

struct optlist 
{
	char name_short;
	char *name_long;
	enum optid id;
	int flags;
	int (*func_chk)(char *);
	int (*func_prs)(struct optlist *, char *);
	union optu {
		struct opt_dsp_type dsp;
		struct opt_flt_type flt;
	} u;
	enum opttype type;
};

int lookup_options(int argc, char **argv,struct optlist *optlist, struct optlist **elem);
int parse_options(int argc, char **argv);

__inline__ int prs_dsp_help(struct optlist *optl, char *val);
__inline__ int prs_dsp_version(struct optlist *optl, char *val);
__inline__ int prs_dsp_license(struct optlist *optl, char *val);
__inline__ int prs_dsp_simpledispla(struct optlist *optl, char *val);
__inline__ int prs_dsp_displaydata(struct optlist *optl, char *val);
__inline__ int prs_dsp_displayheader(struct optlist *optl, char *val);
__inline__ int prs_dsp_displaypackets(struct optlist *optl, char *val);
__inline__ int prs_dsp_displayallpackets(struct optlist *optl, char *val);
__inline__ int prs_dsp_displaydlproto(struct optlist *optl, char *val);
__inline__ int prs_dsp_displaynlproto(struct optlist *optl, char *val);
__inline__ int prs_dsp_displaytlproto(struct optlist *optl, char *val);
__inline__ int prs_dsp_dontdisplayemptyslp(struct optlist *optl, char *val);
__inline__ int prs_dsp_dontdisplaypackets(struct optlist *optl, char *val);
__inline__ int prs_dsp_displayhexa(struct optlist *optl, char *val);
__inline__ int prs_dsp_simpledisplay(struct optlist *optl, char *val);
__inline__ int prs_output(struct optlist *optl, char *val);
__inline__ int prs_outputdata(struct optlist *optl, char *val);
__inline__ int prs_inputfile(struct optlist *optl, char *val);
__inline__ int prs_outputfile(struct optlist *optl, char *val);
__inline__ int prs_flt_dstport(struct optlist *optl, char *val);
__inline__ int prs_flt_srcport(struct optlist *optl, char *val);
__inline__ int prs_flt_globalport(struct optlist *optl, char *val);
__inline__ int prs_flt_srcip(struct optlist *optl, char *val);
__inline__ int prs_flt_dstip(struct optlist *optl, char *val);
__inline__ int prs_flt_globalip(struct optlist *optl, char *val);
__inline__ int prs_flt_srcmac(struct optlist *optl, char *val);
__inline__ int prs_flt_dstmac(struct optlist *optl, char *val);
__inline__ int prs_flt_limitnb(struct optlist *optl, char *val);
__inline__ int prs_flt_timelimit(struct optlist *optl, char *val);
__inline__ int prs_flt_filterstr(struct optlist *optl, char *val);
__inline__ int prs_flt_filterregex(struct optlist *optl, char *val);
__inline__ int prs_flt_protocol(struct optlist *optl, char *val);
__inline__ int prs_flt_ethprotocol(struct optlist *optl, char *val);
__inline__ int prs_flt_ipprotocol(struct optlist *optl, char *val);

#endif

