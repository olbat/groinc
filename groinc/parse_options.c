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

#include "parse_options.h"
#include "check_options.h"
#include "globals_args.h"
#include "globals_display.h"
#include "globals_filter.h"
#include "globals_report.h"
#include "globals_error.h"
#include "display.h"
#include "report.h"
#include "error.h"
#include "defaults.h"
#include "tools/memory_tools.h"
#include "tools/math_tools.h"
#include "tools/linked_list.h"
#include "network/protocols.h"
#include "misc.h"
#include "filter.h"

#include <regex.h>

static char *value;

#define LOOKUP_SHORT(N,C,P) \
__extension__ \
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
__extension__ \
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

#define OPTLIST_ADD_DSP_PKT(O,SH,LG,ID,FL,CK,PR,D) \
	linked_list_add(O,linked_list_opt_value_init_dsp_pkt(SH,LG,ID,FL,CK,PR,D));

#define OPTLIST_ADD_DSP_RPT(O,SH,LG,ID,FL,CK,PR,D,R) \
	linked_list_add(O,linked_list_opt_value_init_dsp_rpt(SH,LG,ID,FL,CK,PR,D,R));

#define OPTLIST_ADD_FLT(O,SH,LG,ID,FL,CK,PR,F) \
	linked_list_add(O,linked_list_opt_value_init_flt(SH,LG,ID,FL,CK,PR,F));

