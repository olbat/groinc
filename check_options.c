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

#include <netdb.h>

#include "check_options.h"
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
#include <sys/socket.h>

int chk_output(char *val)
{
	return OPT_OK;
}

int chk_outputdata(char *val)
{
	return OPT_OK;
}

int chk_inputfile(char *val)
{
	return OPT_OK;
}

int chk_outputfile(char *val)
{
	return OPT_OK;
}

int chk_flt_dstport(char *val)
{
	/* test valid port */
	return OPT_OK;
}

int chk_flt_srcport(char *val)
{
	return OPT_OK;
}

int chk_flt_globalport(char *val)
{
	return OPT_OK;
}

#define CHK_FLT_HOST(V) __extension__ \
	regex_t tmp; \
	regcomp(&tmp,CHK_FLT_HOST_REGEX_HOSTNAME,REG_NOSUB); \
	if (regexec(&tmp,V,0,0,0)) \
	{ \
		regfree(&tmp); \
		regcomp(&tmp,CHK_FLT_HOST_REGEX_CIDR,REG_NOSUB); \
		if (regexec(&tmp,V,0,0,0)) \
		{ \
			regfree(&tmp); \
			regcomp(&tmp,CHK_FLT_HOST_REGEX_IPNETMASK,REG_NOSUB); \
			if (regexec(&tmp,V,0,0,0)) \
			{ \
				regfree(&tmp); \
				regcomp(&tmp,CHK_FLT_HOST_REGEX_IP,REG_NOSUB); \
				if (regexec(&tmp,V,0,0,0)) \
					goto err; \
				else \
					goto out; \
			} \
			else \
				goto out; \
		} \
		else \
			goto out;\
	} \
	else \
		goto name; \
err: \
	regfree(&tmp); \
	ERR_SET(list_error,EHOSTNAME_INVAL,V); \
	return OPT_ERROR; \
name: \
	if (!gethostbyname(V)) \
	{ \
		herror("Cannot resolve hostname"); \
		goto err; \
	} \
out: \
	regfree(&tmp); \
	return OPT_OK;

int chk_flt_srcip(char *val)
{
	CHK_FLT_HOST(val);
}

int chk_flt_dstip(char *val)
{
	CHK_FLT_HOST(val);
}

int chk_flt_globalip(char *val)
{
	CHK_FLT_HOST(val);
}

int chk_flt_srcmac(char *val)
{
	return OPT_OK;
}

int chk_flt_dstmac(char *val)
{
	return OPT_OK;
}

int chk_flt_limitnb(char *val)
{
	return OPT_OK;
}

int chk_flt_timelimit(char *val)
{
	return OPT_OK;
}

int chk_flt_filterstr(char *val)
{
	return OPT_OK;
}

int chk_flt_filterregex(char *val)
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

int chk_flt_protocol(char *val)
{
	if ((lookup_protoid(strupr(val)) < 0) && (lookup_ethid(strupr(val)) < 0)
	   && (lookup_ipid(strupr(val)) < 0))
	{
		ERR_SET(list_error,EOPT_PROTO,val);
		return OPT_ERROR;
	}
	else
	{
		return OPT_OK;
	}
}

