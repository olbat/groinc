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


#define CHK_FLT_HOST_REGEX_HOSTNAME \
	"^\\([a-zA-Z0-9\\-]\\+\\.\\)*[a-zA-Z0-9\\-]\\{1,63\\}\\." \
	"[0-9]*[a-zA-Z\\-][a-zA-Z0-9\\-]*$"

#define CHK_FLT_HOST_REGEX_IP_NUM \
	"\\([0-9]\\{1,2\\}\\|[01][0-9]\\{2\\}\\|2[0-4][0-9]\\|25[0-5]\\)"

#define CHK_FLT_HOST_REGEX_CIDR_NUM \
	"\\([0-2]\\?[0-9]\\|3[0-2]\\)"

#define CHK_FLT_HOST_REGEX_IP_ADDR \
	"\\(" CHK_FLT_HOST_REGEX_IP_NUM "\\.\\)\\{3\\}" \
	CHK_FLT_HOST_REGEX_IP_NUM

#define CHK_FLT_HOST_REGEX_IP "^" CHK_FLT_HOST_REGEX_IP_ADDR "$"

#define CHK_FLT_HOST_REGEX_IPNETMASK \
	"^" CHK_FLT_HOST_REGEX_IP_ADDR ":" CHK_FLT_HOST_REGEX_IP_ADDR "$"

#define CHK_FLT_HOST_REGEX_CIDR \
	"^\\(" CHK_FLT_HOST_REGEX_IP_NUM "\\.\\)\\{0,3\\}" \
	CHK_FLT_HOST_REGEX_IP_NUM "/" CHK_FLT_HOST_REGEX_CIDR_NUM "$"

int check_options();

int chk_output(char *val);
int chk_outputdata(char *val);
int chk_inputfile(char *val);
int chk_outputfile(char *val);
int chk_flt_dstport(char *val);
int chk_flt_srcport(char *val);
int chk_flt_globalport(char *val);
int chk_flt_srcip(char *val);
int chk_flt_dstip(char *val);
int chk_flt_globalip(char *val);
int chk_flt_srcmac(char *val);
int chk_flt_dstmac(char *val);
int chk_flt_limitnb(char *val);
int chk_flt_timelimit(char *val);
int chk_flt_filterstr(char *val);
int chk_flt_filterregex(char *val);
int chk_flt_protocol(char *val);

#endif

