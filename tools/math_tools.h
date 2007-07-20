/*
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

#ifndef _MATH_TOOLS_H
#define _MATH_TOOLS_H

unsigned int my_pow(unsigned int n, unsigned int p);
int my_atoi(char *str, unsigned char base);
int my_atoi_len(char *str, unsigned int len, unsigned char base);
unsigned int my_itoa_buffer_size(unsigned int number, unsigned char base);
char *my_itoa(unsigned int number, unsigned char base, char *buff);
unsigned int my_itoa_char_char_size(unsigned int number, unsigned char nbc,unsigned char base);
char *my_itoa_char(unsigned int number, unsigned char nbc,unsigned char base, char c, char *buff);

#endif

