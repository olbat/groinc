/* this file is a part of groinc
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

#include "parse_options.h"
#include "check_options.h"
#include "globals_args.h"
#include "globals_option.h"
#include "globals_filter.h"
#include "tools/memory_tools.h"
#include "tools/linked_list.h"
#include "network/protocols.h"
#include "misc.h"
#include "filter.h"


#define LOOKUP_SHORT(N,C) \
({ \
	int i, end; \
	end = OPT_NULL; \
	i = 0; \
	while ((end == OPT_NULL) && (N[i].id != OPT_END)) \
	{ \
		if (N[i].name_short == C) \
		{ \
			end = i; \
		} \
		i++; \
	} \
	end; \
})

#define LOOKUP_LONG(N,STR) \
({ \
	int i, end; \
	end = OPT_NULL; \
	i = 0; \
	while ((end == OPT_NULL) && (N[i].id != OPT_END)) \
	{ \
		if (!my_strcmp(N[i].name_long,STR)) \
		{ \
			end = i; \
		} \
		i++; \
	} \
	end; \
})

#define OPTLIST_SET_BASE(O,I,SH,LG,ID,FL,CK,PR) \
	(O+I)->name_short = SH; \
	(O+I)->name_long = LG; \
	(O+I)->id = ID; \
	(O+I)->flags = FL; \
	(O+I)->func_chk = CK; \
	(O+I)->func_prs = PR;

#define OPTLIST_SET_DSP(O,I,SH,LG,ID,FL,CK,PR,N) \
	OPTLIST_SET_BASE(O,I,SH,LG,ID,FL,CK,PR); \
	(O+I)->u.dsp.id = N; \
	(O+I)->type = OPT_TYPE_DSP; \
	I++;

#define OPTLIST_SET_FLT(O,I,SH,LG,ID,FL,CK,PR,F,SI) \
	OPTLIST_SET_BASE(O,I,SH,LG,ID,FL,CK,PR); \
	(O+I)->u.flt.func_flt = F; \
	(O+I)->u.flt.flt_size = SI; \
	(O+I)->type = OPT_TYPE_FLT; \
	I++;


#define OPTLIST_SIZE 34

/* Do not forget to increment the OPTLIST_SIZE when add an option */
void options_optlist_init(struct optlist *optl)
{
	/* >>> TODO: use hashtable instead of array */

	int i = 0;
	OPTLIST_SET_DSP(optl, i, 'o',	"outputdata",	OPT_OUTPUTDATA,	PO_ARG, chk_outputdata,	prs_outputdata,	0 );
	OPTLIST_SET_DSP(optl, i, 'O',	"output",	OPT_OUTPUT,	PO_ARG, chk_output,	prs_output, 0 );
	OPTLIST_SET_DSP(optl, i, 'r',	"read",		OPT_INPUTFILE,	PO_ARG, chk_inputfile,	prs_inputfile, 0 );
	OPTLIST_SET_DSP(optl, i, 'w',	"write",	OPT_OUTPUTFILE,	PO_ARG, chk_outputfile,	prs_outputfile, 0 );
	OPTLIST_SET_DSP(optl, i, 'a',	"displaydata",	OPT_DSP_DISPLAYDATA,	PO_NOARG, 0,	prs_dsp_displaydata, 0 );
	OPTLIST_SET_DSP(optl, i, 'A',	"simpledisplay",OPT_DSP_SIMPLEDISPLAY,	PO_NOARG, 0,	prs_dsp_simpledisplay, 0 );
	OPTLIST_SET_DSP(optl, i, 'v',	"verbose",	OPT_DSP_DISPLAYHEADER,	PO_NOARG, 0,	prs_dsp_displayheader, 0 );
	OPTLIST_SET_DSP(optl, i, 'n',	"displaypackets",OPT_DSP_DISPLAYPACKETS,	PO_NOARG, 0,	prs_dsp_displaypackets, 0 );
	OPTLIST_SET_DSP(optl, i, 'H',	"hexa",		OPT_DSP_DISPLAYHEXA,	PO_NOARG, 0,	prs_dsp_displayhexa, 0 );
	OPTLIST_SET_DSP(optl, i, 'h',	"help", 	OPT_DSP_HELP,		PO_NOARG, 0,	prs_dsp_help, 0 );
	OPTLIST_SET_DSP(optl, i, 'q',	"quiet",	OPT_DSP_DONTDISPLAYPACKETS,	PO_NOARG, 0,	prs_dsp_dontdisplaypackets, 0 );
	OPTLIST_SET_DSP(optl, i,  0,	"version",	OPT_DSP_VERSION,	PO_NOARG, 0,	prs_dsp_version, 0 );
	OPTLIST_SET_DSP(optl, i,  0,	"license",	OPT_DSP_LICENSE,	PO_NOARG, 0,	prs_dsp_license, 0 );
	OPTLIST_SET_DSP(optl, i, 'c',	"displayprotodatalink",	OPT_DSP_DISPLAYDLPROTO,	PO_NOARG, 0,	prs_dsp_displaydlproto, 0 );
	OPTLIST_SET_DSP(optl, i, 'b',	"displayprototransport",OPT_DSP_DISPLAYTLPROTO,	PO_NOARG, 0,	prs_dsp_displaytlproto, 0 );
	OPTLIST_SET_DSP(optl, i, 'B',	"displayprotonetwork",	OPT_DSP_DISPLAYNLPROTO,	PO_NOARG, 0,	prs_dsp_displaynlproto, 0 );
	OPTLIST_SET_DSP(optl, i, 'N',	"displayallpackets",	OPT_DSP_DISPLAYALLPACKETS,		PO_NOARG, 0,	prs_dsp_displayallpackets, 0 );
	OPTLIST_SET_DSP(optl, i, 'z',	"dontdisplayemptysp",	OPT_DSP_DONTDISPLAYEMPTYSLP,	PO_NOARG, 0,	prs_dsp_dontdisplayemptyslp, 0 );

	OPTLIST_SET_FLT(optl, i, 'd',	"destport",	OPT_FLT_DSTPORT,	PO_ARG, chk_flt_dstport,	prs_flt_dstport,		0, 0 );
	OPTLIST_SET_FLT(optl, i, 'D',	"destip",	OPT_FLT_DSTIP,		PO_ARG, chk_flt_dstip,	prs_flt_dstip,		flt_nl_ip_dst, 4 );
	OPTLIST_SET_FLT(optl, i, 's',	"sourceport",	OPT_FLT_SRCPORT,	PO_ARG, chk_flt_srcport,	prs_flt_srcport,		0, 0 );
	OPTLIST_SET_FLT(optl, i, 'S',	"sourceip",	OPT_FLT_SRCIP,		PO_ARG, chk_flt_srcip,	prs_flt_srcip,		flt_nl_ip_src, 4 );
	OPTLIST_SET_FLT(optl, i, 'f',	"filter",	OPT_FLT_FILTERSTR,	PO_ARG, chk_flt_filterstr,	prs_flt_filterstr,		0, 0 );
	OPTLIST_SET_FLT(optl, i, 'Q',	"protocol",	OPT_FLT_PROTOCOL,	PO_ARG, chk_flt_protocol,	prs_flt_protocol,		0, 0 );
	OPTLIST_SET_FLT(optl, i, 'p',	"ipprotocol", 	OPT_FLT_IPPROTOCOL,	PO_ARG, chk_flt_ipprotocol,	prs_flt_ipprotocol,		0, 0 );
	OPTLIST_SET_FLT(optl, i, 'P',	"ethprotocol",	OPT_FLT_ETHPROTOCOL,	PO_ARG, chk_flt_ethprotocol,	prs_flt_ethprotocol,		0, 0 );
	OPTLIST_SET_FLT(optl, i, 'F',	"filter-regex",	OPT_FLT_FILTERREGEX,	PO_ARG, chk_flt_filterregex,	prs_flt_filterregex,		0, 0 );
	OPTLIST_SET_FLT(optl, i, 'm',	"sourcemac",	OPT_FLT_SRCMAC,		PO_ARG, chk_flt_srcmac,	prs_flt_srcmac,		flt_dl_mac_src, 6 );
	OPTLIST_SET_FLT(optl, i, 'M',	"destmac",	OPT_FLT_DSTMAC,		PO_ARG, chk_flt_dstmac,	prs_flt_dstmac,		flt_dl_mac_dst, 6 );
	OPTLIST_SET_FLT(optl, i, 'l',	"limitnb",	OPT_FLT_LIMITNB,	PO_ARG, chk_flt_limitnb,	prs_flt_limitnb,		0, 0 );
	OPTLIST_SET_FLT(optl, i, 'g',	"globalport",	OPT_FLT_GLOBALPORT,	PO_ARG, chk_flt_globalport,	prs_flt_globalport,		0, 0 );
	OPTLIST_SET_FLT(optl, i, 'G',	"globalip",	OPT_FLT_GLOBALIP,	PO_ARG, chk_flt_globalip,	prs_flt_globalip,		flt_nl_ip_global, 4 );
	OPTLIST_SET_FLT(optl, i, 't',	"timelimit",	OPT_FLT_TIMELIMIT,	PO_ARG, chk_flt_timelimit,	prs_flt_timelimit,		0, 0 );
	OPTLIST_SET_DSP(optl, i, '\0',	"",		OPT_END,	PO_NOARG, 0,	0, 0 );
}

