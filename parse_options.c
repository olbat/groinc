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
#include "tools/math_tools.h"
#include "tools/linked_list.h"
#include "network/protocols.h"
#include "misc.h"
#include "filter.h"

static char *value;

#define LOOKUP_SHORT(N,C,P) \
({ \
	int end; \
	struct linked_list *p; \
	p = N; \
	end = OPT_NULL; \
	while ((p) && (end == OPT_NULL)) \
	{ \
		if (p->value->u.opt->name_short == C) \
		{ \
			P = p->value; \
			end = OPT_OK; \
		} \
		p = p->next; \
	} \
	end; \
})

#define LOOKUP_LONG(N,S,P) \
({ \
	int end; \
	struct linked_list *p; \
	p = N; \
	end = OPT_NULL; \
	while ((p) && (end == OPT_NULL)) \
	{ \
		if (!my_strcmp(p->value->u.opt->name_long,(S))) \
		{ \
			P = p->value; \
			end = OPT_OK; \
		} \
		p = p->next; \
	} \
	end; \
})

#define OPTLIST_SET_DSP(O,SH,LG,ID,FL,CK,PR,N) \
	linked_list_add(O,linked_list_opt_value_init_dsp(SH,LG,ID,FL,CK,PR,N));
	/* hashtable_add(O,LG,hashtable_opt_value_init_dsp(SH,LG,ID,FL,CK,PR,N)); */

#define OPTLIST_SET_FLT(O,SH,LG,ID,FL,CK,PR,F,SI) \
	linked_list_add(O,linked_list_opt_value_init_flt(SH,LG,ID,FL,CK,PR,F,SI));
	/* hashtable_add(O,LG,hashtable_opt_value_init_flt(SH,LG,ID,FL,CK,PR,F,SI)); */

