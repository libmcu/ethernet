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
#include <stddef.h>

#define HP_MMTYPE_MSB_BIT		13

typedef enum {
	HP_MMTYPE_REQ, /*< Management message request */
	HP_MMTYPE_CNF, /*< Management message confirm */
	HP_MMTYPE_IND, /*< Management message indication */
	HP_MMTYPE_RSP, /*< Management message response */
} hp_mmtype_lsb_t;

typedef enum {
	HP_MMTYPE_STA_CCO, /*< messages exchanged between STA and CCo */
	HP_MMTYPE_PROXY, /*< messages exchanged with the proxy coordinator */
	HP_MMTYPE_CCO_CCO, /*< messages exchanged between neighboring CCos */
	HP_MMTYPE_STA_STA, /*< messages exchanged between two stations */
	HP_MMTYPE_MANUFACTURE,
	HP_MMTYPE_VENDOR,
} hp_mmtype_msb_t;

typedef enum {
	HP_MMTYPE_DISCOVER_LIST,
	HP_MMTYPE_ENCRYPTED,
	HP_MMTYPE_SET_KEY,
	HP_MMTYPE_GET_KEY,
	HP_MMTYPE_BRG_INFO,
	HP_MMTYPE_NW_INFO,
	HP_MMTYPE_HFID,
	HP_MMTYPE_NW_STATS,
	HP_MMTYPE_SLAC_PARM,
	HP_MMTYPE_START_ATTEN_CHAR,
	HP_MMTYPE_ATTEN_CHAR,
	HP_MMTYPE_PKCS_CERT,
	HP_MMTYPE_MNBC_SOUND,
	HP_MMTYPE_VALIDATE,
	HP_MMTYPE_SLAC_MATCH,
	HP_MMTYPE_SLAC_USER_DATA,
	HP_MMTYPE_ATTEN_PROFILE,
	HP_MMTYPE_MAX,
} hp_mmtype_t;

struct hp_mme {
	uint8_t fmi;     /*< Fragmentation Message Sequence Number */
	uint8_t fmi_opt;
	uint8_t body[]; /*< Entry Data */
} __attribute__((packed));

struct hp_mme_setkey_req {
	uint8_t key_type;
	uint32_t my_nonce;
	uint32_t your_nonce;
	uint8_t pid;
	uint16_t prn;
	uint8_t pmn;
	uint8_t cco;
	uint8_t nid[7];
	uint8_t key_selection;
	uint8_t key[16];
} __attribute__((packed));

struct hp_mme_setkey_cnf {
	uint8_t result;
	uint32_t my_nonce;
	uint32_t your_nonce;
	uint8_t pid;
	uint16_t prn;
	uint8_t pmn;
	uint8_t cco;
} __attribute__((packed));

struct hp_mme_slac_parm_req {
	uint8_t app_type;
	uint8_t sec_type;
	uint8_t run_id[8];
	uint8_t ciphersuite_size;
	uint8_t ciphersuite[];
} __attribute__((packed));

struct hp_mme_slac_parm_cnf {
	uint8_t target_mac[6];
	uint8_t nr_sounds;
	uint8_t timeout_ms_hundredth;
	uint8_t forwarding_type;
	uint8_t forwarding_mac[6];
	uint8_t app_type;
	uint8_t sec_type;
	uint8_t run_id[8];
	uint16_t ciphersuite;
} __attribute__((packed));

struct hp_mme_req {
	union {
		struct hp_mme_setkey_req setkey;
		struct hp_mme_slac_parm_req slac_parm;
	} msg;
};

struct hp_mme_cnf {
	union {
		struct hp_mme_slac_parm_cnf slac_parm;
	} msg;
};

uint16_t hp_mmtype_to_code(hp_mmtype_t type);
hp_mmtype_t hp_code_to_mmtype(uint16_t code);
size_t hp_pack_request(hp_mmtype_t type, const struct hp_mme_req *req,
		struct eth *buf, size_t bufsize);
size_t hp_pack_confirm(hp_mmtype_t type, const struct hp_mme_cnf *cnf,
		struct eth *buf, size_t bufsize);

#if defined(__cplusplus)
}
#endif

#endif /* LIBMCU_ETH_MME_HOMEPLUG_H */
