/*
 * SPDX-FileCopyrightText: 2023 Kyunghwan Kwon <k@mononn.com>
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBMCU_ETH_MME_H
#define LIBMCU_ETH_MME_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "eth/eth.h"
#include <stdint.h>

struct eth_mme {
	uint8_t mmv;     /*< Management Message Version */
	uint16_t mmtype; /*< Management Message Type */
	uint8_t body[];
} __attribute__((packed));

void mme_set_mver(struct eth_mme *mme, uint8_t mmv);
void mme_set_mmtype(struct eth_mme *mme, uint16_t mmtype);

#if defined(__cplusplus)
}
#endif

#endif /* LIBMCU_ETH_MME_H */
