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

#include "protocols.h"
#include "../protocols/scan_proto.h"
#include "../protocols/print_proto.h"
#include "../tools/memory_tools.h"

#include <string.h>

#define LOOKUP_PROTOCOLID(N,T) \
__extension__ \
({ \
	int end,i; \
	i = PROTO_MIN; \
	while ((T[i].id != PROTO_MAX) && (strcmp(T[i].name,N))) \
		i++; \
	if (T[i].id != PROTO_MAX) \
		end = T[i].id; \
	else \
		end = -1; \
	end; \
})

#define LOOKUP_PROTOCOLNAME(I,T) \
__extension__ \
({ \
	char *end; \
	int i; \
	i = PROTO_MIN; \
	while ((T[i].id != PROTO_MAX) && (T[i].id != I)) \
		i++; \
	if (T[i].id != PROTO_MAX) \
		end = T[i].name; \
	else \
		end = ""; \
	end; \
)}

#define LOOKUP_PROTOCOLST(I,T,V) \
__extension__ \
({ \
	int i; \
	i = PROTO_MIN; \
	while ((T[i].id != PROTO_MAX) && (T[i].id != I)) \
		i++; \
	if (T[i].id != PROTO_MAX) \
		return T[i].V; \
	else \
		return 0; \
})

/* all the names are in upper case, use toupper() in your functions */
static struct st_protocol st_proto[] = {
	{	PROTO_MIN,	"",	0,	0	},

	{	PROTO_ETHER,	"ETHERNET",	&scan_ether,	&print_ether	},

	{	PROTO_MAX,	"",	0,	0 } };

static struct st_protocol st_ether[] = {
	{	PROTO_MIN,		"",	0,	0	},
	
	{	ETHPROTO_IP,		"IP",		&scan_ipv4,	&print_ipv4	},
	{	ETHPROTO_IPV6,		"IPV6",		0,	0	},
	{	ETHPROTO_ARP,		"ARP",		&scan_arp,	&print_arp	},
	{	ETHPROTO_RARP,		"RARP",		0,	0	},
	{	ETHPROTO_XNS,		"XNS",		0,	0	},
	{	ETHPROTO_APPLETALK,	"APPLETALK",	0,	0	},
	
	{	PROTO_MAX,		"",	0,	0	} };

static struct st_protocol st_ip[] = {
	{	PROTO_MIN,		"",	0,	0	},
	
