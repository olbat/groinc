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

#include "namescache.h"
#include "../tools/network_tools.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <malloc.h>
#include <string.h>

__u16 ncacheopt = 0;

static struct ncache_ipv4 *ipv4c = 0;
static int ipv4cptr = 0;
static struct sockaddr_in ipv4namesain;

#define NCHE_IPV4_INIT() __extension__ \
({ \
	struct ncache_ipv4 *p; \
	ipv4c = (struct ncache_ipv4 *) \
		malloc(NCHE_IPV4_SIZE*sizeof(struct ncache_ipv4)); \
	p = ipv4c + NCHE_IPV4_SIZE; \
	while (p-- > ipv4c) \
		p->name = 0; \
	ipv4namesain.sin_family = AF_INET; \
	ipv4namesain.sin_port = 0; \
})

#define NCHE_IPV4_FREE() __extension__ \
({ \
	struct ncache_ipv4 *p; \
	p = ipv4c + NCHE_IPV4_SIZE; \
	while (p-- > ipv4c) \
		if (p->name) \
			free(p->name); \
	ipv4cptr = 0; \
	free(ipv4c); \
	ipv4c = 0; \
})

char *
get_ipv4_name(__u32 addr)
{
	static char tmpip[IPV4_STR_MAXSIZE];
	/* to prevent freeing of a name between two call of the function */
	static char *last;
	register struct ncache_ipv4 *ptr;
	struct hostent *tmph;

	if (!ipv4c)
	{	
		NCHE_IPV4_INIT();
		ptr = ipv4c;
		ipv4cptr = 1;
	}
	else
	{
		ptr = ipv4c;
		while(ptr < (ipv4c + ipv4cptr))
		{
			if (ptr->addr == addr)
				goto out;
			ptr++;
		}
		if ((ipv4c + (NCHE_IPV4_SIZE - 1))->name)
		{
			ptr = ipv4c + (NCHE_IPV4_SIZE - 1);
			while (ptr >= (ipv4c + ipv4cptr)) 
			{
				if (ptr->addr == addr)
					goto out;
				ptr--;
			}
		}
#define NCHE_IPV4_INCPTR() __extension__ \
({ \
	(ipv4c + (ipv4cptr >= (NCHE_IPV4_SIZE - 1) \
		? (ipv4cptr = 0) : ipv4cptr++)); \
})

		/* the addr was not found in the cache */
		ptr = NCHE_IPV4_INCPTR();
		while (ptr->name == last)
			ptr = NCHE_IPV4_INCPTR();
	}

#define NCHE_IPV4_NCPY(V,S) __extension__ \
({ \
	V = (char *) malloc(strlen(S)*sizeof(char)+1); \
	V = strcpy(V,S); \
})

	ptr->addr = addr;
	
	if (ptr->name)
		free(ptr->name);

	if (ncacheopt & NCHE_OPT_NORESOLV)
	{
		NCHE_IPV4_NCPY(ptr->name,ipv4_ntoa(ntohl(addr),tmpip));
	}
	else
	{
		ipv4namesain.sin_addr.s_addr = addr;

		/* if (getnameinfo(&ipv4namesain,sizeof(ipv4namesain),tmpname,
			sizeof(tmpname),0,0,0)) */

		if ((tmph = gethostbyaddr(&addr,sizeof(__u32),AF_INET)))
			NCHE_IPV4_NCPY(ptr->name,tmph->h_name);
		else
			NCHE_IPV4_NCPY(ptr->name,ipv4_ntoa(ntohl(addr),tmpip));
	}

	out:
		return (last = ptr->name);
}

__inline__ void ncache_free()
{
	if (ipv4c)
		NCHE_IPV4_FREE();
}
