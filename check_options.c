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

#include "globals_filter.h"
#include "globals_args.h"
#include "globals_error.h"
#include "network/protocols.h"
#include "tools/memory_tools.h"
#include "tools/network_tools.h"
#include "tools/math_tools.h"
#include "parse_options.h"
#include "error.h"

#include <regex.h>

__inline__ int chk_output(char *val)
{
	return OPT_OK;
}

__inline__ int chk_outputdata(char *val)
{
	return OPT_OK;
}

__inline__ int chk_inputfile(char *val)
{
	return OPT_OK;
}

__inline__ int chk_outputfile(char *val)
{
	return OPT_OK;
}

__inline__ int chk_flt_dstport(char *val)
{
	/* test valid port */
	return OPT_OK;
}

__inline__ int chk_flt_srcport(char *val)
{
	return OPT_OK;
}

__inline__ int chk_flt_globalport(char *val)
{
	return OPT_OK;
}

__inline__ int chk_flt_srcip(char *val)
{
	/* test valid ip */
	return OPT_OK;
}

__inline__ int chk_flt_dstip(char *val)
{
	return OPT_OK;
}

__inline__ int chk_flt_globalip(char *val)
{
	return OPT_OK;
}

__inline__ int chk_flt_srcmac(char *val)
{
	return OPT_OK;
}

__inline__ int chk_flt_dstmac(char *val)
{
	return OPT_OK;
}

__inline__ int chk_flt_limitnb(char *val)
{
	return OPT_OK;
}

__inline__ int chk_flt_timelimit(char *val)
{
	return OPT_OK;
}

__inline__ int chk_flt_filterstr(char *val)
{
	return OPT_OK;
}

__inline__ int chk_flt_filterregex(char *val)
{
	regex_t tmp;
	if (regcomp(&tmp,val,REG_NOSUB))
	{
		regfree(&tmp);
		ERR_SET(list_error,EREGEX_INVAL,val);
		return OPT_ERROR;
	}
	else
	{
		regfree(&tmp);
		return OPT_OK;
	}
}

__inline__ int chk_flt_protocol(char *val)
{
	if (lookup_protoid(strupr(val)) < 0)
	{
		ERR_SET(list_error,EARG_INVAL,val);
		return OPT_ERROR;
	}
	else
	{
		return OPT_OK;
	}
}

__inline__ int chk_flt_ethprotocol(char *val)
{
	if (lookup_ethid(strupr(val)) < 0)
	{
		ERR_SET(list_error,EARG_INVAL,val);
		return OPT_ERROR;
	}
	else
	{
		return OPT_OK;
	}
}

__inline__ int chk_flt_ipprotocol(char *val)
{
	if (lookup_ipid(strupr(val)) < 0)
	{
		ERR_SET(list_error,EARG_INVAL,val);
		return OPT_ERROR;
	}
	else
	{
		return OPT_OK;
	}
}

