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

#ifndef _PROTOCOLS_H
#define _PROTOCOLS_H

/* struct for the translation array */
struct protoname
{
	int id;
	char *name;
};

/* the function to get the name/id associations */

char *lookup_protocolname(int id,struct protoname protonames[]);
int lookup_protocolid(char *name,struct protoname protonames[]);
char *lookup_protoname(int id);
char *lookup_ethname(int id);
char *lookup_ipname(int id);
int lookup_protoid(char *name);
int lookup_ethid(char *name);
int lookup_ipid(char *name);


#define PROTO_MIN 	0
#define PROTO_MAX	-1

/* datalink layer protocols */
#define PROTO_ETHER 1

/* network layer protocols (ethernet) */
#define ETHPROTO_IP 	0x8
#define ETHPROTO_IPV6	0xdd68
#define ETHPROTO_ARP	0x608
#define ETHPROTO_RARP	0x5308
#define	ETHPROTO_XNS	0x6
#define	ETHPROTO_APPLETALK	0xb908
#define ETHPROTO_RAW 	0

#endif

#ifndef _NETINET_IN_H

/* transport layer protocols (ip) */
/* for further informations see the RFC 1700 (http://ietf.org/rfc/rfc1700.txt) */

#define IPPROTO_IP          0	   /* Dummy protocol for TCP.                     */
#define IPPROTO_ICMP        1      /* Internet Control Message       [RFC792,JBP] */
#define IPPROTO_IGMP        2      /* Internet Group Management     [RFC1112,JBP] */
#define IPPROTO_GGP         3      /* Gateway-to-Gateway              [RFC823,MB] */
#define IPPROTO_IPIP        4      /* IP in IP (encasulation)               [JBP] */
#define IPPROTO_ST          5      /* Stream                 [RFC1190,IEN119,JWF] */
#define IPPROTO_TCP         6      /* Transmission Control           [RFC793,JBP] */
#define IPPROTO_UCL         7      /* UCL                                    [PK] */
#define IPPROTO_EGP         8      /* Exterior Gateway Protocol     [RFC888,DLM1] */
#define IPPROTO_IGP         9      /* any private interior gateway          [JBP] */
#define IPPROTO_BBNRRCMON   10     /* BBN RCC Monitoring                    [SGC] */
#define IPPROTO_NVP         11     /* Network Voice Protocol         [RFC741,SC3] */
#define IPPROTO_PUP         12     /* PUP                             [PUP,XEROX] */
#define IPPROTO_ARGUS       13     /* ARGUS                                [RWS4] */
#define IPPROTO_EMCON       14     /* EMCON                                 [BN7] */
#define IPPROTO_XNET        15     /* Cross Net Debugger            [IEN158,JFH2] */
#define IPPROTO_CHAOS       16     /* Chaos                                 [NC3] */
#define IPPROTO_UDP         17     /* User Datagram                  [RFC768,JBP] */
#define IPPROTO_MUX         18     /* Multiplexing                    [IEN90,JBP] */
#define IPPROTO_DCNMEAS	    19     /* DCN Measurement Subsystems           [DLM1] */
#define IPPROTO_HMP         20     /* Host Monitoring                [RFC869,RH6] */
#define IPPROTO_PRM         21     /* Packet Radio Measurement              [ZSU] */
#define IPPROTO_XNSIDP      22     /* XEROX NS IDP               [ETHERNET,XEROX] */
#define IPPROTO_TRUNK1      23     /* Trunk-1                              [BWB6] */
#define IPPROTO_TRUNK2      24     /* Trunk-2                              [BWB6] */
#define IPPROTO_LEAF1       25     /* Leaf-1                               [BWB6] */
#define IPPROTO_LEAF2       26     /* Leaf-2                               [BWB6] */
#define IPPROTO_RDP         27     /* Reliable Data Protocol         [RFC908,RH6] */
#define IPPROTO_IRTP        28     /* Internet Reliable Transaction  [RFC938,TXM] */
#define IPPROTO_ISOTP4	    29     /* ISO Transport Protocol Class 4 [RFC905,RC77] */
#define IPPROTO_NETBLT      30     /* Bulk Data Transfer Protocol    [RFC969,DDC1] */
#define IPPROTO_MFENSP      31     /* MFE Network Services Protocol  [MFENET,BCH2] */
#define IPPROTO_MERITINP    32     /* MERIT Internodal Protocol             [HWB] */
#define IPPROTO_SEP         33     /* Sequential Exchange Protocol        [JC120] */
#define IPPROTO_3PC         34     /* Third Party Connect Protocol         [SAF3] */
#define IPPROTO_IDPR        35     /* Inter-Domain Policy Routing Protocol [MXS1] */
#define IPPROTO_XTP         36     /* XTP                                   [GXC] */
#define IPPROTO_DDP         37     /* Datagram Delivery Protocol            [WXC] */
#define IPPROTO_IDPRCMTP    38     /* IDPR Control Message Transport Proto [MXS1] */
#define IPPROTO_TPPP        39     /* TP++ Transport Protocol               [DXF] */
#define IPPROTO_IL          40     /* IL Transport Protocol                [DXP2] */
#define IPPROTO_SIP         41     /* Simple Internet Protocol              [SXD] */
#define IPPROTO_SDRP        42     /* Source Demand Routing Protocol       [DXE1] */
#define IPPROTO_SIPSR       43     /* SIP Source Route                      [SXD] */
#define IPPROTO_SIPFRAG     44     /* SIP Fragment                          [SXD] */
#define IPPROTO_IDRP        45     /* Inter-Domain Routing Protocol   [Sue Hares] */
#define IPPROTO_RSVP        46     /* Reservation Protocol           [Bob Braden] */
#define IPPROTO_GRE         47     /* General Routing Encapsulation     [Tony Li] */
#define IPPROTO_MHRP        48     /* Mobile Host Routing Protocol[David Johnson] */
#define IPPROTO_BNA         49     /* BNA                          [Gary Salamon] */
#define IPPROTO_SIPPESSP    50     /* SIPP Encap Security Payload [Steve Deering] */
#define IPPROTO_SIPPAH      51     /* SIPP Authentication Header  [Steve Deering] */
#define IPPROTO_INLS	    52     /* Integrated Net Layer Security  TUBA [GLENN] */
#define IPPROTO_SWIPE       53     /* IP with Encryption                    [JI6] */
#define IPPROTO_NHRP        54     /* NBMA Next Hop Resolution Protocol           */
#define IPPROTO_ANYHIP      61     /* host internal protocol                [JBP] */
#define IPPROTO_CFTP        62     /* CFTP                            [CFTP,HCF2] */
#define IPPROTO_ANY         63     /* local network                         [JBP] */
#define IPPROTO_SATEXPAK    64     /* SATNET and Backroom EXPAK             [SHB] */
#define IPPROTO_KRYPTOLAN   65     /* Kryptolan                            [PXL1] */
#define IPPROTO_RVD         66     /* MIT Remote Virtual Disk Protocol      [MBG] */
#define IPPROTO_IPPC        67     /* Internet Pluribus Packet Core         [SHB] */
#define IPPROTO_ANYDFS      68     /* distributed file system               [JBP] */
#define IPPROTO_SATMON      69     /* SATNET Monitoring                     [SHB] */
#define IPPROTO_VISA        70     /* VISA Protocol                        [GXT1] */
#define IPPROTO_IPCV        71     /* Internet Packet Core Utility          [SHB] */
#define IPPROTO_CPNX        72     /* Computer Protocol Network Executive  [DXM2] */
#define IPPROTO_CPHB        73     /* Computer Protocol Heart Beat         [DXM2] */
#define IPPROTO_WSN         74     /* Wang Span Network                     [VXD] */
#define IPPROTO_PVP         75     /* Packet Video Protocol                 [SC3] */
#define IPPROTO_BRSATMON    76     /* Backroom SATNET Monitoring            [SHB] */
#define IPPROTO_SUNND       77     /* SUN ND PROTOCOL-Temporary             [WM3] */
#define IPPROTO_WBMON       78     /* WIDEBAND Monitoring                   [SHB] */
#define IPPROTO_WBEXPAK     79     /* WIDEBAND EXPAK                        [SHB] */
#define IPPROTO_ISOIP       80     /* ISO Internet Protocol                 [MTR] */
#define IPPROTO_VMTP        81     /* VMTP                                 [DRC3] */
#define IPPROTO_SECUREVMTP  82     /* SECURE-VMTP                          [DRC3] */
#define IPPROTO_VINES       83     /* VINES                                 [BXH] */
#define IPPROTO_TTP         84     /* TTP                                   [JXS] */
#define IPPROTO_NSFNETIGP   85     /* NSFNET-IGP                            [HWB] */
#define IPPROTO_DGP         86     /* Dissimilar Gateway Protocol     [DGP,ML109] */
#define IPPROTO_TCF         87     /* TCF                                  [GAL5] */
#define IPPROTO_IGRP        88     /* IGRP                            [CISCO,GXS] */
#define IPPROTO_OSPFIGP     89     /* OSPFIGP                      [RFC1583,JTM4] */
#define IPPROTO_SPRITERPC   90     /* Sprite RPC Protocol            [SPRITE,BXW] */
#define IPPROTO_LARP        91     /* Locus Address Resolution Protocol     [BXH] */
#define IPPROTO_MTP         92     /* Multicast Transport Protocol          [SXA] */
#define IPPROTO_AX25        93     /* AX.25 Frames                         [BK29] */
#define IPPROTO_IPWIP       94     /* IP-within-IP Encapsulation Protocol   [JI6] */
#define IPPROTO_MICP        95     /* Mobile Internetworking Control Pro.   [JI6] */
#define IPPROTO_SCCSP       96     /* Semaphore Communications Sec. Pro.    [HXH] */
#define IPPROTO_ETHERIP     97     /* Ethernet-within-IP Encapsulation     [RXH1] */
#define IPPROTO_ENCAP       98     /* Encapsulation Header         [RFC1241,RXB3] */
#define IPPROTO_ANYPES	    99     /* private encryption scheme         [JBP] */
#define IPPROTO_GMTP        100    /* GMTP                                 [RXB5] */
#define IPPROTO_RAW         255    /* [JBP] */

#endif

