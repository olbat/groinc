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

#include "parse_options.h"
#include "globals_args.h"
#include "globals_option.h"
#include "tools/memory_tools.h"
#include "network/protocols.h"

enum { OPT_ERROR,OPT_HELP,OPT_DSTPORT,OPT_DSTIP,OPT_SRCPORT,OPT_SRCIP,OPT_OUTPUT,OPT_OUTPUTDATA, OPT_DISPLAYDATA,OPT_DISPLAYHEADER,OPT_DISPLAYPACKETS,OPT_DISPLAYALLPACKETS,OPT_FILTERSTR,OPT_IPPROTOCOL,OPT_SIMPLEDISPLAY,OPT_DISPLAYTLPROTO,OPT_FILTERREGEX,OPT_DONTDISPLAYEMPTYSLP,OPT_ETHPROTOCOL,OPT_DISPLAYNLPROTO,OPT_DISPLAYDLPROTO,OPT_PROTOCOL,OPT_SRCMAC,OPT_DSTMAC,OPT_GLOBALIP,OPT_GLOBALPORT,OPT_INPUTFILE,OPT_OUTPUTFILE,OPT_NULL,OPT_END };

static struct optlist po_optlist[] = {
	{'d',	"destport",	OPT_DSTPORT,	PO_ARG },
	{'D',	"destip",	OPT_DSTIP,	PO_ARG },
	{'s',	"sourceport",	OPT_SRCPORT,	PO_ARG },
	{'S',	"sourceip",	OPT_SRCIP,	PO_ARG },
	{'o',	"outputdata",	OPT_OUTPUTDATA,	PO_ARG },
	{'O',	"output",	OPT_OUTPUT,	PO_ARG },
	{'f',	"filter",	OPT_FILTERSTR,	PO_ARG },
	{'q',	"protocol",	OPT_PROTOCOL,	PO_ARG },
	{'p',	"ipprotocol", 	OPT_IPPROTOCOL,	PO_ARG },
	{'P',	"ethprotocol",	OPT_ETHPROTOCOL,PO_ARG },
	{'F',	"filter-regex",	OPT_FILTERREGEX,PO_ARG },
	{'m',	"sourcemac",	OPT_SRCMAC,	PO_ARG },
	{'M',	"destmac",	OPT_DSTMAC,	PO_ARG },
	{'g',	"globalport",	OPT_GLOBALPORT,	PO_ARG },
	{'G',	"globalip",	OPT_GLOBALIP,	PO_ARG },
	{'l',	"load",		OPT_INPUTFILE,	PO_ARG },	
	{'w',	"save",		OPT_OUTPUTFILE,	PO_ARG },
	{'a',	"displaydata",	OPT_DISPLAYDATA,	PO_NOARG },
	{'A',	"simpledisplay",OPT_SIMPLEDISPLAY,	PO_NOARG },
	{'v',	"verbose",	OPT_DISPLAYHEADER,	PO_NOARG },
	{'n',	"displaypackets",OPT_DISPLAYPACKETS,	PO_NOARG },
	{'N',	"displayallpackets",	OPT_DISPLAYALLPACKETS,		PO_NOARG },
	{'z',	"dontdisplayemptysp",	OPT_DONTDISPLAYEMPTYSLP,	PO_NOARG },
	{'c',	"displayprotodatalink",	OPT_DISPLAYDLPROTO,	PO_NOARG },
	{'b',	"displayprototransport",OPT_DISPLAYTLPROTO,	PO_NOARG },
	{'B',	"displayprotonetwork",	OPT_DISPLAYNLPROTO,	PO_NOARG },
	{'h',	"help", 	OPT_HELP,		PO_NOARG },
	{'\0',	"",		OPT_END,	PO_NOARG}
};
static char *value;

int lookup_short(char c, struct optlist *optlist)
{
	int end,i;
	end = OPT_NULL;
	i = 0;
	while ((end == OPT_NULL) && (optlist[i].id != OPT_END))
	{
		if (optlist[i].name_short == c)
		{
			end = i;
		}
		i++;
	}
	return end;
}

