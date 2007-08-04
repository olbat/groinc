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
#include "memory_tools.h"

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

struct linked_list_value *linked_list_flt_value_init(int (*func)(struct protocol_header *datalink_layerph, struct protocol_header *network_layerph, struct protocol_header *transport_layerph, __u8 *v), __u8 *val, unsigned int size)
{
	struct flt_value *flt;
	struct linked_list_value *end;
	flt = (struct flt_value *) malloc(sizeof(struct flt_value));
	flt->func_flt = func;
	flt->val = (__u8 *) malloc(size * sizeof(__u8));
	my_memcpy((char *)flt->val,(char *)val,size);
	end = (struct linked_list_value *) malloc(sizeof(struct linked_list_value *));
	end->u.flt = flt;
	end->type = LKD_FLT_TYPE;

	return end;
}

void linked_list_flt_value_free(struct linked_list_value *val)
{
	free(val->u.flt->val);
	free(val->u.flt);
	free(val);
}

void linked_list_value_free(struct linked_list_value *val)
{
	if (val)
	{
		switch (val->type)
		{
			case LKD_FLT_TYPE : 
				linked_list_flt_value_free(val);
				break;
		}
	}
}
