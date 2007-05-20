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

#include "protocols.h"
#include "../tools/memory_tools.h"

#define LOOKUP_PROTOCOLID(N,T) \
	int end,i; \
	end = -1; \
	i = PROTO_MIN; \
	while ((T[i].id != PROTO_MAX) && (my_strcmp(T[i].name,N))) \
		i++; \
	if (T[i].id != PROTO_MAX) \
		end = T[i].id; \
	return end; 

#define LOOKUP_PROTOCOLNAME(I,T) \
	char *end; \
	int i; \
	end = ""; \
	i = PROTO_MIN; \
	while ((T[i].id != PROTO_MAX) && (T[i].id != I)) \
		i++; \
	if (T[i].id != PROTO_MAX) \
		end = T[i].name; \
	return end;

/* all the names are in upper case, use toupper() in your functions */
static struct protoname protonames[] = {
	{	PROTO_MIN,	""			},

	{	PROTO_ETHER,	"ETHERNET",		},

	{	PROTO_MAX,	""			} };

static struct protoname ethnames[] = {
	{	PROTO_MIN,		""		},
	
	{	ETHPROTO_IP,		"IP"		},
	{	ETHPROTO_IPV6,		"IPV6"		},
	{	ETHPROTO_ARP,		"ARP"		},
	{	ETHPROTO_RARP,		"RARP"		},
	{	ETHPROTO_XNS,		"XNS"		},
	{	ETHPROTO_APPLETALK,	"APPLETALK"	},
	
	{	PROTO_MAX,		""		} };

static struct protoname ipnames[] = {
	{	PROTO_MIN,		""		},
	
