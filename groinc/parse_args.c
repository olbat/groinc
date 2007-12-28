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
#include "protocols/namescache.h"
#include "check_options.h"

#include <regex.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>

int prs_dsp_pkt_help(struct linked_list_opt_value *optl, char *val)
{
	po_misc = MISCNO_HELP;
	return P_MISC;
}

int prs_dsp_pkt_version(struct linked_list_opt_value *optl, char *val)
{
	po_misc = MISCNO_VERSION;
	return P_MISC;
}

int prs_dsp_pkt_license(struct linked_list_opt_value *optl, char *val)
{
	po_misc = MISCNO_LICENSE;
	return P_MISC;
}

#define PRS_DSP_PKT_LKD_ADD(L,O) \
	linked_list_add(L, linked_list_dsp_pkt_value_init(O->u.dsp_pkt.func_dsp_pkt));

int prs_dsp_pkt_displayopt(struct linked_list_opt_value *optl, char *val)
{
	PRS_DSP_PKT_LKD_ADD(list_display_packet,optl);
	return OPT_OK;
}

#define PRS_DSP_RPT_LKD_ADD(L,O,V,S) \
__extension__ \
({ \
	linked_list_add(L, linked_list_dsp_rpt_value_init((O)->u.dsp_rpt.func_dsp_rpt,V,S)); \
})

#define PRS_RPT_LKD_ADD(L,O,V,S) \
__extension__ \
({ \
	linked_list_add(L, linked_list_rpt_value_init((O)->u.dsp_rpt.func_rpt,V,S)); \
})

int prs_dsp_rpt_timetot(struct linked_list_opt_value *optl, char *val)
{
	struct timeval tmp;

	gettimeofday(&tmp,0);

	PRS_DSP_RPT_LKD_ADD(list_display_report,optl,(__u8 *)&tmp,sizeof(struct timeval));

	return OPT_OK;
}

int prs_dsp_rpt_countpackets(struct linked_list_opt_value *optl, char *val)
{
	unsigned int tmp = 0;
	struct linked_list_value *ptr,*p;

	ptr = PRS_RPT_LKD_ADD(list_report,optl,(__u8 *)&tmp,sizeof(unsigned int));
	p = PRS_DSP_RPT_LKD_ADD(list_display_report,optl,(__u8 *)&(ptr->u.rpt->val),sizeof(ptr->u.rpt->val));
	/* printf("L %hu\n",*((unsigned int *) *((unsigned int *) p->u.dsp_rpt->val))); */

	return OPT_OK;
}

int prs_output(struct linked_list_opt_value *optl, char *val)
{
	opt_output = 1;
	output = val;
	return OPT_OK;
}

int prs_outputdata(struct linked_list_opt_value *optl, char *val)
{
	opt_outputdata = 1;
	outputdata = val;
	return OPT_OK;
}

int prs_inputfile(struct linked_list_opt_value *optl, char *val)
{
	inputfile = val;
	return OPT_OK;
}

int prs_outputfile(struct linked_list_opt_value *optl, char *val)
{
	outputfile = val;
	return OPT_OK;
}

int prs_noresolv(struct linked_list_opt_value *optl, char *val)
{
	ncacheopt = ncacheopt | NCHE_OPT_NORESOLV;
	return OPT_OK;
}

#define PRS_FLT_LKD_ADD(L,O,V,S) \
	linked_list_add(L, \
			linked_list_flt_value_init(O->u.flt.func_flt, \
						   O->u.flt.func_free, \
						   (__u8 *)V, \
						   S));
	
#define PRS_FLT_PORT(L,O,V) \
	__u16 tmp = my_atoi(V,10); \
	PRS_FLT_LKD_ADD(L,O,&tmp,sizeof(__u16));

int prs_flt_srcport(struct linked_list_opt_value *optl, char *val)
{
	PRS_FLT_PORT(list_filter,optl,val);
	return OPT_OK;
}

int prs_flt_dstport(struct linked_list_opt_value *optl, char *val)
{
	PRS_FLT_PORT(list_filter,optl,val);
	return OPT_OK;
}

int prs_flt_globalport(struct linked_list_opt_value *optl, char *val)
{
	PRS_FLT_PORT(list_filter,optl,val);
	return OPT_OK;
}

#define PRS_FLT_IP(L,O,V) \
	__u32 addrs[2]; \
	__u32 addr,mask; \
	regex_t reg; \
	char *str; \
	char buff[IPV4_STR_MAXSIZE]; \
	struct hostent *hostt; \
\
	str = buff; \
	strcpy(str,V); \
