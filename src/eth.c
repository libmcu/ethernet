/*
 * SPDX-FileCopyrightText: 2023 Kyunghwan Kwon <k@mononn.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "eth/eth.h"
#include <string.h>
#if defined(__NEWLIB__)
#include <sys/endian.h>
#define htons		htobe16
#else
#include <arpa/inet.h>
#endif

void eth_set_dst(struct eth *eth, uint8_t dst[6])
{
	memcpy(eth->dst, dst, sizeof(eth->dst));
}

void eth_set_src(struct eth *eth, uint8_t src[6])
{
	memcpy(eth->src, src, sizeof(eth->src));
}

void eth_set_etype(struct eth *eth, eth_etype_t etype)
{
	eth->etype = htons(etype);
}

uint16_t eth_get_etype(struct eth *eth)
{
	return htons(eth->etype);
}
