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

#ifndef _PRINTP_H
#define _PRINTP_H

#define BFLAG_VAL(V,F) (((V)&(F)) != 0)
#define NBFLAG_VAL16(V,F) ((ntohs((V))&(F)) != 0)
#define NBFLAG_VAL32(V,F) (((ntohl(V))&(F)) != 0)
#define FLAG_VAL(V,F,OF) (((V)&(F)) >> OF)
#define NFLAG_VAL16(V,F,OF) (((ntohs(V))&(F)) >> OF)
#define NFLAG_VAL32(V,F,OF) (((ntohl(V))&(F)) >> OF)

void print_proto(int fd, char *format, ...);

#endif
