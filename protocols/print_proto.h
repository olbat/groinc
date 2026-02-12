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

#ifndef _PRINT_PROTO_H
#define _PRINT_PROTO_H

#include <stdio.h>

void print_ether(FILE *fd, char *datagram);
void print_ether_simple(FILE *fd, char *datagram);
void print_ipv4(FILE *fd, char *datagram);
void print_ipv4_simple(FILE *fd, char *datagram, __u16 sourceport, __u16 destport);
void print_arp(FILE *fd, char *datagram);
void print_icmp(FILE *fd, char *datagram);
void print_tcp(FILE *fd, char *datagram);
void print_udp(FILE *fd, char *datagram);

#endif
