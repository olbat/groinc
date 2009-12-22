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

#include "linked_list.h"
#include <malloc.h>
#include <string.h>

struct linked_list *linked_list_init()
{
	struct linked_list *end;
	end = (struct linked_list *) malloc(sizeof(struct linked_list));
	end->next = 0;
	end->value = 0;
	return end;
}

void linked_list_free(struct linked_list *l)
{
	struct linked_list *ptr;
	ptr = l;
	while (l)
	{
		ptr = l;
		l = l->next;
		linked_list_value_free(ptr->value);
		free(ptr);
	}
}

struct linked_list_value *linked_list_add(struct linked_list *l,struct linked_list_value *val)
{
	if (l->value)
	{
		struct linked_list *ptr;
		ptr = l;
		while (l->next)
			l = l->next;
		l->next = linked_list_init(); 
		l = l->next;
	}
	l->value = val;
	return val;
}

struct linked_list_value *linked_list_flt_value_init(int (*func)(struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram, __u8 *v), void (*func_free)(__u8 *), __u8 *val, unsigned int size)
{
	struct linked_list_value *end;

	end = (struct linked_list_value *) malloc(sizeof(struct linked_list_value));
	end->type = LKD_TYPE_FLT;
	end->u.flt = (struct linked_list_flt_value *) malloc(sizeof(struct linked_list_flt_value));
	end->u.flt->func_flt = func;
	end->u.flt->func_free = func_free;
	end->u.flt->val = (__u8 *) malloc(size * sizeof(__u8));
	memcpy((char *)end->u.flt->val,(char *)val,size);

	return end;
}

struct linked_list_value *linked_list_dsp_pkt_value_init(void (*func)(int, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram))
{
	struct linked_list_value *end;

	end = (struct linked_list_value *) malloc(sizeof(struct linked_list_value));
	end->type = LKD_TYPE_DSP_PKT;
	end->u.dsp_pkt = (struct linked_list_dsp_pkt_value *) malloc(sizeof(struct linked_list_dsp_pkt_value));
	end->u.dsp_pkt->func_dsp_pkt = func;

	return end;
}

struct linked_list_value *linked_list_dsp_rpt_value_init(void (*func)(int, __u8 *), __u8 *val, unsigned int size)
{
	struct linked_list_value *end;

	end = (struct linked_list_value *) malloc(sizeof(struct linked_list_value *));
	end->type = LKD_TYPE_DSP_RPT;
	end->u.dsp_rpt = (struct linked_list_dsp_rpt_value *) malloc(sizeof(struct linked_list_dsp_rpt_value));
	end->u.dsp_rpt->func_dsp_rpt = func;
	end->u.dsp_rpt->val = (__u8 *) malloc(size * sizeof(__u8));
	memcpy((char *)end->u.dsp_rpt->val,(char *)val,size);

	return end;
}

struct linked_list_value *linked_list_rpt_value_init(void (*func)(__u8 *,int,int,int), __u8 *val, unsigned int size)
{
	struct linked_list_value *end;

	end = (struct linked_list_value *) malloc(sizeof(struct linked_list_value *));
	end->type = LKD_TYPE_RPT;
	end->u.rpt = (struct linked_list_rpt_value *) malloc(sizeof(struct linked_list_rpt_value));
	end->u.rpt->func_rpt = func;
	end->u.rpt->val = (__u8 *) malloc(size * sizeof(__u8));
	memcpy((char *)end->u.rpt->val,(char *)val,size);

	return end;
}

#define LKD_OPT_VALUE_INIT(V,NS,NL,ID,FL,CHK,PRS,TY) \
	V->name_short = NS; \
	V->name_long = NL; \
	V->id = ID; \
	V->flags = FL; \
	V->func_chk = CHK; \
	V->func_prs = PRS; \
	V->type = TY;

struct linked_list_value *linked_list_opt_value_init_flt(char ns, char *nl, enum optid id, int fl, int (*f_chk)(char *), int (*f_prs)(struct linked_list_opt_value *, char *), int (*f_flt)(struct protocol_header *, struct protocol_header *, struct protocol_header *, struct data *datagram, __u8 *), void (*f_free)(__u8 *))
{
	struct linked_list_value *end;
	
	end = (struct linked_list_value *) malloc(sizeof(struct linked_list_value));
	end->type = LKD_TYPE_OPT;
	