/* Do not forget to increment the OPTLIST_SIZE when add an option */
/* void options_optlist_init(struct linked_list_opt_value *optl) */
#define OPTLIST_INIT(OPTL) \
	OPTLIST_SET_DSP(OPTL, 'o',	"outputdata",	OPT_OUTPUTDATA,	PO_ARG, chk_outputdata,	prs_outputdata,	0 ); \
	OPTLIST_SET_DSP(OPTL, 'O',	"output",	OPT_OUTPUT,	PO_ARG, chk_output,	prs_output, 0 ); \
	OPTLIST_SET_DSP(OPTL, 'r',	"read",		OPT_INPUTFILE,	PO_ARG, chk_inputfile,	prs_inputfile, 0 ); \
	OPTLIST_SET_DSP(OPTL, 'w',	"write",	OPT_OUTPUTFILE,	PO_ARG, chk_outputfile,	prs_outputfile, 0 ); \
	OPTLIST_SET_DSP(OPTL, 'a',	"displaydata",	OPT_DSP_DISPLAYDATA,	PO_NOARG, 0,	prs_dsp_displaydata, 0 ); \
	OPTLIST_SET_DSP(OPTL, 'A',	"simpledisplay",OPT_DSP_SIMPLEDISPLAY,	PO_NOARG, 0,	prs_dsp_simpledisplay, 0 ); \
	OPTLIST_SET_DSP(OPTL, 'v',	"verbose",	OPT_DSP_DISPLAYHEADER,	PO_NOARG, 0,	prs_dsp_displayheader, 0 ); \
	OPTLIST_SET_DSP(OPTL, 'n',	"displaypackets",OPT_DSP_DISPLAYPACKETS,	PO_NOARG, 0,	prs_dsp_displaypackets, 0 ); \
	OPTLIST_SET_DSP(OPTL, 'H',	"hexa",		OPT_DSP_DISPLAYHEXA,	PO_NOARG, 0,	prs_dsp_displayhexa, 0 ); \
	OPTLIST_SET_DSP(OPTL, 'h',	"help", 	OPT_DSP_HELP,		PO_NOARG, 0,	prs_dsp_help, 0 ); \
	OPTLIST_SET_DSP(OPTL, 'q',	"quiet",	OPT_DSP_DONTDISPLAYPACKETS,	PO_NOARG, 0,	prs_dsp_dontdisplaypackets, 0 ); \
	OPTLIST_SET_DSP(OPTL,  0,	"version",	OPT_DSP_VERSION,	PO_NOARG, 0,	prs_dsp_version, 0 ); \
	OPTLIST_SET_DSP(OPTL,  0,	"license",	OPT_DSP_LICENSE,	PO_NOARG, 0,	prs_dsp_license, 0 ); \
	OPTLIST_SET_DSP(OPTL, 'c',	"displayprotodatalink",	OPT_DSP_DISPLAYDLPROTO,	PO_NOARG, 0,	prs_dsp_displaydlproto, 0 ); \
	OPTLIST_SET_DSP(OPTL, 'b',	"displayprototransport",OPT_DSP_DISPLAYTLPROTO,	PO_NOARG, 0,	prs_dsp_displaytlproto, 0 ); \
	OPTLIST_SET_DSP(OPTL, 'B',	"displayprotonetwork",	OPT_DSP_DISPLAYNLPROTO,	PO_NOARG, 0,	prs_dsp_displaynlproto, 0 ); \
	OPTLIST_SET_DSP(OPTL, 'N',	"displayallpackets",	OPT_DSP_DISPLAYALLPACKETS,		PO_NOARG, 0,	prs_dsp_displayallpackets, 0 ); \
	OPTLIST_SET_DSP(OPTL, 'z',	"dontdisplayemptysp",	OPT_DSP_DONTDISPLAYEMPTYSLP,	PO_NOARG, 0,	prs_dsp_dontdisplayemptyslp, 0 ); \
 \
	OPTLIST_SET_FLT(OPTL, 'd',	"destport",	OPT_FLT_DSTPORT,	PO_ARG, chk_flt_dstport,	prs_flt_dstport,	flt_tl_port_dst, 2 ); \
	OPTLIST_SET_FLT(OPTL, 'D',	"destip",	OPT_FLT_DSTIP,		PO_ARG, chk_flt_dstip,		prs_flt_dstip,		flt_nl_ip_dst, 4 ); \
	OPTLIST_SET_FLT(OPTL, 's',	"sourceport",	OPT_FLT_SRCPORT,	PO_ARG, chk_flt_srcport,	prs_flt_srcport,	flt_tl_port_src, 2 ); \
	OPTLIST_SET_FLT(OPTL, 'S',	"sourceip",	OPT_FLT_SRCIP,		PO_ARG, chk_flt_srcip,		prs_flt_srcip,		flt_nl_ip_src, 4 ); \
	OPTLIST_SET_FLT(OPTL, 'f',	"filter",	OPT_FLT_FILTERSTR,	PO_ARG, chk_flt_filterstr,	prs_flt_filterstr,	0, 0 ); \
	OPTLIST_SET_FLT(OPTL, 'Q',	"protocol",	OPT_FLT_PROTOCOL,	PO_ARG, chk_flt_protocol,	prs_flt_protocol,	flt_dl_protocol, 4 ); \
	OPTLIST_SET_FLT(OPTL, 'p',	"ipprotocol", 	OPT_FLT_IPPROTOCOL,	PO_ARG, chk_flt_ipprotocol,	prs_flt_ipprotocol,	flt_tl_protocol, 4 ); \
	OPTLIST_SET_FLT(OPTL, 'P',	"ethprotocol",	OPT_FLT_ETHPROTOCOL,	PO_ARG, chk_flt_ethprotocol,	prs_flt_ethprotocol,	flt_nl_protocol, 4 ); \
	OPTLIST_SET_FLT(OPTL, 'F',	"filter-regex",	OPT_FLT_FILTERREGEX,	PO_ARG, chk_flt_filterregex,	prs_flt_filterregex,	0, 0 ); \
	OPTLIST_SET_FLT(OPTL, 'm',	"sourcemac",	OPT_FLT_SRCMAC,		PO_ARG, chk_flt_srcmac,		prs_flt_srcmac,		flt_dl_mac_src, 6 ); \
	OPTLIST_SET_FLT(OPTL, 'M',	"destmac",	OPT_FLT_DSTMAC,		PO_ARG, chk_flt_dstmac,		prs_flt_dstmac,		flt_dl_mac_dst, 6 ); \
	OPTLIST_SET_FLT(OPTL, 'l',	"limitnb",	OPT_FLT_LIMITNB,	PO_ARG, chk_flt_limitnb,	prs_flt_limitnb,	0, 0 ); \
	OPTLIST_SET_FLT(OPTL, 'g',	"globalport",	OPT_FLT_GLOBALPORT,	PO_ARG, chk_flt_globalport,	prs_flt_globalport,	flt_tl_port_global, 2 ); \
	OPTLIST_SET_FLT(OPTL, 'G',	"globalip",	OPT_FLT_GLOBALIP,	PO_ARG, chk_flt_globalip,	prs_flt_globalip,	flt_nl_ip_global, 4 ); \
	OPTLIST_SET_FLT(OPTL, 't',	"timelimit",	OPT_FLT_TIMELIMIT,	PO_ARG, chk_flt_timelimit,	prs_flt_timelimit,	0, 0 ); \
	OPTLIST_SET_DSP(OPTL, '\0',	"",		OPT_END,	PO_NOARG, 0,	0, 0 );

