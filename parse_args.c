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

#include <netdb.h>

#include "parse_args.h"
#include "globals_args.h"
#include "globals_display.h"
#include "globals_filter.h"
#include "globals_report.h"
#include "tools/memory_tools.h"
#include "tools/linked_list.h"
#include "network/protocols.h"
#include "misc.h"

#include <regex.h>
#include <sys/socket.h>

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
	struct hostent *tmp; \
	tmp = gethostbyname(val); \
	PRS_FLT_LKD_ADD(L,O,tmp->h_addr,sizeof(__u32));

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

