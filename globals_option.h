/* This file is a part of groinc
 * Copyright 2006 Sarzyniec Luc <olbat@xiato.com>
 * This software is released under GPL the license
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
		opt_displaytlproto;
extern char	*inputfile,
		*outputfile;
extern int 	datafd,
      		headerfd;

#endif

