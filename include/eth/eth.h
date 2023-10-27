/*
 * SPDX-FileCopyrightText: 2023 Kyunghwan Kwon <k@mononn.com>
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBMCU_ETH_H
#define LIBMCU_ETH_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

typedef uint16_t eth_etype_t;
enum {
	ETH_ETYPE_IPv4		= 0x0800,
	ETH_ETYPE_IPv6		= 0x86DD,
	ETH_ETYPE_HOMEPLUG	= 0x88E1,
};

struct eth {
	uint8_t dst[6];
	uint8_t src[6];
	uint16_t etype;
	uint8_t payload[];
} __attribute__((packed));

void eth_set_dst(struct eth *eth, const uint8_t dst[6]);
void eth_set_src(struct eth *eth, const uint8_t src[6]);
void eth_set_etype(struct eth *eth, uint16_t etype);
uint16_t eth_get_etype(const struct eth *eth);

#if defined(__cplusplus)
}
#endif

#endif /* LIBMCU_ETH_H */