	{	IPPROTO_ICMP,		"ICMP",		&scan_icmp,	&print_icmp	},
	{	IPPROTO_UDP,		"UDP",		&scan_udp,	&print_udp	},
	{	IPPROTO_TCP,		"TCP",		&scan_tcp,	&print_tcp	},
	{	IPPROTO_IP,		"IP",		0,	0	},
	{	IPPROTO_IGMP,		"IGMP",		0,	0	},
	{	IPPROTO_GGP,		"GGP",		0,	0	},
	{	IPPROTO_IPIP,		"IPIP",		0,	0	},
	{	IPPROTO_ST,		"ST",		0,	0	},
	{	IPPROTO_UCL,		"UCL",		0,	0	},
	{	IPPROTO_EGP,		"EGP",		0,	0	},
	{	IPPROTO_IGP,		"IGP",		0,	0	},
	{	IPPROTO_BBNRRCMON,	"BBNRRCMON",	0,	0	},
	{	IPPROTO_NVP,		"NVP",		0,	0	},
	{	IPPROTO_PUP,		"PUP",		0,	0	},
	{	IPPROTO_ARGUS,		"ARGUS",	0,	0	},
	{	IPPROTO_EMCON,		"EMCON",	0,	0	},
	{	IPPROTO_XNET,		"XNET",		0,	0	},
	{	IPPROTO_CHAOS,		"CHAOS",	0,	0	},
	{	IPPROTO_MUX,		"MUX",		0,	0	},
	{	IPPROTO_DCNMEAS,	"DCNMEAS",	0,	0	},
	{	IPPROTO_HMP,		"HMP",		0,	0	},
	{	IPPROTO_PRM,		"PRM",		0,	0	},
	{	IPPROTO_XNSIDP,		"XNSIDP",	0,	0	},
	{	IPPROTO_TRUNK1,		"TRUNK1",	0,	0	},
	{	IPPROTO_TRUNK2,		"TRUNK2",	0,	0	},
	{	IPPROTO_LEAF1,		"LEAF1",	0,	0	},
	{	IPPROTO_LEAF2,		"LEAF2",	0,	0	},
	{	IPPROTO_RDP,		"RDP",		0,	0	},
	{	IPPROTO_IRTP,		"IRTP",		0,	0	},
	{	IPPROTO_ISOTP4,		"ISOTP4",	0,	0	},
	{	IPPROTO_NETBLT,		"NETBLT",	0,	0	},
	{	IPPROTO_MFENSP,		"MFENSP",	0,	0	},
	{	IPPROTO_MERITINP,	"MERITINP",	0,	0	},
	{	IPPROTO_SEP,		"SEP",		0,	0	},
	{	IPPROTO_3PC,		"3PC",		0,	0	},
	{	IPPROTO_IDPR,		"IDPR",		0,	0	},
	{	IPPROTO_XTP,		"XTP",		0,	0	},
	{	IPPROTO_DDP,		"DDP",		0,	0	},
	{	IPPROTO_IDPRCMTP,	"IDPRCMTP",	0,	0	},
	{	IPPROTO_TPPP,		"TPPP",		0,	0	},
	{	IPPROTO_IL,		"IL",		0,	0	},
	{	IPPROTO_SIP,		"SIP",		0,	0	},
	{	IPPROTO_SDRP,		"SDRP",		0,	0	},
	{	IPPROTO_SIPSR,		"SIPSR",	0,	0	},
	{	IPPROTO_SIPFRAG,	"SIPFRAG",	0,	0	},
	{	IPPROTO_IDRP,		"IDRP",		0,	0	},
	{	IPPROTO_RSVP,		"RSVP",		0,	0	},
	{	IPPROTO_GRE,		"GRE",		0,	0	},
	{	IPPROTO_MHRP,		"MHRP",		0,	0	},
	{	IPPROTO_BNA,		"BNA",		0,	0	},
	{	IPPROTO_SIPPESSP,	"SIPPESSP",	0,	0	},
	{	IPPROTO_SIPPAH,		"SIPPAH",	0,	0	},
	{	IPPROTO_INLS,		"INLS",		0,	0	},
	{	IPPROTO_SWIPE,		"SWIPE",	0,	0	},
	{	IPPROTO_NHRP,		"NHRP",		0,	0	},
	{	IPPROTO_ANYHIP,		"ANYHIP",	0,	0	},
	{	IPPROTO_CFTP,		"CFTP",		0,	0	},
	{	IPPROTO_ANY,		"ANY",		0,	0	},
	{	IPPROTO_SATEXPAK,	"SATEXPAK",	0,	0	},
	{	IPPROTO_KRYPTOLAN,	"KRYPTOLAN",	0,	0	},
	{	IPPROTO_RVD,		"RVD",		0,	0	},
	{	IPPROTO_IPPC,		"IPPC",		0,	0	},
	{	IPPROTO_ANYDFS,		"ANYDFS",	0,	0	},
	{	IPPROTO_SATMON,		"SATMON",	0,	0	},
	{	IPPROTO_VISA,		"VISA",		0,	0	},
	{	IPPROTO_IPCV,		"IPCV",		0,	0	},
	{	IPPROTO_CPNX,		"CPNX",		0,	0	},
	{	IPPROTO_CPHB,		"CPHB",		0,	0	},
	{	IPPROTO_WSN,		"WSN",		0,	0	},
	{	IPPROTO_PVP,		"PVP",		0,	0	},
	{	IPPROTO_BRSATMON,	"BRSATMON",	0,	0	},
	{	IPPROTO_SUNND,		"SUNND",	0,	0	},
	{	IPPROTO_WBMON,		"WBMON",	0,	0	},
	{	IPPROTO_WBEXPAK,	"WBEXPAK",	0,	0	},
	{	IPPROTO_ISOIP,		"ISOIP",	0,	0	},
	{	IPPROTO_VMTP,		"VMTP",		0,	0	},
	{	IPPROTO_SECUREVMTP,	"SECUREVMTP",	0,	0	},
	{	IPPROTO_VINES,		"VINES",	0,	0	},
	{	IPPROTO_TTP,		"TTP",		0,	0	},
	{	IPPROTO_NSFNETIGP,	"NSFNETIGP",	0,	0	},
	{	IPPROTO_DGP,		"DGP",		0,	0	},
	{	IPPROTO_TCF,		"TCF",		0,	0	},
	{	IPPROTO_IGRP,		"IGRP",		0,	0	},
	{	IPPROTO_OSPFIGP,	"OSPFIGP",	0,	0	},
	{	IPPROTO_SPRITERPC,	"SPRITERPC",	0,	0	},
	{	IPPROTO_LARP,		"LARP",		0,	0	},
	{	IPPROTO_MTP,		"MTP",		0,	0	},
	{	IPPROTO_AX25,		"AX25",		0,	0	},
	{	IPPROTO_IPWIP,		"IPWIP",	0,	0	},
	{	IPPROTO_MICP,		"MICP",		0,	0	},
	{	IPPROTO_SCCSP,		"SCCSP",	0,	0	},
	{	IPPROTO_ETHERIP,	"ETHERIP",	0,	0	},
	{	IPPROTO_ENCAP,		"ENCAP",	0,	0	},
	{	IPPROTO_ANYPES,		"ANYPES",	0,	0	},
	{	IPPROTO_GMTP,		"GMTP",		0,	0	},
	{	IPPROTO_RAW,		"RAW",		0,	0	},

