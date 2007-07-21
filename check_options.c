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
#include "network/protocols.h"
#include "tools/memory_tools.h"
#include "tools/network_tools.h"
#include "tools/math_tools.h"

int check_options()
{
	int end;
	end = 0;
	if (*filterregexstr)
	{
		if (regcomp(&filterregex,filterregexstr,REG_NOSUB))
		{
			co_error = filterregexstr;
			end = 1;
		}
	}
	if (*ipprotoname)
	{
		int tmp;	
		if ((tmp = lookup_ipid(strupr(ipprotoname))) < 0)
		{
			co_error = ipprotoname;
			end = 1;
		}
		else
		{
			ipproto = (unsigned int) tmp;
		}
		filter_transport = 1;
	}
	if (*ethprotoname)
	{
		int tmp;	
		if ((tmp = lookup_ethid(strupr(ethprotoname))) < 0)
		{
			co_error = ethprotoname;
			end = 1;
		}
		else
		{
			ethproto = (unsigned int) tmp;
		}
		filter_network = 1;
	}
	if (*protoname)
	{
		int tmp;	
		if ((tmp = lookup_protoid(strupr(protoname))) < 0)
		{
			co_error = protoname;
			end = 1;
		}
		else
		{
			proto = (unsigned int) tmp;
		}
		filter_datalink = 1;
	}
	if (*fsourceport)
	{
		lsourceport = my_atoi(fsourceport,10);
		filter_transport = 1;
	}
	if (*fdestport)
	{
		ldestport = my_atoi(fdestport,10);
		filter_transport = 1;
	}
	if (*fglobalport)
	{
		lglobalport = my_atoi(fglobalport,10);
		filter_transport = 1;
	}
	if (*fglobalip)
	{
		lglobalip = ipv4_aton(fglobalip);
		filter_network = 1;
	}
	if (*fsourceip)
	{
		lsourceip = ipv4_aton(fsourceip);
		filter_network = 1;
	}
	if (*fdestip)
	{
		ldestip = ipv4_aton(fdestip);
		filter_network = 1;
	}
	if (*fdestmac)
	{
		mac_aton(fdestmac,ldestmac);
		filter_datalink = 1;
	}
	if (*fsourcemac)
	{
		mac_aton(fsourcemac,lsourcemac);
		filter_datalink = 1;
	}
	if (*flimitnb)
	{
		llimitnb = (long int) my_atoi(flimitnb,10);
	}
	if (*ftimelimit)
	{
		gettimeofday(&timelimit,0);
		timelimit.tv_sec = timelimit.tv_sec + my_atoi(ftimelimit,10);
	}

	if ((!ldestip) && (!ldestport) && (!lsourceip) && (!lsourceport)
	&& (!lglobalip) && (!lglobalport) && (mac_null(lsourcemac)) 
	&& (mac_null(ldestmac)) && (ipproto == IPPROTO_RAW) && (ethproto == ETHPROTO_RAW))
	{
		nofilter = 1;
	}
	else
	{
		nofilter = 0;
	}

	return end;
}
