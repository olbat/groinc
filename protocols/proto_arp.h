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

/*
 * For further informations about this implementation please take a look to the following RFC :
 * 	RFC 826 - An Ethernet Address Resolution Protocol (http://ietf.org/rfc/rfc826.txt)
 * 	RFC 903 - A Reverse Address Resolution Protocol (http://ietf.org/rfc/rfc903.txt)
 * 	RFC 2390 - Inverse Address Resolution Protocol (http://ietf.org/rfc/rfc2390.txt)
 */

#ifndef _PROTO_ARP_H
#define _PROTO_ARP_H

#include <asm/types.h>

#include "../network/headers.h"

#define 	PROTOARP_HARD_ETHERNET					01 	/* Ethernet (10Mb) [JBP] */
#define		PROTOARP_HARD_EXPERIMENTAL_ETHERNET			02 	/* Experimental Ethernet (3Mb) [JBP] */
#define 	PROTOARP_HARD_AMATEUR_RADIO_AX25			03 	/* Amateur Radio AX.25 [PXK] */
#define 	PROTOARP_HARD_PROTEON_PRONET_TOKEN_RING			04 	/* Proteon ProNET Token Ring [Doria] */
#define 	PROTOARP_HARD_CHAOS					05 	/* Chaos [GXP] */
#define 	PROTOARP_HARD_IEEE_802_NETWORKS				06 	/* IEEE 802 Networks [JBP] */
#define 	PROTOARP_HARD_ARCNET					07 	/* ARCNET [JBP] */
#define 	PROTOARP_HARD_HYPERCHANNEL				08 	/* Hyperchannel [JBP] */
#define 	PROTOARP_HARD_LANSTAR					09 	/* Lanstar [TU] */
#define 	PROTOARP_HARD_AUTONET_SHORT_ADDRESS			10 	/* Autonet Short Address [MXB1] */
#define 	PROTOARP_HARD_LOCALTALK					11 	/* LocalTalk [JKR1] */
#define 	PROTOARP_HARD_LOCALNET					12 	/* LocalNet (IBM PCNet or SYTEK LocalNET) [JXM] */
#define 	PROTOARP_HARD_ULTRA_LINK				13 	/* Ultra link [RXD2] */
#define 	PROTOARP_HARD_SMDS					14 	/* SMDS [GXC1] */
#define 	PROTOARP_HARD_FRAME_RELAY				15 	/* Frame Relay [AGM] */
#define 	PROTOARP_HARD_ASYNCHRONOUS_TRANSMISSION_MODE		16 	/* Asynchronous Transmission Mode (ATM) [JXB2] */
#define 	PROTOARP_HARD_HDLC					17 	/* HDLC [JBP] */
#define 	PROTOARP_HARD_FIBRE_CHANNEL				18 	/* Fibre Channel [Yakov Rekhter] */
#define 	PROTOARP_HARD_ASYNCHRONOUS_TRANSMISSION_MODE_2225	19 	/* Asynchronous Transmission Mode (ATM) [RFC2225] */
#define 	PROTOARP_HARD_SERIAL_LINE				20 	/* Serial Line [JBP] */
#define 	PROTOARP_HARD_ASYNCHRONOUS_TRANSMISSION_MODE_MXB1	21 	/* Asynchronous Transmission Mode (ATM) [MXB1] */
#define 	PROTOARP_HARD_MIL_STD_188_220				22 	/* MIL-STD-188-220 [Jensen] */
#define 	PROTOARP_HARD_METRICOM					23 	/* Metricom [Stone] */
#define 	PROTOARP_HARD_IEEE_1394_1995				24 	/* IEEE 1394.1995 [Hattig] */
#define 	PROTOARP_HARD_MAPOS					25 	/* MAPOS [Maruyama] */
#define 	PROTOARP_HARD_TWINAXIAL					26 	/* Twinaxial [Pitts] */
#define 	PROTOARP_HARD_EUI_64					27 	/* EUI-64 [Fujisawa] */
#define 	PROTOARP_HARD_HIPARP					28 	/* HIPARP [JMP] */
#define 	PROTOARP_HARD_IP_AND_ARP_OVER_ISO_7816_3		29 	/* IP and ARP over ISO 7816-3 [Guthery] */
#define 	PROTOARP_HARD_ARPSEC					30 	/* ARPSec [Etienne] */
#define 	PROTOARP_HARD_IPSEC_TUNNEL				31 	/* IPsec tunnel [RFC3456] */
#define 	PROTOARP_HARD_INFINIBAND				32 	/* InfiniBand (TM) [Kashyap] */
#define 	PROTOARP_HARD_TIA_102_PROJECT_25_COMMON_AIR_INTERFACE	33 	/* TIA-102 Project 25 Common Air Interface (CAI) [Anderson] */

#define		PROTOARP_PROTO_IP	0x800	/* IPv4 */

struct arp_header
{
	__u16 hardtype;
	__u16 prototype;
	__u8 hardlen;
	__u8 protolen;
	__u16 opcode;
};

void scan_arp(struct data *datagram, struct protocol_header *network_layerph, struct protocol_header *transport_layerph);
void print_arp(int fd, char *datagram);

#endif
