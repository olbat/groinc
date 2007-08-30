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

#include "globals_report.h"
#include "network/my_types.h"
#include "tools/memory_tools.h"
#include <malloc.h>
#include <sys/time.h>

void default_rpt_init()
{
	default_report.countpacketstot = 0;
	default_report.countpacketsfiltred = 0;
	gettimeofday(&default_report.timestart,0);
}

__inline__ void default_free()
{
}

