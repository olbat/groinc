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

#include "report.h"
#include "globals_report.h"
#include "tools/compiler.h"

void report(int id_dl, int id_nl, int id_tl)
{
	struct linked_list *ptr = list_report;
	if (likely(ptr->value))
	{
		while (ptr)
		{
			(ptr->value->u.rpt->func_rpt)(ptr->value->u.rpt->val,id_dl,id_nl,id_tl);
			ptr = ptr->next;
		}
	}
	else
	{
		default_report.countpacketstot++;
		if ((id_dl != -1) || (id_nl != -1) || (id_tl != -1))
			default_report.countpacketsfiltred++;
		/*
		rpt_countpacketstot(default_report,id_dl,id_nl,id_tl);
		rpt_countpacketsfiltred((default_report + sizeof(unsigned int)),id_dl,id_nl,id_tl);
		*/
	}
}

#define RPT_COUNT_PACKETS_INC(V) \
__extension__ \
({ \
	*V = (*((unsigned int *) V) + 1); \
})

void rpt_countpacketstot(__u8 *val, int id_dl, int id_nl, int id_tl)
{
	RPT_COUNT_PACKETS_INC(val);
	
}

void rpt_countpacketsfiltred(__u8 *val, int id_dl, int id_nl, int id_tl)
{
	if ((id_dl != -1) || (id_nl != -1) || (id_tl != -1))
		RPT_COUNT_PACKETS_INC(val);
}