	{	IPPROTO_IP,		"IP" 		},
	{	IPPROTO_ICMP,		"ICMP"		},
	{	IPPROTO_IGMP,		"IGMP"		},
	{	IPPROTO_GGP,		"GGP"		},
	{	IPPROTO_IPIP,		"IPIP"		},
	{	IPPROTO_ST,		"ST"		},
	{	IPPROTO_TCP,		"TCP"		},
	{	IPPROTO_UCL,		"UCL"		},
	{	IPPROTO_EGP,		"EGP"		},
	{	IPPROTO_IGP,		"IGP"		},
	{	IPPROTO_BBNRRCMON,	"BBNRRCMON"	},
	{	IPPROTO_NVP,		"NVP"		},
	{	IPPROTO_PUP,		"PUP"		},
	{	IPPROTO_ARGUS,		"ARGUS"		},
	{	IPPROTO_EMCON,		"EMCON"		},
	{	IPPROTO_XNET,		"XNET"		},
	{	IPPROTO_CHAOS,		"CHAOS"		},
	{	IPPROTO_UDP,		"UDP"		},
	{	IPPROTO_MUX,		"MUX"		},
	{	IPPROTO_DCNMEAS,	"DCNMEAS"	},
	{	IPPROTO_HMP,		"HMP"		},
	{	IPPROTO_PRM,		"PRM"		},
	{	IPPROTO_XNSIDP,		"XNSIDP"	},
	{	IPPROTO_TRUNK1,		"TRUNK1"	},
	{	IPPROTO_TRUNK2,		"TRUNK2"	},
	{	IPPROTO_LEAF1,		"LEAF1"		},
	{	IPPROTO_LEAF2,		"LEAF2"		},
	{	IPPROTO_RDP,		"RDP"		},
	{	IPPROTO_IRTP,		"IRTP"		},
	{	IPPROTO_ISOTP4,		"ISOTP4"	},
	{	IPPROTO_NETBLT,		"NETBLT"	},
	{	IPPROTO_MFENSP,		"MFENSP"	},
	{	IPPROTO_MERITINP,	"MERITINP"	},
	{	IPPROTO_SEP,		"SEP"		},
	{	IPPROTO_3PC,		"3PC"		},
	{	IPPROTO_IDPR,		"IDPR"		},
	{	IPPROTO_XTP,		"XTP"		},
	{	IPPROTO_DDP,		"DDP"		},
	{	IPPROTO_IDPRCMTP,	"IDPRCMTP"	},
	{	IPPROTO_TPPP,		"TPPP"		},
	{	IPPROTO_IL,		"IL"		},
	{	IPPROTO_SIP,		"SIP"		},
	{	IPPROTO_SDRP,		"SDRP"		},
	{	IPPROTO_SIPSR,		"SIPSR"		},
	{	IPPROTO_SIPFRAG,	"SIPFRAG"	},
	{	IPPROTO_IDRP,		"IDRP"		},
	{	IPPROTO_RSVP,		"RSVP"		},
	{	IPPROTO_GRE,		"GRE"		},
	{	IPPROTO_MHRP,		"MHRP"		},
	{	IPPROTO_BNA,		"BNA"		},
	{	IPPROTO_SIPPESSP,	"SIPPESSP"	},
	{	IPPROTO_SIPPAH,		"SIPPAH"	},
	{	IPPROTO_INLS,		"INLS"		},
	{	IPPROTO_SWIPE,		"SWIPE"		},
	{	IPPROTO_NHRP,		"NHRP"		},
	{	IPPROTO_ANYHIP,		"ANYHIP"	},
	{	IPPROTO_CFTP,		"CFTP"		},
	{	IPPROTO_ANY,		"ANY"		},
	{	IPPROTO_SATEXPAK,	"SATEXPAK"	},
	{	IPPROTO_KRYPTOLAN,	"KRYPTOLAN"	},
	{	IPPROTO_RVD,		"RVD"		},
	{	IPPROTO_IPPC,		"IPPC"		},
	{	IPPROTO_ANYDFS,		"ANYDFS"	},
	{	IPPROTO_SATMON,		"SATMON"	},
	{	IPPROTO_VISA,		"VISA"		},
	{	IPPROTO_IPCV,		"IPCV"		},
	{	IPPROTO_CPNX,		"CPNX"		},
	{	IPPROTO_CPHB,		"CPHB"		},
	{	IPPROTO_WSN,		"WSN"		},
	{	IPPROTO_PVP,		"PVP"		},
	{	IPPROTO_BRSATMON,	"BRSATMON"	},
	{	IPPROTO_SUNND,		"SUNND"		},
	{	IPPROTO_WBMON,		"WBMON"		},
	{	IPPROTO_WBEXPAK,	"WBEXPAK"	},
	{	IPPROTO_ISOIP,		"ISOIP"		},
	{	IPPROTO_VMTP,		"VMTP"		},
	{	IPPROTO_SECUREVMTP,	"SECUREVMTP"	},
	{	IPPROTO_VINES,		"VINES"		},
	{	IPPROTO_TTP,		"TTP"		},
	{	IPPROTO_NSFNETIGP,	"NSFNETIGP"	},
	{	IPPROTO_DGP,		"DGP"		},
	{	IPPROTO_TCF,		"TCF"		},
	{	IPPROTO_IGRP,		"IGRP"		},
	{	IPPROTO_OSPFIGP,	"OSPFIGP"	},
	{	IPPROTO_SPRITERPC,	"SPRITERPC"	},
	{	IPPROTO_LARP,		"LARP"		},
	{	IPPROTO_MTP,		"MTP"		},
	{	IPPROTO_AX25,		"AX25"		},
	{	IPPROTO_IPWIP,		"IPWIP"		},
	{	IPPROTO_MICP,		"MICP"		},
	{	IPPROTO_SCCSP,		"SCCSP"		},
	{	IPPROTO_ETHERIP,	"ETHERIP"	},
	{	IPPROTO_ENCAP,		"ENCAP"		},
	{	IPPROTO_ANYPES,		"ANYPES"	},
	{	IPPROTO_GMTP,		"GMTP"		},
	{	IPPROTO_RAW,		"RAW"		},

	{	PROTO_MAX,		""		} };


char *lookup_protoname(int id)
{
	LOOKUP_PROTOCOLNAME(id,protonames);
}

char *lookup_ethname(int id)
{
	LOOKUP_PROTOCOLNAME(id,ethnames);
}

char *lookup_ipname(int id)
{
	LOOKUP_PROTOCOLNAME(id,ipnames);
}

/* all the names are in upper case, use toupper() in your functions */

int lookup_protoid(char *name)
{
	LOOKUP_PROTOCOLID(name,protonames);
}

int lookup_ethid(char *name)
{
	LOOKUP_PROTOCOLID(name,ethnames);
}

int lookup_ipid(char *name)
{
	LOOKUP_PROTOCOLID(name,ipnames);
}

