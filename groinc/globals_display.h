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

#ifndef _GLOBALS_DISPLAY_H
#define _GLOBALS_DISPLAY_H

#include "tools/linked_list.h"

extern char	opt_output,
		opt_outputdata;

extern char	*inputfile,
		*outputfile;

extern int 	datafd,
      		headerfd;

extern struct linked_list 	*list_display_packet,
				*list_display_report,
				*list_report;

#endif