int lookup_long(char *str, struct optlist *optlist)
{
	int end,i;
	end = OPT_NULL;
	i = 0;
	while ((end == OPT_NULL) && (optlist[i].id != OPT_END))
	{
		if (my_strcmp(optlist[i].name_long,str) == 0) 
		{
			end = i;
		}
		i++;
	}
	return end;
}

int lookup_options(int argc, char **argv,struct optlist *optlist)
{
	static int args = 1;
	char *ptr;
	int end,ptri;
	end = OPT_NULL;
	if (args <= argc)
	{
		ptr = argv[args];
		if (*ptr == '-')
		{
			if (*(ptr + 1) == '-')
			{
				ptri = lookup_long(ptr + 2,optlist);
			}
			else
			{
				ptri = lookup_short(*(ptr + 1),optlist);
			}
			
			if (ptri != OPT_NULL)
			{
				if (optlist[ptri].flags == PO_ARG)
				{
					if (args < argc)
					{
						if (*argv[args + 1] != '-')
						{
							value = argv[args + 1];
							args++;
						}
						else
						{
							value = argv[args];
							return OPT_ERROR;
						}
					}
					else 
					{
						value = argv[args];
						return OPT_ERROR;
					}
				}
				args++;
				end = optlist[ptri].id;
			}
			else
			{
				value = argv[args];
				end = OPT_ERROR;
			}
		}
		else
		{
			value = argv[args];
			end = OPT_ERROR;
		}
	}
	else
	{
		end = OPT_END;
	}
	return end;
}
int parse_options(int argc, char **argv)
{
	int o;
	while ((o = lookup_options(argc,argv,po_optlist)) != OPT_END)
	{
		switch (o)
		{
			case OPT_ERROR :
				po_error = value;
				return P_ERROR;
				break;
			case OPT_HELP :
				return P_ERROR;
				break;
			case OPT_DSTPORT :
				fdestport = value;
				break;
			case OPT_DSTIP :
				fdestip = value;
				break;
			case OPT_SRCPORT :
				fsourceport = value;
				break;
			case OPT_SRCIP :
				fsourceip = value;
				break;
			case OPT_GLOBALIP :
				fglobalip = value;
				break;
			case OPT_GLOBALPORT :
				fglobalport = value;
				break;
			case OPT_SRCMAC :
				fsourcemac = value;
				break;
			case OPT_DSTMAC :
				fdestmac = value;
				break;
			case OPT_SIMPLEDISPLAY:
				opt_simpledisplay = 1;
				break;
			case OPT_DISPLAYDATA :
				opt_displaydata = 1;
				break;
			case OPT_DISPLAYHEADER :
				opt_displayheader = 1;
				break;
			case OPT_DISPLAYPACKETS :
				opt_displaypackets = 1;
				break;
			case OPT_DISPLAYALLPACKETS :
				opt_displayallpackets = 1;
				break;
			case OPT_DISPLAYDLPROTO :
				opt_displaydlproto = 1;
				break;
			case OPT_DISPLAYNLPROTO :
				opt_displaynlproto = 1;
				break;
			case OPT_DISPLAYTLPROTO :
				opt_displaytlproto = 1;
				break;
			case OPT_DONTDISPLAYEMPTYSLP :
				opt_ndisplayemptyslp = 1;
				break;
			case OPT_OUTPUT :
				opt_output = 1;
				output = value;
				break;
			case OPT_OUTPUTDATA :
				opt_outputdata = 1;
				outputdata = value;
				break;
			case OPT_FILTERSTR :
				filterstr = value;
				break;
			case OPT_FILTERREGEX :
				filterregexstr = value;
				//regcomp(&filterregex,value,0);
				break;
			case OPT_PROTOCOL :
				protoname = value;
				break;
			case OPT_ETHPROTOCOL :
				ethprotoname = value;
				break;
			case OPT_IPPROTOCOL :
				ipprotoname = value;
				break;
			case OPT_INPUTFILE :
				inputfile = value;
				break;
			case OPT_OUTPUTFILE :
				outputfile = value;
				break;
		}
	}
	return 0;
}
