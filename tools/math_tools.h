/* Copyright 2006 Sarzyniec Luc <olbat@xiato.com>
 * This software is released under GPL the license
 * see the COPYING file for more informations */

#ifndef _MATH_TOOLS_H
#define _MATH_TOOLS_H

unsigned int my_pow(unsigned int n, unsigned int p);
char *my_itoa(unsigned int number, unsigned char base);
int my_atoi(char *str, unsigned char base);
int my_atoi_len(char *str, unsigned int len, unsigned char base);
char *my_itoa_zero(unsigned int number, unsigned char zeros,unsigned char base);

#endif

