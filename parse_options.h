/* This file is a part of groinc
 * Copyright 2006 Sarzyniec Luc <olbat@xiato.com>
 * This software is released under GPL the license
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

