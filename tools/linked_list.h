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

#include "../network/headers.h"
#include "../network/my_types.h"

enum linked_list_type
{
	LKD_TYPE_FLT,
	LKD_TYPE_OPT
};

struct linked_list_value
{
	union linked_list_union
	{
		struct linked_list_flt_value *flt;
		struct linked_list_opt_value *opt;
	} u;
	enum linked_list_type type;
};

	
struct linked_list
{
	struct linked_list_value *value;
	struct linked_list *next;
};

#include "../parse_options.h"
#include "../filter.h"

struct linked_list *linked_list_init();
void linked_list_free(struct linked_list *l);
struct linked_list_value *linked_list_add(struct linked_list *l,struct linked_list_value *val);
void linked_list_value_free(struct linked_list_value *val);

struct linked_list_value *linked_list_flt_value_init(int (*func)(struct protocol_header *, struct protocol_header *, struct protocol_header *, __u8 *), __u8 *, unsigned int);
void linked_list_flt_value_free(struct linked_list_value *val);
struct linked_list_value *linked_list_opt_value_init_flt(char ns, char *nl, enum optid id, int fl, int (*f_chk)(char *), int (*f_prs)(struct linked_list_opt_value *, char *), int (*f_flt)(struct protocol_header *, struct protocol_header *, struct protocol_header *, __u8 *), unsigned int flt_size);
struct linked_list_value *linked_list_opt_value_init_dsp(char ns, char *nl, enum optid id, int fl, int (*f_chk)(char *), int (*f_prs)(struct linked_list_opt_value *, char *), __u32 optid);
void linked_list_opt_value_free(struct linked_list_value *val);

#endif