static char *value;

int lookup_options(int argc, char **argv,struct optlist *optlist, struct optlist **elem)
{
	static int args = 1;
	char *ptr;
	int end,ptri;
	end = OPT_NULL;
	if (args <= argc)
	{
		ptr = argv[args];
		if (*ptr == '-')
		{
			if (*(ptr + 1) == '-')
				ptri = LOOKUP_LONG(optlist,ptr + 2);
			else
				ptri = LOOKUP_SHORT(optlist,*(ptr + 1));

			if (ptri != OPT_NULL)
			{
				if (optlist[ptri].flags == PO_ARG)
				{
					if (args < argc)
					{
						value = argv[++args];
					}
					else
					{
						value = argv[args];
						return OPT_ERROR;
					}
				}
				args++;
				end = OPT_OK;
				*elem = optlist + ptri;
			}
			else
			{
				value = argv[args];
				end = OPT_ERROR;
			}
		}
		else
		{
			value = argv[args];
			end = OPT_ERROR;
		}
	}
	else
	{
		end = OPT_END;
	}
	return end;
}

int parse_options(int argc, char **argv)
{
	int o;
	struct optlist *optl;
	struct optlist po_optlist[OPTLIST_SIZE];
	options_optlist_init(po_optlist);
	while ((o = lookup_options(argc,argv,po_optlist,&optl)) != OPT_END)
	{
		if (o != OPT_OK)
		{
			po_error = value;
			return P_ERROR;
		}
		else 
		{
			if (optl->func_chk)
			{
				if (optl->func_chk(value) != OPT_OK)
				{
					/* >>> TODO : specific check error */
					po_error = value;
					return P_ERROR;
				}
			}
			if (optl->func_prs)
			{
				if ((o = optl->func_prs(optl,value)) != OPT_OK)
					return o;
			}
		}
	}
	return 0;
}

