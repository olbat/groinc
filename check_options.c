/* This file is a part of groinc
 * 
 * Copyright (C) 2006, 2007 Sarzyniec Luc <olbat@xiato.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
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
		if (regcomp(&filterregex,filterregexstr,0))
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
			ipproto = (unsigned char) tmp;
		}
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
			ethproto = (unsigned char) tmp;
		}
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
			proto = (unsigned char) tmp;
		}
	}
	if (*fsourceport)
	{
		lsourceport = my_atoi(fsourceport,10);
	}
	if (*fdestport)
	{
		ldestport = my_atoi(fdestport,10);
	}
	if (*fglobalport)
	{
		lglobalport = my_atoi(fglobalport,10);
	}
	if (*fglobalip)
	{
		lglobalip = ipv4_aton(fglobalip);
	}
	if (*fsourceip)
	{
		lsourceip = ipv4_aton(fsourceip);
	}
	if (*fdestip)
	{
		ldestip = ipv4_aton(fdestip);
	}
	if (*fdestmac)
	{
		ldestmac = (__u8 *) mac_aton(fdestmac);
	}
	if (*fsourcemac)
	{
		lsourcemac = (__u8 *) mac_aton(fsourcemac);
	}
	return end;
}
