/* This file is a part of groinc
 * Copyright 2006 Sarzyniec Luc <olbat@xiato.com>
 * This software is released under GPL the license
 * see the COPYING file for more informations */

#ifndef _PRINT_PROTO_H
#define _PRINT_PROTO_H

void print_ether(int fd, char *datagram);
void print_ipv4(int fd, char *datagram);
void print_ipv4_simple(int fd, char *datagram, __u16 sourceport, __u16 destport);
void print_arp(int fd, char *datagram);
void print_icmp(int fd, char *datagram);
void print_tcp(int fd, char *datagram);
void print_udp(int fd, char *datagram);

#endif