/* Do not forget to increment the OPTLIST_SIZE when add an option */
/* void options_optlist_init(struct linked_list_opt_value *optl) */
#define OPTLIST_INIT(OPTL) \
	OPTLIST_ADD_DSP_PKT(OPTL, 'o',	"outputdata",	OPT_OUTPUTDATA,	PO_ARG, chk_outputdata,	prs_outputdata,	0 ); \
	OPTLIST_ADD_DSP_PKT(OPTL, 'O',	"output",	OPT_OUTPUT,	PO_ARG, chk_output,	prs_output, 0 ); \
	OPTLIST_ADD_DSP_PKT(OPTL, 'r',	"read",		OPT_INPUTFILE,	PO_ARG, chk_inputfile,	prs_inputfile, 0 ); \
	OPTLIST_ADD_DSP_PKT(OPTL, 'w',	"write",	OPT_OUTPUTFILE,	PO_ARG, chk_outputfile,	prs_outputfile, 0 ); \
	OPTLIST_ADD_DSP_PKT(OPTL, 'a',	"displaydata",	OPT_DSP_PKT_DISPLAYDATA,	PO_NOARG, 0,	prs_dsp_pkt_displayopt, dsp_pkt_data); \
	OPTLIST_ADD_DSP_PKT(OPTL, 'A',	"simpledisplay",OPT_DSP_PKT_SIMPLEDISPLAY,	PO_NOARG, 0,	prs_dsp_pkt_displayopt, dsp_pkt_simple); \
	OPTLIST_ADD_DSP_PKT(OPTL, 'v',	"verbose",	OPT_DSP_PKT_DISPLAYHEADER,	PO_NOARG, 0,	prs_dsp_pkt_displayopt, dsp_pkt_header); \
	OPTLIST_ADD_DSP_PKT(OPTL, 'n',	"displaypackets",OPT_DSP_PKT_DISPLAYPACKETS,	PO_NOARG, 0,	prs_dsp_pkt_displayopt, dsp_pkt_packets); \
	OPTLIST_ADD_DSP_PKT(OPTL, 'H',	"hexa",		OPT_DSP_PKT_DISPLAYHEXA,	PO_NOARG, 0,	prs_dsp_pkt_displayopt, dsp_pkt_hexa); \
	OPTLIST_ADD_DSP_PKT(OPTL, 'h',	"help", 	OPT_DSP_PKT_HELP,		PO_NOARG, 0,	prs_dsp_pkt_help, 0 ); \
	OPTLIST_ADD_DSP_PKT(OPTL,  0,	"version",	OPT_DSP_PKT_VERSION,	PO_NOARG, 0,	prs_dsp_pkt_version, 0 ); \
	OPTLIST_ADD_DSP_PKT(OPTL,  0,	"license",	OPT_DSP_PKT_LICENSE,	PO_NOARG, 0,	prs_dsp_pkt_license, 0 ); \
	OPTLIST_ADD_DSP_PKT(OPTL, 'j',	"displayprotodatalink",	OPT_DSP_PKT_DISPLAYDLPROTO,	PO_NOARG, 0,	prs_dsp_pkt_displayopt, dsp_pkt_dlproto); \
	OPTLIST_ADD_DSP_PKT(OPTL, 'b',	"displayprototransport",OPT_DSP_PKT_DISPLAYTLPROTO,	PO_NOARG, 0,	prs_dsp_pkt_displayopt, dsp_pkt_tlproto); \
	OPTLIST_ADD_DSP_PKT(OPTL, 'B',	"displayprotonetwork",	OPT_DSP_PKT_DISPLAYNLPROTO,	PO_NOARG, 0,	prs_dsp_pkt_displayopt, dsp_pkt_nlproto); \
	OPTLIST_ADD_DSP_PKT(OPTL, 'N',	"displayallpackets",	OPT_DSP_PKT_DISPLAYALLPACKETS,	PO_NOARG, 0,	prs_dsp_pkt_displayopt, dsp_pkt_allpackets); \
	OPTLIST_ADD_DSP_RPT(OPTL, 'T',	"reportotaltime",	OPT_DSP_RPT_TIMETOT,		PO_NOARG, 0,	prs_dsp_rpt_timetot, dsp_rpt_timetot, 0); \
	OPTLIST_ADD_DSP_RPT(OPTL, 'c',	"countpacketstot",	OPT_DSP_RPT_COUNTPACKETSTOT,	PO_NOARG, 0,	prs_dsp_rpt_countpackets, dsp_rpt_countpacketstot, rpt_countpacketstot); \
	OPTLIST_ADD_DSP_RPT(OPTL, 'C',	"countpacketsfiltred",	OPT_DSP_RPT_COUNTPACKETSFILTRED,	PO_NOARG, 0,	prs_dsp_rpt_countpackets, dsp_rpt_countpacketsfiltred, rpt_countpacketsfiltred); \
	OPTLIST_ADD_FLT(OPTL, 'd',	"destport",	OPT_FLT_DSTPORT,	PO_ARG, chk_flt_dstport,	prs_flt_dstport,	flt_tl_port_dst); \
	OPTLIST_ADD_FLT(OPTL, 'D',	"destip",	OPT_FLT_DSTIP,		PO_ARG, chk_flt_dstip,		prs_flt_dstip,		flt_nl_ip_dst); \
	OPTLIST_ADD_FLT(OPTL, 's',	"sourceport",	OPT_FLT_SRCPORT,	PO_ARG, chk_flt_srcport,	prs_flt_srcport,	flt_tl_port_src); \
	OPTLIST_ADD_FLT(OPTL, 'S',	"sourceip",	OPT_FLT_SRCIP,		PO_ARG, chk_flt_srcip,		prs_flt_srcip,		flt_nl_ip_src); \
	OPTLIST_ADD_FLT(OPTL, 'f',	"filter",	OPT_FLT_FILTERSTR,	PO_ARG, chk_flt_filterstr,	prs_flt_filterstr,	flt_string); \
	OPTLIST_ADD_FLT(OPTL, 'Q',	"protocol",	OPT_FLT_PROTOCOL,	PO_ARG, chk_flt_protocol,	prs_flt_protocol,	flt_dl_protocol); \
	OPTLIST_ADD_FLT(OPTL, 'p',	"ipprotocol", 	OPT_FLT_IPPROTOCOL,	PO_ARG, chk_flt_ipprotocol,	prs_flt_ipprotocol,	flt_tl_protocol); \
	OPTLIST_ADD_FLT(OPTL, 'P',	"ethprotocol",	OPT_FLT_ETHPROTOCOL,	PO_ARG, chk_flt_ethprotocol,	prs_flt_ethprotocol,	flt_nl_protocol); \
	OPTLIST_ADD_FLT(OPTL, 'F',	"filter-regex",	OPT_FLT_FILTERREGEX,	PO_ARG, chk_flt_filterregex,	prs_flt_filterregex,	flt_regex); \
	OPTLIST_ADD_FLT(OPTL, 'm',	"sourcemac",	OPT_FLT_SRCMAC,		PO_ARG, chk_flt_srcmac,		prs_flt_srcmac,		flt_dl_mac_src); \
	OPTLIST_ADD_FLT(OPTL, 'M',	"destmac",	OPT_FLT_DSTMAC,		PO_ARG, chk_flt_dstmac,		prs_flt_dstmac,		flt_dl_mac_dst); \
	OPTLIST_ADD_FLT(OPTL, 'l',	"limitnb",	OPT_FLT_LIMITNB,	PO_ARG, chk_flt_limitnb,	prs_flt_limitnb,	0); \
	OPTLIST_ADD_FLT(OPTL, 'g',	"globalport",	OPT_FLT_GLOBALPORT,	PO_ARG, chk_flt_globalport,	prs_flt_globalport,	flt_tl_port_global); \
	OPTLIST_ADD_FLT(OPTL, 'G',	"globalip",	OPT_FLT_GLOBALIP,	PO_ARG, chk_flt_globalip,	prs_flt_globalip,	flt_nl_ip_global); \
	OPTLIST_ADD_FLT(OPTL, 't',	"timelimit",	OPT_FLT_TIMELIMIT,	PO_ARG, chk_flt_timelimit,	prs_flt_timelimit,	0); \
	OPTLIST_ADD_FLT(OPTL, 'z',	"dontdisplayemptysl",	OPT_DSP_PKT_DONTDISPLAYEMPTYSL,	PO_NOARG, 0,	prs_flt_dontdisplayemptysl, flt_sl_nempty); \
	OPTLIST_ADD_FLT(OPTL, 'q',	"quiet",	OPT_DSP_PKT_DONTDISPLAYPACKETS,	PO_NOARG, 0,	prs_flt_dontdisplaypackets, flt_dontdisplaypackets); \
	OPTLIST_ADD_DSP_PKT(OPTL, '\0',	"",		OPT_END,	PO_NOARG, 0,	0, 0 );

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
				ERR_SET(list_error,EOPT_INVAL,argv[args]);
				args++;
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
						ERR_SET(list_error,EARG_MISSING,argv[args]);
						args++;
						return OPT_ERROR;
					}
				}
				args++;
				end = ptrr->u.opt->id;
				*elem = ptrr->u.opt;
			}
			else
			{
				ERR_SET(list_error,EOPT_UNKNOWN,argv[args]);
				args++;
				end = OPT_ERROR;
			}
		}
		else
		{
			ERR_SET(list_error,EOPT_INVAL,argv[args]);
			args++;
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
__extension__ \
({ \
	linked_list_free(L); \
	return V; \
})

int parse_options(int argc, char **argv)
{
	int o,defaults;
	struct linked_list_opt_value *optval;
	struct linked_list *optlist = linked_list_init();
	OPTLIST_INIT(optlist);
	defaults = 0;

	while ((o = lookup_options(argc,argv,optlist,&optval)) != OPT_END)
	{
		if (o >= OPT_OK)
		{
			if ((o > OPT_DSP_RPT_START) && (o < OPT_DSP_RPT_END))
				defaults = defaults | OPT_TYPE_DSP_RPT;
			if (optval->func_chk)
			{
				if (optval->func_chk(value) != OPT_OK)
				{
				}
			}
			if (optval->func_prs)
			{
				if ((o = optval->func_prs(optval,value)) != OPT_OK)
					OPT_PARSE_ERROR(optlist,o);
			}
		}
	}

	linked_list_free(optlist);

	if (!(defaults & OPT_TYPE_DSP_RPT))
		default_rpt_init();

	return 0;
}

#define PRS_DSP_PKT_LKD_ADD(L,O) \
	linked_list_add(L, linked_list_dsp_pkt_value_init(O->u.dsp_pkt.func_dsp_pkt));

__inline__ int prs_dsp_pkt_help(struct linked_list_opt_value *optl, char *val)
{
	po_misc = MISCNO_HELP;
	return P_MISC;
}

__inline__ int prs_dsp_pkt_version(struct linked_list_opt_value *optl, char *val)
{
	po_misc = MISCNO_VERSION;
	return P_MISC;
}

__inline__ int prs_dsp_pkt_license(struct linked_list_opt_value *optl, char *val)
{
	po_misc = MISCNO_LICENSE;
	return P_MISC;
}

__inline__ int prs_dsp_pkt_displayopt(struct linked_list_opt_value *optl, char *val)
{
	PRS_DSP_PKT_LKD_ADD(list_display_packet,optl);
	return OPT_OK;
}

#define PRS_DSP_RPT_LKD_ADD(L,O,V,S) \
__extension__ \
({ \
	linked_list_add(L, linked_list_dsp_rpt_value_init(O->u.dsp_rpt.func_dsp_rpt,V,S)); \
})

#define PRS_RPT_LKD_ADD(L,O,V,S) \
__extension__ \
({ \
	linked_list_add(L, linked_list_rpt_value_init(O->u.dsp_rpt.func_rpt,V,S)); \
})

__inline__ int prs_dsp_rpt_timetot(struct linked_list_opt_value *optl, char *val)
{
	struct timeval tmp;

	gettimeofday(&tmp,0);

	PRS_DSP_RPT_LKD_ADD(list_display_report,optl,(__u8 *)&tmp,sizeof(struct timeval));

	return OPT_OK;
}

__inline__ int prs_dsp_rpt_countpackets(struct linked_list_opt_value *optl, char *val)
{
	unsigned int tmp = 0;
	struct linked_list_value *ptr,*p;

	ptr = PRS_RPT_LKD_ADD(list_report,optl,(__u8 *)&tmp,sizeof(unsigned int));
	p = PRS_DSP_RPT_LKD_ADD(list_display_report,optl,(__u8 *)&(ptr->u.rpt->val),sizeof(ptr->u.rpt->val));
	/* printf("L %hu\n",*((unsigned int *) *((unsigned int *) p->u.dsp_rpt->val))); */

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

#define PRS_FLT_LKD_ADD(L,O,V,S) \
	linked_list_add(L, \
			linked_list_flt_value_init(O->u.flt.func_flt, \
						   (__u8 *)V, \
						   S));
	
#define PRS_FLT_PORT(L,O,V) \
	__u16 tmp = my_atoi(V,10); \
	PRS_FLT_LKD_ADD(L,O,&tmp,sizeof(__u16));

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
	PRS_FLT_LKD_ADD(L,O,&tmp,sizeof(__u32));

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
	PRS_FLT_LKD_ADD(L,O,&tmp,(MAC_STR_SIZE * sizeof(__u8)));

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
	llimitnb = (long int) my_atoi(val,10);
	return OPT_OK;
}

__inline__ int prs_flt_timelimit(struct linked_list_opt_value *optl, char *val)
{
	timelimit.tv_sec = my_atoi(val,10);
	timelimit.tv_usec = 500;
	return OPT_OK;
}

__inline__ int prs_flt_filterstr(struct linked_list_opt_value *optl, char *val)
{
	PRS_FLT_LKD_ADD(list_filter,optl,val,sizeof(char *));
	return OPT_OK;
}

__inline__ int prs_flt_filterregex(struct linked_list_opt_value *optl, char *val)
{
	regex_t tmp; 
	regcomp(&tmp,val,REG_NOSUB);
	PRS_FLT_LKD_ADD(list_filter,optl,&tmp,sizeof(regex_t));
	regfree(&tmp);
	return OPT_OK;
}

__inline__ int prs_flt_protocol(struct linked_list_opt_value *optl, char *val)
{
	int tmp = lookup_protoid(strupr(val));
	PRS_FLT_LKD_ADD(list_filter,optl,&tmp,sizeof(unsigned int));
	return OPT_OK;
}

__inline__ int prs_flt_ethprotocol(struct linked_list_opt_value *optl, char *val)
{
	int tmp = lookup_ethid(strupr(val));
	PRS_FLT_LKD_ADD(list_filter,optl,&tmp,sizeof(unsigned int));
	return OPT_OK;
}

__inline__ int prs_flt_ipprotocol(struct linked_list_opt_value *optl, char *val)
{
	int tmp = lookup_ipid(strupr(val));
	PRS_FLT_LKD_ADD(list_filter,optl,&tmp,sizeof(unsigned int));
	return OPT_OK;
}

__inline__ int prs_flt_dontdisplayemptysl(struct linked_list_opt_value *optl, char *val)
{
	PRS_FLT_LKD_ADD(list_filter,optl,0,0);
	return OPT_OK;
}

__inline__ int prs_flt_dontdisplaypackets(struct linked_list_opt_value *optl, char *val)
{
	PRS_FLT_LKD_ADD(list_filter,optl,0,0);
	return OPT_OK;
}

