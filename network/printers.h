/* This file is a part of groinc
 * Copyright 2006 Sarzyniec Luc <olbat@xiato.com>
 * This software is released under GPL the license
 * see the COPYING file for more informations */

#include "my_types.h"
#include "headers.h"

#ifndef _PRINTERS_H
#define _PRINTERS_H

/*
void print_ether(int fd, struct ethernet_header *etherh);
void print_ipv4(int fd,struct ipv4_header *iph);
void print_tcp(int fd,struct tcp_header *tcph);
void print_udp(int fd, struct udp_header *udph);
*/
void print_unknown(int fd,char *layer,char *protoname);
void print_datalink_layer(int fd, struct protocol_header *datalink_layerph);
void print_network_layer(int fd, struct protocol_header *network_layerph);
void print_transport_layer(int fd, struct protocol_header *transport_layerph);
void print_data(int fd,struct data *data);
void print_simple(int fd,struct protocol_header *network_layerph,struct protocol_header *transport_layerph);
void print_datalink_layer_proto(int fd,struct protocol_header *datalink_layerph);
void print_network_layer_proto(int fd,struct protocol_header *datalink_layerph,struct protocol_header *network_layerph);
void print_transport_layer_proto(int fd,struct protocol_header *network_layerph,struct protocol_header *transport_layerph);
void print_protoproto(int fd,struct protocol_header *datalink_layerph);
void print_ethproto(int fd,struct protocol_header *network_layerph);
void print_ipproto(int fd,struct protocol_header *transport_layerph);
void print_packetnb(int fd, int size);

#endif
