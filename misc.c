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

#include "misc.h"
#include "globals_option.h"
#include "prints.h"

void print_misc(enum miscno no)
{
	if (no == MISCNO_VERSION)
		print_format(headerfd,GROINC_VERSION "\nThis is free software; see the source or use --license for copying conditions.\n");
	else if (no == MISCNO_LICENSE)
		print_format(headerfd,	"Groinc is Copyright 2006,2007 by Sarzyniec Luc <olbat@xiato.com>.\n\n"
					"Groinc is free software, covered by the \"GNU General Public License\", you are\n"
					"welcome to change it and/or distribute copies of it under certain conditions.\n"
					"There is absolutely no warranty for groinc.\n\n"
					"See the source for copying conditions.\n"
					"You can also get a copy of the \"GNU General Public License\" at this address :\n"
					"http://www.gnu.org/copyleft/gpl.html\n");
}

