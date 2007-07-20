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

#ifndef _GLOBALS_OPTION_H
#define _GLOBALS_OPTION_H

extern char	opt_output,
		opt_outputdata,
		opt_simpledisplay,
		opt_displaydata,
		opt_displayheader,
		opt_displaypackets,
		opt_displayallpackets,
		opt_ndisplayemptyslp,
		opt_displaydlproto,
		opt_displaynlproto,
		opt_displaytlproto,
		opt_displayhexa,
		opt_ndisplaypackets;

extern char	*inputfile,
		*outputfile;

extern int 	datafd,
      		headerfd;

#endif

