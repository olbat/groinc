/* This file is a part of groinc
 * 
 * Copyright (C) 2006-2008 Sarzyniec Luc <olbat@xiato.net>
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

#ifndef _FILTER_PROTO_H
#define _FILTER_PROTO_H

#include <asm/types.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "../globals_filter.h"
#include "../network/headers.h"

#define PRT_FLT_OK 0
#define PRT_FLT_ERROR -1

enum proto_filter_id 
{
	PROTOFLT_ETHER = 0,
	PROTOFLT_ARP,
	PROTOFLT_IP,
	PROTOFLT_ICMP,
	PROTOFLT_UDP,
	PROTOFLT_TCP,
	_PROTOFLT_MAX
};

struct proto_filter
{
	enum proto_filter_id id;
	char *name;
	int (*func_check)(char *filtername, char *val);
	int (*func_parse)(char *filtername, char *val, __u8 *buff);
	void (*func_free)(__u8 *);
	int (*func_flt)(struct protocol_header *,struct protocol_header *, 
		struct protocol_header *,struct data *, __u8 *);
};

enum proto_filter_parse_type
{
	PRF_TYPE_BITFIELD,
	PRF_TYPE_NORMAL
};

struct proto_filter_parse
{
	char *name;
	enum proto_filter_parse_type type;
	int offset;
	int bfoffset;
	int size;
};

#define PRT_LKD_FLT_ADD(V,S,FV,FR) \
	linked_list_add(list_filter, \
			linked_list_flt_value_init(FV, \
						   FR, \
						   (__u8 *)V, \
						   S));
#define PRT_FLT_PARSE(T) \
	struct proto_filter_parse *ptr; \
	/* >>> TODO: better implementation of argument treatment */ \
	long int tval; \
	ptr = T; \
	while (*ptr->name && strcmp(ptr->name,filtername)) \
		ptr++; \
	if (*ptr->name) \
	{ \
		if (ptr->type == PRF_TYPE_NORMAL) \
		{ \
			if (ptr->size < sizeof(tval)) \
				tval = (htonl(strtol(val,0,16))  \
					>> ((sizeof(tval) - ptr->size)) * 8); \
			else \
				tval = htonl(strtol(val,0,16)); \
			PRT_FLT_VAL_NORMAL(buff,tval,ptr->offset,ptr->size); \
			return (ptr->size + 2); \
		} \
		else if (ptr->type == PRF_TYPE_BITFIELD) \
		{ \
			tval = strtol(val,0,16); \
			PRT_FLT_VAL_BITFIELD(buff,tval,ptr->offset, \
				ptr->bfoffset,ptr->size); \
			return (2 + (ptr->size > 1 ? 1 : 0)); \
		} \
	} \
	return 0;

/* Specific filter values :
 * -1 byte : 
 * 	1 bit: type (bitfield or normal)
 * 	7 bits: offset
 *   -bitfield : 1 byte
 * 	4 bits : offset (from the lsb) of the specified bits 
 * 	3 bits : size of the specified bits
 * 	1 bit : quick value, if size <= 1
 *   -normal : 1byte
 *  	8 bits : size
 * -normal && bitfield which size > 1
 *  	n byte(s) : value
 */

/* Size in bytes */
#define PRT_FLT_VAL_NORMAL(P,V,OF,S) \
	*P = (__u8) (OF & 0x7F); \
	*(P + 1) = (__u8) S; \
	memcpy((P + 2),&V,(sizeof(V) > S ? S : sizeof(V)));

#define PRT_FLT_OFFSET(P) ((P) & 0x7F)
#define PRT_FLT_IS_NORMAL(V) !((V) & 0x80) 
#define PRT_FLT_VAL_HEADER(P,H) (((__u8 *)(H)) + PRT_FLT_OFFSET(*P))
#define PRT_FLT_NORMAL_CMP(P,H) __extension__ \
({ \
	int I; \
	__u8 *HV; \
	HV = PRT_FLT_VAL_HEADER(P,H); \
	I = *(P + 1) - 1; \
	while ((I) && (*(P + I + 2) == *(HV + I))) \
		I--; \
	(I || (*(P + I + 2) != *(HV + I))); \
})	

#define PRT_FLT_BITFIELD_OFFSET(P) ((*(P + 1) & 0xF0) >> 4)
#define PRT_FLT_BITFIELD_SIZE(P) ((*(P + 1) >> 1) & 0x07)
/* /!\ Size is in bits */
#define PRT_FLT_VAL_BITFIELD(P,V,OF,OFB,S) \
	*P = (__u8) (OF | 0x80); \
	*(P + 1) = (__u8) (OFB << 4); \
	*(P + 1) |= (__u8) ((S << 1) & 0x0E); \
	if (S <= 1) \
		*(P + 1) |= (__u8) ((V) & 0x01); \
	else \
		*(P + 2) = (__u8) ((V) << (OFB)); 

#define PRT_FLT_BITFIELD_BITMASK(P,V) __extension__ \
({ \
	(((((V) >> PRT_FLT_BITFIELD_OFFSET(P) \
		) << (8 - PRT_FLT_BITFIELD_SIZE(P)))) & 0xFF) \
		>> (8 - (PRT_FLT_BITFIELD_SIZE(P) + PRT_FLT_BITFIELD_OFFSET(P))); \
})
	
#define PRT_FLT_BITFIELD_CMP(P,H) __extension__ \
({ \
	int R; \
	__u8 *HV; \
	HV = PRT_FLT_VAL_HEADER(P,H); \
	if (PRT_FLT_BITFIELD_SIZE(P) > 1) \
		R = (*HV & PRT_FLT_BITFIELD_BITMASK(P,0xFF)) != *(P + 2); \
	else \
		R = ((*HV >> PRT_FLT_BITFIELD_OFFSET(P)) & 0x01) \
			!= (*(P + 1) & 0x01); \
	R; \
})

#define PRT_FLT_CMP(H,V) __extension__ \
({ \
	if (PRT_FLT_IS_NORMAL(*V)) \
	{ \
		return !PRT_FLT_NORMAL_CMP(V,H); \
	} \
	else \
	{ \
		return !PRT_FLT_BITFIELD_CMP(V,H); \
	} \
})


__inline__ int proto_filter(char *protofilterstr, char *val);
int check_filter(char *filtername, char *val);

#endif