__inline__ int prs_dsp_help(struct optlist *optl, char *val)
{
	return P_ERROR;
}

__inline__ int prs_dsp_version(struct optlist *optl, char *val)
{
	po_misc = MISCNO_VERSION;
	return P_MISC;
}

__inline__ int prs_dsp_license(struct optlist *optl, char *val)
{
	po_misc = MISCNO_LICENSE;
	return P_MISC;
}

__inline__ int prs_dsp_simpledispla(struct optlist *optl, char *val)
{
	opt_simpledisplay = 1;
	return OPT_OK;
}

__inline__ int prs_dsp_displaydata(struct optlist *optl, char *val)
{
	opt_displaydata = 1;
	return OPT_OK;
}

__inline__ int prs_dsp_displayheader(struct optlist *optl, char *val)
{
	opt_displayheader = 1;
	return OPT_OK;
}

__inline__ int prs_dsp_displaypackets(struct optlist *optl, char *val)
{
	opt_displaypackets = 1;
	return OPT_OK;
}

__inline__ int prs_dsp_displayallpackets(struct optlist *optl, char *val)
{
	opt_displayallpackets = 1;
	return OPT_OK;
}

__inline__ int prs_dsp_displaydlproto(struct optlist *optl, char *val)
{
	opt_displaydlproto = 1;
	return OPT_OK;
}

__inline__ int prs_dsp_displaynlproto(struct optlist *optl, char *val)
{
	opt_displaynlproto = 1;
	return OPT_OK;
}

__inline__ int prs_dsp_displaytlproto(struct optlist *optl, char *val)
{
	opt_displaytlproto = 1;
	return OPT_OK;
}

__inline__ int prs_dsp_dontdisplayemptyslp(struct optlist *optl, char *val)
{
	opt_ndisplayemptyslp = 1;
	return OPT_OK;
}

