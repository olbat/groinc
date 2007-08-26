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

#include "error.h"
#include "tools/hashtable.h"
#include "globals_error.h"
#include "globals_display.h"
#include "prints.h"
#include "tools/compiler.h"

#define ERRLIST_ADD(T,ID,ST,M) \
({ \
	char key[my_itoa_buffer_size(ID,10)]; \
	my_itoa(ID,10,key); \
	hashtable_add(T,key,hashtable_err_value_init(ID,ST,M)); \
})

#define ERRLIST_INIT(ERRL) \
({ \
	ERRLIST_ADD(ERRL,EARG_MISSING,	ERR_ST_ERROR,	"Argument missing"); \
	ERRLIST_ADD(ERRL,EARG_INVAL,	ERR_ST_ERROR,	"Invalid argument"); \
	ERRLIST_ADD(ERRL,EREGEX_INVAL,	ERR_ST_ERROR,	"Invalid regular expression"); \
	ERRLIST_ADD(ERRL,EOPT_INVAL,	ERR_ST_ERROR,	"Invalid option"); \
	ERRLIST_ADD(ERRL,EOPT_UNKNOWN,	ERR_ST_ERROR,	"Unknown option"); \
})

int error_display()
{
	int end;

	struct hashtable *errlist = hashtable_init(4);
	ERRLIST_INIT(errlist);

	end = 0;
	
	struct linked_list *ptr = list_error;
	struct hashtable_err_value *tptr;

	while (ptr)
	{
		if (unlikely(ptr->value))
		{
			tptr = hashtable_err_lookup(errlist,ptr->value->u.err->id);
			print_error(headerfd,2,"error: ",tptr->msg);
			if (likely(ptr->value->u.err->arg))
				print_error(headerfd,3," \"",ptr->value->u.err->arg,"\"");
			print_newline(headerfd);
			end = end | tptr->state; 
		}
		ptr = ptr->next;
	}
	
	hashtable_delete(errlist);

	return (end & ERR_ST_ERROR);
}

