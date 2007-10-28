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

#ifndef _ERROR_H
#define _ERROR_H

#include "tools/math_tools.h"

enum err_id
{
	EARG_MISSING = 1,
	EARG_INVAL,
	EHOSTNAME_INVAL,
	EREGEX_INVAL,
	EOPT_INVAL,
	EOPT_UNKNOWN
};

enum err_state
{
	ERR_ST_WARNING = 0x1,
	ERR_ST_ERROR = 0x2
};

struct hashtable_err_value
{
	enum err_id id;
	char *msg;
	enum err_state state;
};

struct linked_list_err_value
{
	enum err_id id;
	char *arg;
};

#define ERR_SET(L,ID,A) \
	linked_list_add(L,linked_list_err_value_init(ID,A)); 

int error_display();

#endif