\
	regcomp(&reg,CHK_FLT_HOST_REGEX_HOSTNAME,REG_NOSUB); \
	if (regexec(&reg,V,0,0,0)) \
	{ \
		regfree(&reg); \
		regcomp(&reg,CHK_FLT_HOST_REGEX_CIDR,REG_NOSUB); \
		if (!regexec(&reg,V,0,0,0)) \
		{ \
			str = index(buff,'/'); \
			addrs[0] = my_atoi(str + 1,10); \
			mask = 0; \
			for (addr = 0;addr < sizeof(__u32)*8; addr++) \
			{ \
				if (addr <= addrs[0]) \
					mask |= 1; \
				mask <<= 1; \
			} \
			if (addr == addrs[0]) \
				mask |= 1; \
			mask = htonl(mask); \
			if (*(str - 1) == '.') \
				*(str - 1) = 0; \
			else \
				*str = 0; \
			while (strccount(buff,'.') < 3) \
				strcat(buff,".0"); \
		} \
		else \
		{ \
			regfree(&reg); \
			regcomp(&reg,CHK_FLT_HOST_REGEX_IPNETMASK,REG_NOSUB); \
			if (regexec(&reg,V,0,0,0)) \
				mask = ~0; \
			else \
			{ \
				str = index(buff,':'); \
				*str = 0; \
				hostt = gethostbyname(str + 1); \
				mask = *((__u32 *) hostt->h_addr); \
			} \
		} \
		regfree(&reg); \
	} \
	else \
		mask = ~0; \
	hostt = gethostbyname(buff); \
	addr = *((__u32 *) hostt->h_addr); \
	addr &= mask; \
	addrs[0] = addr; \
	addrs[1] = mask; \
	PRS_FLT_LKD_ADD(L,O,(__u8 *)addrs,sizeof(__u32) * sizeof(addrs));

int prs_flt_srcip(struct linked_list_opt_value *optl, char *val)
{
	PRS_FLT_IP(list_filter,optl,val);
	return OPT_OK;
}

int prs_flt_dstip(struct linked_list_opt_value *optl, char *val)
{
	PRS_FLT_IP(list_filter,optl,val);
	return OPT_OK;
}

int prs_flt_globalip(struct linked_list_opt_value *optl, char *val)
{
	PRS_FLT_IP(list_filter,optl,val);
	return OPT_OK;
}

#define PRS_FLT_MAC(L,O,V) \
	__u8 tmp[MAC_STR_SIZE]; \
	mac_aton(val,tmp); \
	PRS_FLT_LKD_ADD(L,O,&tmp,(MAC_STR_SIZE * sizeof(__u8)));

int prs_flt_srcmac(struct linked_list_opt_value *optl, char *val)
{
	PRS_FLT_MAC(list_filter,optl,val);
	return OPT_OK;
}

int prs_flt_dstmac(struct linked_list_opt_value *optl, char *val)
{
	PRS_FLT_MAC(list_filter,optl,val);
	return OPT_OK;
}

int prs_flt_limitnb(struct linked_list_opt_value *optl, char *val)
{
	llimitnb = (int) my_atoi(val,10);
	return OPT_OK;
}

int prs_flt_timelimit(struct linked_list_opt_value *optl, char *val)
{
	timelimit.tv_sec = my_atoi(val,10);
	timelimit.tv_usec = 500;
	return OPT_OK;
}

int prs_flt_filterstr(struct linked_list_opt_value *optl, char *val)
{
	PRS_FLT_LKD_ADD(list_filter,optl,val,sizeof(char *));
	return OPT_OK;
}

int prs_flt_filterregex(struct linked_list_opt_value *optl, char *val)
{
	regex_t tmp; 
	regcomp(&tmp,val,REG_NOSUB);
	PRS_FLT_LKD_ADD(list_filter,optl,&tmp,sizeof(regex_t));
	/* freed at the end */
	/* regfree(&tmp); */
	return OPT_OK;
}

#define PRS_FLT_PROTO(L,F,FR,V,S) \
	linked_list_add(L, \
			linked_list_flt_value_init(F,FR,(__u8 *)V,S));

int prs_flt_protocol(struct linked_list_opt_value *optl, char *val)
{
	int tmp;
	if ((tmp = lookup_ipid(strupr(val))) < 0)
	{
		if ((tmp = lookup_ethid(strupr(val))) < 0)
		{
			tmp = lookup_protoid(strupr(val));
			PRS_FLT_PROTO(list_filter,flt_dl_protocol,0,
				&tmp,sizeof(unsigned int));
		}
		else
		{
			PRS_FLT_PROTO(list_filter,flt_nl_protocol,0,
				&tmp,sizeof(unsigned int));
		}
	}
	else
	{
		PRS_FLT_PROTO(list_filter,flt_tl_protocol,0,
			&tmp,sizeof(unsigned int));
	}

	return OPT_OK;
}

int prs_flt_dontdisplayemptysl(struct linked_list_opt_value *optl, char *val)
{
	PRS_FLT_LKD_ADD(list_filter,optl,0,0);
	return OPT_OK;
}

int prs_flt_dontdisplaypackets(struct linked_list_opt_value *optl, char *val)
{
	PRS_FLT_LKD_ADD(list_filter,optl,0,0);
	return OPT_OK;
}

