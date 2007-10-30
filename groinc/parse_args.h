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

#ifndef _PARSE_ARGS_H
#define _PARSE_ARGS_H

#include "tools/linked_list.h"

__inline__ int prs_dsp_pkt_help(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_dsp_pkt_version(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_dsp_pkt_license(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_dsp_pkt_displayopt(struct linked_list_opt_value *optl, char *val);

__inline__ int prs_dsp_rpt_timetot(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_dsp_rpt_countpackets(struct linked_list_opt_value *optl, char *val);

__inline__ int prs_output(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_outputdata(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_inputfile(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_outputfile(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_noresolv(struct linked_list_opt_value *optl, char *val);

__inline__ int prs_flt_dstport(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_flt_srcport(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_flt_globalport(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_flt_srcip(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_flt_dstip(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_flt_globalip(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_flt_srcmac(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_flt_dstmac(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_flt_limitnb(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_flt_timelimit(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_flt_filterstr(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_flt_filterregex(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_flt_protocol(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_flt_ethprotocol(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_flt_ipprotocol(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_flt_dontdisplayemptysl(struct linked_list_opt_value *optl, char *val);
__inline__ int prs_flt_dontdisplaypackets(struct linked_list_opt_value *optl, char *val);

#endif
