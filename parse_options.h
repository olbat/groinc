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

#ifndef _PARSE_OPTIONS_H
#define _PARSE_OPTIONS_H

#define P_MISC -2 
#define P_ERROR -1
#define PO_NOARG 0
#define PO_ARG 1
#define PO_OPTIONALARGS 2

struct optlist {
	char name_short;
	char *name_long;
	int id;
	int flags;
};

int lookup_short(char c, struct optlist *optlist);
int lookup_long(char *str, struct optlist *optlist);
int lookup_options(int argc, char **argv,struct optlist *optlist);
int parse_options(int argc, char **argv);

#endif