	end->u.opt = (struct linked_list_opt_value *) malloc(sizeof(struct linked_list_opt_value));
	LKD_OPT_VALUE_INIT(end->u.opt,ns,nl,id,fl,f_chk,f_prs,OPT_TYPE_FLT);
	end->u.opt->u.flt.func_flt = f_flt;
	end->u.opt->u.flt.func_free = f_free;
	
	return end;
}

struct linked_list_value *linked_list_opt_value_init_dsp_pkt(char ns, char *nl, enum optid id, int fl, int (*f_chk)(char *), int (*f_prs)(struct linked_list_opt_value *, char *), void (*func)(int, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram))
{
	struct linked_list_value *end;
	
	end = (struct linked_list_value *) malloc(sizeof(struct linked_list_value));
	end->type = LKD_TYPE_OPT;

	end->u.opt = (struct linked_list_opt_value *) malloc(sizeof(struct linked_list_opt_value));
	LKD_OPT_VALUE_INIT(end->u.opt,ns,nl,id,fl,f_chk,f_prs,OPT_TYPE_DSP_PKT);
	end->u.opt->u.dsp_pkt.func_dsp_pkt = func;
	
	return end;
}

struct linked_list_value *linked_list_opt_value_init_dsp_rpt(char ns, char *nl, enum optid id, int fl, int (*f_chk)(char *), int (*f_prs)(struct linked_list_opt_value *, char *), void (*func_dsp)(int, __u8 *), void (*func_rpt)(__u8 *,int, int, int))
{
	struct linked_list_value *end;
	
	end = (struct linked_list_value *) malloc(sizeof(struct linked_list_value));
	end->type = LKD_TYPE_OPT;

	end->u.opt = (struct linked_list_opt_value *) malloc(sizeof(struct linked_list_opt_value));
	LKD_OPT_VALUE_INIT(end->u.opt,ns,nl,id,fl,f_chk,f_prs,OPT_TYPE_DSP_RPT);
	end->u.opt->u.dsp_rpt.func_dsp_rpt = func_dsp;
	end->u.opt->u.dsp_rpt.func_rpt = func_rpt;
	
	return end;
}

struct linked_list_value *linked_list_err_value_init(enum err_id id, char *arg)
{
	struct linked_list_value *end;
	
	end = (struct linked_list_value *) malloc(sizeof(struct linked_list_value));
	end->type = LKD_TYPE_ERR;

	end->u.err = (struct linked_list_err_value *) malloc(sizeof(struct linked_list_err_value));
	end->u.err->id = id;
	end->u.err->arg = arg;

	return end;
}

void linked_list_opt_value_free(struct linked_list_value *val)
{
	free(val->u.opt);
	free(val);
}

void linked_list_flt_value_free(struct linked_list_value *val)
{
	if (val->u.flt->func_free)
		(val->u.flt->func_free)(val->u.flt->val);
	free(val->u.flt->val);
	free(val->u.flt);
	free(val);
}

void linked_list_dsp_pkt_value_free(struct linked_list_value *val)
{
	free(val->u.dsp_pkt);
	free(val);
}

void linked_list_dsp_rpt_value_free(struct linked_list_value *val)
{
	free(val->u.dsp_rpt->val);
	free(val->u.dsp_rpt);
	free(val);
}

void linked_list_rpt_value_free(struct linked_list_value *val)
{
	free(val->u.rpt->val);
	free(val->u.rpt);
	free(val);
}

void linked_list_err_value_free(struct linked_list_value *val)
{
	free(val->u.err);
	free(val);
}

void linked_list_value_free(struct linked_list_value *val)
{
	if (val)
	{
		switch (val->type)
		{
			case LKD_TYPE_FLT : 
				linked_list_flt_value_free(val);
				break;
			case LKD_TYPE_DSP_PKT : 
				linked_list_dsp_pkt_value_free(val);
				break;
			case LKD_TYPE_DSP_RPT : 
				linked_list_dsp_rpt_value_free(val);
				break;
			case LKD_TYPE_RPT : 
				linked_list_rpt_value_free(val);
				break;
			case LKD_TYPE_OPT : 
				linked_list_opt_value_free(val);
				break;
			case LKD_TYPE_ERR : 
				linked_list_err_value_free(val);
				break;
		}
	}
}
