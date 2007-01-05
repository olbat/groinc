/* Copyright 2006 Sarzyniec Luc <olbat@xiato.com>
 * This software is released under GPL the license
 * see the COPYING file for more informations */

#ifndef _NETWORK_TOOLS_H
#define _NETWORK_TOOLS_H

#include "../network/my_types.h"

char *ipv4_ntoa(__u32 longip);
__u32 ipv4_aton(char *str);
__u8 *mac_aton(char *str);
char *mac_ntoa(__u8 *macaddr);
int mac_cmp(__u8 *mac1, __u8 *mac2);
int mac_null(__u8 *macaddr);

#endif