__inline__ int prs_dsp_dontdisplaypackets(struct optlist *optl, char *val)
{
	opt_ndisplaypackets = 1;
	return OPT_OK;
}

__inline__ int prs_dsp_displayhexa(struct optlist *optl, char *val)
{
	opt_displayhexa = 1;
	return OPT_OK;
}

__inline__ int prs_dsp_simpledisplay(struct optlist *optl, char *val)
{
	opt_simpledisplay = 1;
	return OPT_OK;
}

__inline__ int prs_output(struct optlist *optl, char *val)
{
	opt_output = 1;
	output = val;
	return OPT_OK;
}

__inline__ int prs_outputdata(struct optlist *optl, char *val)
{
	opt_outputdata = 1;
	outputdata = val;
	return OPT_OK;
}

__inline__ int prs_inputfile(struct optlist *optl, char *val)
{
	inputfile = val;
	return OPT_OK;
}

__inline__ int prs_outputfile(struct optlist *optl, char *val)
{
	outputfile = val;
	return OPT_OK;
}

__inline__ int prs_flt_dstport(struct optlist *optl, char *val)
{
	fdestport = val;
	/* if (chk_... == OK) ... */
	return OPT_OK;
}

__inline__ int prs_flt_srcport(struct optlist *optl, char *val)
{
	fsourceport = val;
	return OPT_OK;
}

__inline__ int prs_flt_globalport(struct optlist *optl, char *val)
{
	fglobalport = val;
	return OPT_OK;
}

__inline__ int prs_flt_srcip(struct optlist *optl, char *val)
{
	fsourceip = val;
	__u32 tmp = ipv4_aton(val);
	linked_list_add(list_filter,
			linked_list_flt_value_init(optl->u.flt.func_flt,
						   (__u8 *)&tmp,
						   optl->u.flt.flt_size));
	return OPT_OK;
}

__inline__ int prs_flt_dstip(struct optlist *optl, char *val)
{
	fdestip = val;
	__u32 tmp = ipv4_aton(val);
	linked_list_add(list_filter,
			linked_list_flt_value_init(optl->u.flt.func_flt,
						   (__u8 *)&tmp,
						   optl->u.flt.flt_size));
	return OPT_OK;
}

__inline__ int prs_flt_globalip(struct optlist *optl, char *val)
{
	fglobalip = val;
	__u32 tmp = ipv4_aton(val);
	linked_list_add(list_filter,
			linked_list_flt_value_init(optl->u.flt.func_flt,
						   (__u8 *)&tmp,
						   optl->u.flt.flt_size));
	return OPT_OK;
}

__inline__ int prs_flt_srcmac(struct optlist *optl, char *val)
{
	fsourcemac = val;
	__u8 tmp[MAC_STR_SIZE];
	mac_aton(val,tmp);
	linked_list_add(list_filter,
			linked_list_flt_value_init(optl->u.flt.func_flt,
						   (__u8 *)&tmp,
						   optl->u.flt.flt_size));
	return OPT_OK;
}

__inline__ int prs_flt_dstmac(struct optlist *optl, char *val)
{
	fdestmac = val;
	__u8 tmp[MAC_STR_SIZE];
	mac_aton(val,tmp);
	linked_list_add(list_filter,
			linked_list_flt_value_init(optl->u.flt.func_flt,
						   (__u8 *)&tmp,
						   optl->u.flt.flt_size));
	return OPT_OK;
}

__inline__ int prs_flt_limitnb(struct optlist *optl, char *val)
{
	flimitnb = val;
	return OPT_OK;
}

__inline__ int prs_flt_timelimit(struct optlist *optl, char *val)
{
	ftimelimit = val;
	return OPT_OK;
}

__inline__ int prs_flt_filterstr(struct optlist *optl, char *val)
{
	filterstr = val;
	return OPT_OK;
}

__inline__ int prs_flt_filterregex(struct optlist *optl, char *val)
{
	filterregexstr = val;
	return OPT_OK;
}

__inline__ int prs_flt_protocol(struct optlist *optl, char *val)
{
	protoname = val;
	return OPT_OK;
}

__inline__ int prs_flt_ethprotocol(struct optlist *optl, char *val)
{
	ethprotoname = val;
	return OPT_OK;
}

__inline__ int prs_flt_ipprotocol(struct optlist *optl, char *val)
{
	ipprotoname = val;
	return OPT_OK;
}