int lookup_options(int argc, char **argv, struct linked_list *optlist, struct linked_list_opt_value **elem)
{
	static int args = 1;
	char *ptr;
	int end,ptri;
	struct linked_list_value *ptrr;
	end = OPT_NULL;
	if (args <= argc)
	{
		ptr = argv[args];
		if (*ptr == '-')
		{
			if (*(ptr + 1) == '-')
			{
				ptri = LOOKUP_LONG(optlist,(ptr + 2),ptrr);
			}
			else if (*(ptr + 2))
			{
				value = argv[args];
				return OPT_ERROR;
			}
			else
			{
				ptri = LOOKUP_SHORT(optlist,*(ptr + 1),ptrr);
			}

			if (ptri != OPT_NULL)
			{
				if (ptrr->u.opt->flags == PO_ARG)
				{
					if (args < argc)
					{
						value = argv[++args];
					}
					else
					{
						/* >>> TODO: error "argument missing */
						value = argv[args];
						return OPT_ERROR;
					}
				}
				args++;
				end = OPT_OK;
				*elem = ptrr->u.opt;
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

#define OPT_PARSE_ERROR(L,V) \
	linked_list_free(L); \
	return V;

int parse_options(int argc, char **argv)
{
	int o;
	struct linked_list_opt_value *optl;
	struct linked_list *po_optlist = linked_list_init();
	OPTLIST_INIT(po_optlist);
	while ((o = lookup_options(argc,argv,po_optlist,&optl)) != OPT_END)
	{
		if (o != OPT_OK)
		{
			po_error = value;
			OPT_PARSE_ERROR(po_optlist,P_ERROR);
		}
		else 
		{
			if (optl->func_chk)
			{
				if (optl->func_chk(value) != OPT_OK)
				{
					/* >>> TODO : specific check error */
					po_error = value;
					OPT_PARSE_ERROR(po_optlist,P_ERROR);
				}
			}
			if (optl->func_prs)
			{
				if ((o = optl->func_prs(optl,value)) != OPT_OK)
				{
					OPT_PARSE_ERROR(po_optlist,o);
				}
			}
		}
	}
	linked_list_free(po_optlist); 
	return 0;
}

__inline__ int prs_dsp_help(struct linked_list_opt_value *optl, char *val)
{
	return P_ERROR;
}

__inline__ int prs_dsp_version(struct linked_list_opt_value *optl, char *val)
{
	po_misc = MISCNO_VERSION;
	return P_MISC;
}

__inline__ int prs_dsp_license(struct linked_list_opt_value *optl, char *val)
{
	po_misc = MISCNO_LICENSE;
	return P_MISC;
}

__inline__ int prs_dsp_simpledispla(struct linked_list_opt_value *optl, char *val)
{
	opt_simpledisplay = 1;
	return OPT_OK;
}

__inline__ int prs_dsp_displaydata(struct linked_list_opt_value *optl, char *val)
{
	opt_displaydata = 1;
	return OPT_OK;
}

__inline__ int prs_dsp_displayheader(struct linked_list_opt_value *optl, char *val)
{
	opt_displayheader = 1;
	return OPT_OK;
}

__inline__ int prs_dsp_displaypackets(struct linked_list_opt_value *optl, char *val)
{
	opt_displaypackets = 1;
	return OPT_OK;
}

__inline__ int prs_dsp_displayallpackets(struct linked_list_opt_value *optl, char *val)
{
	opt_displayallpackets = 1;
	return OPT_OK;
}

__inline__ int prs_dsp_displaydlproto(struct linked_list_opt_value *optl, char *val)
{
	opt_displaydlproto = 1;
	return OPT_OK;
}

__inline__ int prs_dsp_displaynlproto(struct linked_list_opt_value *optl, char *val)
{
	opt_displaynlproto = 1;
	return OPT_OK;
}

__inline__ int prs_dsp_displaytlproto(struct linked_list_opt_value *optl, char *val)
{
	opt_displaytlproto = 1;
	return OPT_OK;
}

__inline__ int prs_dsp_dontdisplayemptyslp(struct linked_list_opt_value *optl, char *val)
{
	opt_ndisplayemptyslp = 1;
	return OPT_OK;
}

__inline__ int prs_dsp_dontdisplaypackets(struct linked_list_opt_value *optl, char *val)
{
	opt_ndisplaypackets = 1;
	return OPT_OK;
}

__inline__ int prs_dsp_displayhexa(struct linked_list_opt_value *optl, char *val)
{
	opt_displayhexa = 1;
	return OPT_OK;
}

__inline__ int prs_dsp_simpledisplay(struct linked_list_opt_value *optl, char *val)
{
	opt_simpledisplay = 1;
	return OPT_OK;
}

__inline__ int prs_output(struct linked_list_opt_value *optl, char *val)
{
	opt_output = 1;
	output = val;
	return OPT_OK;
}

__inline__ int prs_outputdata(struct linked_list_opt_value *optl, char *val)
{
	opt_outputdata = 1;
	outputdata = val;
	return OPT_OK;
}

__inline__ int prs_inputfile(struct linked_list_opt_value *optl, char *val)
{
	inputfile = val;
	return OPT_OK;
}

__inline__ int prs_outputfile(struct linked_list_opt_value *optl, char *val)
{
	outputfile = val;
	return OPT_OK;
}

#define PRS_FLT_LKD_ADD(L,O,V) \
	linked_list_add(L, \
			linked_list_flt_value_init(O->u.flt.func_flt, \
						   (__u8 *)V, \
						   O->u.flt.flt_size));
	
#define PRS_FLT_PORT(L,O,V) \
	__u16 tmp = my_atoi(V,10); \
	PRS_FLT_LKD_ADD(L,O,&tmp);

__inline__ int prs_flt_srcport(struct linked_list_opt_value *optl, char *val)
{
	PRS_FLT_PORT(list_filter,optl,val);
	return OPT_OK;
}

__inline__ int prs_flt_dstport(struct linked_list_opt_value *optl, char *val)
{
	PRS_FLT_PORT(list_filter,optl,val);
	return OPT_OK;
}

__inline__ int prs_flt_globalport(struct linked_list_opt_value *optl, char *val)
{
	PRS_FLT_PORT(list_filter,optl,val);
	return OPT_OK;
}

#define PRS_FLT_IP(L,O,V) \
	__u32 tmp = ipv4_aton(V); \
	PRS_FLT_LKD_ADD(L,O,&tmp);

__inline__ int prs_flt_srcip(struct linked_list_opt_value *optl, char *val)
{
	PRS_FLT_IP(list_filter,optl,val);
	return OPT_OK;
}

__inline__ int prs_flt_dstip(struct linked_list_opt_value *optl, char *val)
{
	PRS_FLT_IP(list_filter,optl,val);
	return OPT_OK;
}

__inline__ int prs_flt_globalip(struct linked_list_opt_value *optl, char *val)
{
	PRS_FLT_IP(list_filter,optl,val);
	return OPT_OK;
}

#define PRS_FLT_MAC(L,O,V) \
	__u8 tmp[MAC_STR_SIZE]; \
	mac_aton(val,tmp); \
	PRS_FLT_LKD_ADD(L,O,&tmp);

__inline__ int prs_flt_srcmac(struct linked_list_opt_value *optl, char *val)
{
	PRS_FLT_MAC(list_filter,optl,val);
	return OPT_OK;
}

__inline__ int prs_flt_dstmac(struct linked_list_opt_value *optl, char *val)
{
	PRS_FLT_MAC(list_filter,optl,val);
	return OPT_OK;
}

__inline__ int prs_flt_limitnb(struct linked_list_opt_value *optl, char *val)
{
	flimitnb = val;
	return OPT_OK;
}

__inline__ int prs_flt_timelimit(struct linked_list_opt_value *optl, char *val)
{
	ftimelimit = val;
	return OPT_OK;
}

__inline__ int prs_flt_filterstr(struct linked_list_opt_value *optl, char *val)
{
	filterstr = val;
	return OPT_OK;
}

__inline__ int prs_flt_filterregex(struct linked_list_opt_value *optl, char *val)
{
	filterregexstr = val;
	return OPT_OK;
}

__inline__ int prs_flt_protocol(struct linked_list_opt_value *optl, char *val)
{
	int tmp = lookup_protoid(strupr(val));
	PRS_FLT_LKD_ADD(list_filter,optl,&tmp);
	return OPT_OK;
}

__inline__ int prs_flt_ethprotocol(struct linked_list_opt_value *optl, char *val)
{
	int tmp = lookup_ethid(strupr(val));
	PRS_FLT_LKD_ADD(list_filter,optl,&tmp);
	return OPT_OK;
}

__inline__ int prs_flt_ipprotocol(struct linked_list_opt_value *optl, char *val)
{
	int tmp = lookup_ipid(strupr(val));
	PRS_FLT_LKD_ADD(list_filter,optl,&tmp);
	return OPT_OK;
}
