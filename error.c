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
#include "network/protocols.h"

#define ERRLIST_ADD(T,ID,ST,M,F) __extension__ \
({ \
	char key[my_itoa_buffer_size(ID,10)]; \
	my_itoa(ID,10,key); \
	hashtable_add(T,key,hashtable_err_value_init(ID,ST,M,F)); \
})

#define ERRLIST_INIT(ERRL) __extension__ \
({ \
	ERRLIST_ADD(ERRL,EARG_MISSING,	ERR_ST_ERROR,	"Argument missing",0); \
	ERRLIST_ADD(ERRL,EARG_INVAL,	ERR_ST_ERROR,	"Invalid argument",0); \
	ERRLIST_ADD(ERRL,EOPT_PROTO,	ERR_ST_ERROR,	"Unknown protocol name",err_opt_proto); \
	ERRLIST_ADD(ERRL,EHOSTNAME_INVAL,ERR_ST_ERROR,	"Invalid hostname",0); \
	ERRLIST_ADD(ERRL,EREGEX_INVAL,	ERR_ST_ERROR,	"Invalid regular expression",0); \
	ERRLIST_ADD(ERRL,EOPT_INVAL,	ERR_ST_ERROR,	"Invalid option",0); \
	ERRLIST_ADD(ERRL,EOPT_UNKNOWN,	ERR_ST_ERROR,	"Unknown option",0); \
})

int error_display()
{
	int end;
	struct hashtable *errlist;
	struct hashtable_err_value *tptr;
	struct linked_list *ptr;

	errlist = hashtable_init(4);
	ERRLIST_INIT(errlist);

	end = 0;
	
	ptr = list_error;

	while (ptr)
	{
		if (unlikely(ptr->value))
		{
			tptr = hashtable_err_lookup(errlist,ptr->value->u.err->id);
			print_error(headerfd,2,"error: ",tptr->msg);
			if (likely(ptr->value->u.err->arg))
				print_error(headerfd,3," \"",ptr->value->u.err->arg,"\"");
			if (tptr->func)
				(tptr->func)(headerfd);
			print_newline(headerfd);
			end = end | tptr->state; 
		}
		ptr = ptr->next;
	}
	
	hashtable_delete(errlist);

	return (end & ERR_ST_ERROR);
}

#define ERR_PROTO_PRINTNAME(F,T) __extension__ \
({ \
	int i; \
	i = PROTO_MIN + 1; \
	while (T[i].id != PROTO_MAX) \
		print_error(F,2,T[i++].name,","); \
})

void err_opt_proto(int fd)
{
	print_error(fd,1,"\n\nAvailable protocols :\n  -datalink layer: \n\t");
	ERR_PROTO_PRINTNAME(fd,st_proto);
	print_error(fd,1,"\010.\n  -network layer: \n\t");
	ERR_PROTO_PRINTNAME(fd,st_ether);
	print_error(fd,1,"\010.\n  -transport layer: \n\t");
	ERR_PROTO_PRINTNAME(fd,st_ip);
	print_error(fd,1,"\010.");
}
