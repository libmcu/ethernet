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
} hp_mmtype_variant_t;

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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
	uint8_t ciphersuite[0];
#pragma GCC diagnostic pop
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

struct hp_mme_start_atten_char_ind_body {
	uint8_t nr_sounds;
	uint8_t timeout_ms_hundredth;
	uint8_t resp_type;
	uint8_t forwarding_sta[6];
	uint8_t run_id[8];
} __attribute__((packed));

struct hp_mme_start_atten_char_ind {
	uint8_t app_type;
	uint8_t sec_type;
	struct hp_mme_start_atten_char_ind_body atten;
} __attribute__((packed));

struct hp_mme_mnbc_sound_ind_body {
	uint8_t sender_id[17];
	uint8_t count;
	uint8_t run_id[8];
	uint8_t reserved[8];
	uint8_t rnd[16];
} __attribute__((packed));

struct hp_mme_mnbc_sound_ind {
	uint8_t app_type;
	uint8_t sec_type;
	struct hp_mme_mnbc_sound_ind_body msound;
} __attribute__((packed));

struct hp_mme_atten_profile_ind {
	uint8_t pev_mac[6];
	uint8_t nr_groups;
	uint8_t reserved;
	uint8_t aag[];
} __attribute__((packed));

struct hp_mme_atten_char_ind_body {
	uint8_t mac_src[6];
	uint8_t run_id[8];
	uint8_t id_src[17];
	uint8_t id_rsp[17];
	uint8_t nr_sounds;
	uint8_t nr_groups;
	uint8_t aag[58];
} __attribute__((packed));

struct hp_mme_atten_char_ind {
	uint8_t app_type;
	uint8_t sec_type;
	struct hp_mme_atten_char_ind_body atten;
} __attribute__((packed));

struct hp_mme_atten_char_rsp_body {
	uint8_t mac_src[6];
	uint8_t run_id[8];
	uint8_t id_src[17];
	uint8_t id_rsp[17];
	uint8_t result;
} __attribute__((packed));

struct hp_mme_atten_char_rsp {
	uint8_t app_type;
	uint8_t sec_type;
	struct hp_mme_atten_char_rsp_body atten;
} __attribute__((packed));

struct hp_mme_validate_req {
	uint8_t signal_type;
	uint8_t vfield[];
} __attribute__((packed));

struct hp_mme_validate_cnf {
	uint8_t signal_type;
	uint8_t vfield[];
} __attribute__((packed));

struct hp_mme_slac_match_req_body {
	uint8_t id_pev[17];
	uint8_t mac_pev[6];
	uint8_t id_evse[17];
	uint8_t mac_evse[6];
	uint8_t run_id[8];
	uint8_t reserved[8];
} __attribute__((packed));

struct hp_mme_slac_match_req {
	uint8_t app_type;
	uint8_t sec_type;
	uint16_t len;
	struct hp_mme_slac_match_req_body mfield;
} __attribute__((packed));

struct hp_mme_slac_match_cnf_body {
	uint8_t id_pev[17];
	uint8_t mac_pev[6];
	uint8_t id_evse[17];
	uint8_t mac_evse[6];
	uint8_t run_id[8];
	uint8_t reserved[8];
	uint8_t nid[7];
	uint8_t reserved2;
	uint8_t nmk[16];
} __attribute__((packed));

struct hp_mme_slac_match_cnf {
	uint8_t app_type;
	uint8_t sec_type;
	uint16_t len;
	struct hp_mme_slac_match_cnf_body mfield;
} __attribute__((packed));

struct hp_mme_pkcs_cert_req {
	uint8_t target_mac[6];
	uint8_t ciphersuite_size;
	uint8_t ciphersuite[];
} __attribute__((packed));

struct hp_mme_pkcs_cert_cnf {
	uint8_t target_mac[6];
	uint8_t status;
	uint16_t ciphersuite;
	uint16_t cert_len;
	uint8_t cert[];
} __attribute__((packed));

struct hp_mme_pkcs_cert_ind {
	uint8_t target_mac[6];
	uint16_t ciphersuite;
	uint16_t cert_len;
	uint8_t cert[];
} __attribute__((packed));

struct hp_mme_pkcs_cert_rsp {
	uint8_t target_mac[6];
	uint8_t status;
	uint16_t ciphersuite_size;
	uint8_t ciphersuite[];
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
		struct hp_mme_slac_match_cnf slac_match;
	} msg;
};

struct hp_mme_ind {
	union {
		struct hp_mme_atten_char_ind atten_char;
	} msg;
};

uint16_t hp_mmtype_to_code(hp_mmtype_t type);
hp_mmtype_t hp_code_to_mmtype(uint16_t code);

size_t hp_pack_request(hp_mmtype_t type, const struct hp_mme_req *req,
		struct eth *buf, size_t bufsize);
size_t hp_pack_confirm(hp_mmtype_t type, const struct hp_mme_cnf *cnf,
		struct eth *buf, size_t bufsize);
size_t hp_pack_indication(hp_mmtype_t type, const struct hp_mme_ind *ind,
		struct eth *buf, size_t bufsize);

hp_mmtype_t hp_mmtype(const struct eth *eth_frame);
hp_mmtype_variant_t hp_mmtype_variant(const struct eth *eth_frame);

#if defined(__cplusplus)
}
#endif

#endif /* LIBMCU_ETH_MME_HOMEPLUG_H */