	{	PROTO_MAX,		"",	0,	0	} };


char *lookup_protoname(int id)
{
	/* LOOKUP_PROTOCOLNAME(id,st_proto); */
	LOOKUP_PROTOCOLST(id,st_proto,name);
}

char *lookup_ethname(int id)
{
	/* LOOKUP_PROTOCOLNAME(id,st_ether); */
	LOOKUP_PROTOCOLST(id,st_ether,name);
}

char *lookup_ipname(int id)
{
	/* LOOKUP_PROTOCOLNAME(id,st_ip); */
	LOOKUP_PROTOCOLST(id,st_ip,name);
}

void (*lookup_protoscan(int id))(struct data *, struct protocol_header *, struct protocol_header *)
{
	LOOKUP_PROTOCOLST(id,st_proto,func_scan);
}

void (*lookup_ethscan(int id))(struct data *, struct protocol_header *, struct protocol_header *)
{
	LOOKUP_PROTOCOLST(id,st_ether,func_scan);
}

void (*lookup_ipscan(int id))(struct data *, struct protocol_header *, struct protocol_header *)
{
	LOOKUP_PROTOCOLST(id,st_ip,func_scan);
}

void (*lookup_protoprint(int id))(int, char *)
{
	LOOKUP_PROTOCOLST(id,st_proto,func_print);
}

void (*lookup_ethprint(int id))(int, char *)
{
	LOOKUP_PROTOCOLST(id,st_ether,func_print);
}

void (*lookup_ipprint(int id))(int, char *)
{
	LOOKUP_PROTOCOLST(id,st_ip,func_print);
}
/* all the names are in upper case, use toupper() in your functions */

int lookup_protoid(char *name)
{
	return LOOKUP_PROTOCOLID(name,st_proto);
}

int lookup_ethid(char *name)
{
	return LOOKUP_PROTOCOLID(name,st_ether);
}

int lookup_ipid(char *name)
{
	return LOOKUP_PROTOCOLID(name,st_ip);
}

