/*
 * SPDX-FileCopyrightText: 2023 Kyunghwan Kwon <k@mononn.com>
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LIBMCU_ETH_MME_QUALCOMM_H
#define LIBMCU_ETH_MME_QUALCOMM_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "eth/homeplug/hp.h"
#include <stdint.h>
#include <stddef.h>

typedef enum {
	QCA_MMTYPE_SW_VER,
	QCA_MMTYPE_WR_MEM,
	QCA_MMTYPE_RD_MEM,
	QCA_MMTYPE_ST_MAC,
	QCA_MMTYPE_GET_NVM,
	QCA_MMTYPE_RS_DEV,
	QCA_MMTYPE_WR_MOD,
	QCA_MMTYPE_RD_MOD,
	QCA_MMTYPE_MOD_NVM,
	QCA_MMTYPE_WD_RPT,
	QCA_MMTYPE_LINK_STATS,
	QCA_MMTYPE_NW_INFO,
	QCA_MMTYPE_CP_RPT,
	QCA_MMTYPE_SET_KEY,
	QCA_MMTYPE_MFG_STR,
	QCA_MMTYPE_RD_CBLOCK,
	QCA_MMTYPE_SET_SDRAM,
	QCA_MMTYPE_HST_ACTION,
	QCA_MMTYPE_OP_ATTR,
	QCA_MMTYPE_ETH_SET,
	QCA_MMTYPE_TONE_MAP,
	QCA_MMTYPE_NW_STAT,
	QCA_MMTYPE_SLAVE_MEM,
	QCA_MMTYPE_FAC_DEFAULT,
	QCA_MMTYPE_MULTICAST_INFO,
	QCA_MMTYPE_CLASSIIFCATION,
	QCA_MMTYPE_RX_TONE_MAP,
	QCA_MMTYPE_SET_LED,
	QCA_MMTYPE_WRITE_EXC_APPLET,
	QCA_MMTYPE_MDIO_CMD,
	QCA_MMTYPE_SLAVE_REG,
	QCA_MMTYPE_BW_LIMIT,
	QCA_MMTYPE_SNID,
	QCA_MMTYPE_NN_MITIGATE,
	QCA_MMTYPE_MODULE,
	QCA_MMTYPE_DIAG_NETWORK_PROBE,
	QCA_MMTYPE_PL_LINK_STATUS,
	QCA_MMTYPE_GPIO_STATE,
	QCA_MMTYPE_CONN_ADD,
	QCA_MMTYPE_CONN_MOD,
	QCA_MMTYPE_CONN_REL,
	QCA_MMTYPE_CONN_INFO,
	QCA_MMTYPE_MULTIPORT_LNK_STA,
	QCA_MMTYPE_EM_ID_TABLE,
	QCA_MMTYPE_STANDBY,
	QCA_MMTYPE_SLEEP_SCHED,
	QCA_MMTYPE_SLEEP_SCHED_NOTI,
	QCA_MMTYPE_MCU_DIAG,
	QCA_MMTYPE_GET_PROERTY,
	QCA_MMTYPE_SET_PROERTY,
	QCA_MMTYPE_ATTEN,
	QCA_MMTYPE_MAX,
} qca_mmtype_t;

struct qca_mme {
	uint8_t oui[3]; /*< Qualcomm OUI: 0x00, 0xB0, 0x52 */
	uint8_t body[];
} __attribute__((packed));

struct qca_mme_ver_cnf {
	uint8_t status; /*< 0x00 on success */
	uint8_t device_class;
	uint8_t verlen;
	uint8_t verstr[253];
	uint8_t reserved;
	uint32_t ic_id;
	uint32_t ic_rev;
	uint32_t chip_serial;
	uint32_t chip_package;
	uint32_t chip_option;
} __attribute__((packed));

struct qca_mme_mo_cnf {
	uint16_t status;
	uint16_t err_recovery_code;
	uint32_t reserved;
	uint8_t num_op_data;
	uint8_t data[];
} __attribute__((packed));

struct qca_mme_entry {
	int reserved;
};

size_t qca_pack_query(qca_mmtype_t type, const struct qca_mme_entry *mme,
		struct eth *buf, size_t bufsize);
uint16_t qca_mmtype_to_value(qca_mmtype_t type);

size_t qca_pack_pib_read(struct eth *buf, size_t bufsize, uint32_t offset);

#if defined(__cplusplus)
}
#endif

#endif /* LIBMCU_ETH_MME_QUALCOMM_H */
