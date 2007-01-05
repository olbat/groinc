/* This file is a part of groinc
 * Copyright 2006 Sarzyniec Luc <olbat@xiato.com>
 * This software is released under GPL the license
 * see the COPYING file for more informations */

#ifndef _PACKET_INOUT_H
#define _PACKET_INOUT_H

int read_packet(int fd,char *buffer, int size);
/* The format of the output file : a short integer for the size of the packet data then, the packet data */
int write_packet(int fd,char *buffer,short size);

#endif

