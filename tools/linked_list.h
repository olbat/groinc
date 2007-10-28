/*
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

#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

#include <asm/types.h>

#include "../network/headers.h"

enum linked_list_type
{
	LKD_TYPE_FLT,
	LKD_TYPE_DSP_PKT,
	LKD_TYPE_DSP_RPT,
	LKD_TYPE_RPT,
	LKD_TYPE_OPT,
	LKD_TYPE_ERR
};

struct linked_list_value
{
	union linked_list_union
	{
		struct linked_list_flt_value *flt;
		struct linked_list_dsp_pkt_value *dsp_pkt;
		struct linked_list_dsp_rpt_value *dsp_rpt;
		struct linked_list_rpt_value *rpt;
		struct linked_list_opt_value *opt;
		struct linked_list_err_value *err;
	} u;
	enum linked_list_type type;
};

	
struct linked_list
{
	struct linked_list_value *value;
	struct linked_list *next;
};


struct linked_list *linked_list_init();
void linked_list_free(struct linked_list *);
struct linked_list_value *linked_list_add(struct linked_list *,struct linked_list_value *);
void linked_list_value_free(struct linked_list_value *);

#include "../filter.h"
struct linked_list_value *linked_list_flt_value_init(int (*func)(struct protocol_header *, struct protocol_header *, struct protocol_header *, struct data *, __u8 *), __u8 *, unsigned int);
void linked_list_flt_value_free(struct linked_list_value *);

#include "../display.h"
struct linked_list_value *linked_list_dsp_pkt_value_init(void (*func)(int, struct protocol_header *, struct protocol_header *, struct protocol_header *, struct data *));
void linked_list_dsp_pkt_value_free(struct linked_list_value *);
struct linked_list_value *linked_list_dsp_rpt_value_init(void (*func_dsp)(int, __u8 *), __u8 *val, unsigned int size);
void linked_list_dsp_rpt_value_free(struct linked_list_value *);

#include "../report.h"
struct linked_list_value *linked_list_rpt_value_init(void (*func)(__u8 *,long int,long int,long int), __u8 *val, unsigned int size);
void linked_list_rpt_value_free(struct linked_list_value *);

#include "../parse_options.h"
struct linked_list_value *linked_list_opt_value_init_flt(char ns, char *nl, enum optid id, int fl, int (*f_chk)(char *), int (*f_prs)(struct linked_list_opt_value *, char *), int (*f_flt)(struct protocol_header *, struct protocol_header *, struct protocol_header *, struct data *,__u8 *));
struct linked_list_value *linked_list_opt_value_init_dsp_pkt(char ns, char *nl, enum optid id, int fl, int (*f_chk)(char *), int (*f_prs)(struct linked_list_opt_value *, char *), void (*func)(int, struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, struct data *datagram));
struct linked_list_value *linked_list_opt_value_init_dsp_rpt(char ns, char *nl, enum optid id, int fl, int (*f_chk)(char *), int (*f_prs)(struct linked_list_opt_value *, char *), void (*func_dsp)(int, __u8 *), void (*func_rpt)(__u8 *,long int, long int, long int));
void linked_list_opt_value_free(struct linked_list_value *);

#include "../error.h"
struct linked_list_value *linked_list_err_value_init(enum err_id, char *);
void linked_list_err_value_free(struct linked_list_value *val);

#endif
