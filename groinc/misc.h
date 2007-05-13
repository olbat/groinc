/* This file is a part of groinc
 * Copyright 2006 Sarzyniec Luc <olbat@xiato.com>
 * This software is released under GPL the license
 * see the COPYING file for more informations */

#ifndef _MISC_H
#define _MISC_H

#define GROINC_VERSION "groinc 0.0.9rc3 (20070513)"

enum miscno 
{
	MISCNO_LICENSE,
	MISCNO_VERSION
};

void print_misc(enum miscno no);

#endif
