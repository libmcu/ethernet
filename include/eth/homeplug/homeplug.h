/*
 * SPDX-FileCopyrightText: 2023 Kyunghwan Kwon <k@mononn.com>
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBMCU_ETH_MME_HOMEPLUG_H
#define LIBMCU_ETH_MME_HOMEPLUG_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "eth/mme.h"
#include <stdint.h>

#define HP_MMTYPE_MSB_BIT		13

typedef enum {
	HP_MMTYPE_REQ, /*< Management message request */
	HP_MMTYPE_CNF, /*< Management message confirm */
	HP_MMTYPE_IND, /*< Management message indication */
	HP_MMTYPE_RSP, /*< Management message response */
} homeplug_mmtype_lsb_t;

typedef enum {
	HP_MMTYPE_STA_CCO, /*< messages exchanged between STA and CCo */
	HP_MMTYPE_PROXY, /*< messages exchanged with the proxy coordinator */
	HP_MMTYPE_CCO_CCO, /*< messages exchanged between neighboring CCos */
	HP_MMTYPE_STA_STA, /*< messages exchanged between two stations */
	HP_MMTYPE_MANUFACTURE,
	HP_MMTYPE_VENDOR,
} homeplug_mmtype_msb_t;

struct homeplug_mme {
	uint8_t fmi;     /*< Fragmentation Message Sequence Number */
	uint8_t fmi_opt;
	uint8_t body[]; /*< Entry Data */
} __attribute__((packed));

#if defined(__cplusplus)
}
#endif

#endif /* LIBMCU_ETH_MME_HOMEPLUG_H */
