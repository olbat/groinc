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

#ifndef _CHECK_OPTIONS_H
#define _CHECK_OPTIONS_H

int check_options();

__inline__ int chk_output(char *val);
__inline__ int chk_outputdata(char *val);
__inline__ int chk_inputfile(char *val);
__inline__ int chk_outputfile(char *val);
__inline__ int chk_flt_dstport(char *val);
__inline__ int chk_flt_srcport(char *val);
__inline__ int chk_flt_globalport(char *val);
__inline__ int chk_flt_srcip(char *val);
__inline__ int chk_flt_dstip(char *val);
__inline__ int chk_flt_globalip(char *val);
__inline__ int chk_flt_srcmac(char *val);
__inline__ int chk_flt_dstmac(char *val);
__inline__ int chk_flt_limitnb(char *val);
__inline__ int chk_flt_timelimit(char *val);
__inline__ int chk_flt_filterstr(char *val);
__inline__ int chk_flt_filterregex(char *val);
__inline__ int chk_flt_protocol(char *val);

#endif

