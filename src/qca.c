/*
 * SPDX-FileCopyrightText: 2023 Kyunghwan Kwon <k@mononn.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "eth/homeplug/qca.h"
#include <string.h>

#define QCA_MMTYPE_BASE		(HP_MMTYPE_VENDOR << HP_MMTYPE_MSB_BIT)

enum module_operation {
	MOP_READ_RAM		= 0x00,
	MOP_READ_NVM		= 0x01,
	MOP_START_WRITE_SESSION	= 0x10,
	MOP_WRITE		= 0x11,
	MOP_COMMIT		= 0x12,
};

enum module_id {
	MID_INIT		= 0x1000,
	MID_UART		= 0x2000,
	MID_ENUM_ID_TABLE	= 0x3000,
	MID_POWER_MANAGEMENT	= 0x4000,
	MID_FORWARD_CONF	= 0x7000,
	MID_FIRMWARE		= 0x7001,
	MID_PIB			= 0x7002,
	MID_SOFTLOADER		= 0x7003,
	MID_PIB_MERGE		= 0x7005,
};

static void set_oui(struct qca_mme *qca)
{
	qca->oui[0] = 0x00;
	qca->oui[1] = 0xB0;
	qca->oui[2] = 0x52;
}

static void set_header(struct eth *eth, qca_mmtype_t type)
{
	struct eth_mme *mme = (struct eth_mme *)eth->payload;
	struct qca_mme *qca = (struct qca_mme *)mme->body;

	eth_set_etype(eth, ETH_ETYPE_HOMEPLUG);

	mme_set_mver(mme, 0);
	mme_set_mmtype(mme, qca_mmtype_to_value(type));

	memset(qca, 0, sizeof(*qca));
	set_oui(qca);
}

static size_t pack_nothing(struct qca_mme *qca, const struct qca_mme_entry *entry)
{
	(void)qca;
	(void)entry;
	return 0;
}

static size_t pack_version(struct qca_mme *qca, const struct qca_mme_entry *entry)
{
	(void)qca;
	(void)entry;
	return 24;
}

static size_t (*pack[QCA_MMTYPE_MAX])(struct qca_mme *qca, const struct qca_mme_entry *mme) = {
	pack_version,		/*QCA_MMTYPE_SW_VER*/
	pack_nothing,		/*QCA_MMTYPE_WR_MEM*/
	pack_nothing,		/*QCA_MMTYPE_RD_MEM*/
	pack_nothing,		/*QCA_MMTYPE_ST_MAC*/
	pack_nothing,		/*QCA_MMTYPE_GET_NVM*/
	pack_nothing,		/*QCA_MMTYPE_RS_DEV*/
	pack_nothing,		/*QCA_MMTYPE_WR_MOD*/
	pack_nothing,		/*QCA_MMTYPE_RD_MOD*/
	pack_nothing,		/*QCA_MMTYPE_MOD_NVM*/
	pack_nothing,		/*QCA_MMTYPE_WD_RPT*/
	pack_nothing,		/*QCA_MMTYPE_LINK_STATS*/
	pack_nothing,		/*QCA_MMTYPE_NW_INFO*/
	pack_nothing,		/*QCA_MMTYPE_CP_RPT*/
	pack_nothing,		/*QCA_MMTYPE_SET_KEY*/
	pack_nothing,		/*QCA_MMTYPE_MFG_STR*/
	pack_nothing,		/*QCA_MMTYPE_RD_CBLOCK*/
	pack_nothing,		/*QCA_MMTYPE_SET_SDRAM*/
	pack_nothing,		/*QCA_MMTYPE_HST_ACTION*/
	pack_nothing,		/*QCA_MMTYPE_OP_ATTR*/
	pack_nothing,		/*QCA_MMTYPE_ETH_SET*/
	pack_nothing,		/*QCA_MMTYPE_TONE_MAP*/
	pack_nothing,		/*QCA_MMTYPE_NW_STAT*/
	pack_nothing,		/*QCA_MMTYPE_SLAVE_MEM*/
	pack_nothing,		/*QCA_MMTYPE_FAC_DEFAULT*/
	pack_nothing,		/*QCA_MMTYPE_MULTICAST_INFO*/
	pack_nothing,		/*QCA_MMTYPE_CLASSIIFCATION*/
	pack_nothing,		/*QCA_MMTYPE_RX_TONE_MAP*/
	pack_nothing,		/*QCA_MMTYPE_SET_LED*/
	pack_nothing,		/*QCA_MMTYPE_WRITE_EXC_APPLET*/
	pack_nothing,		/*QCA_MMTYPE_MDIO_CMD*/
	pack_nothing,		/*QCA_MMTYPE_SLAVE_REG*/
	pack_nothing,		/*QCA_MMTYPE_BW_LIMIT*/
	pack_nothing,		/*QCA_MMTYPE_SNID*/
	pack_nothing,		/*QCA_MMTYPE_NN_MITIGATE*/
	pack_nothing,		/*QCA_MMTYPE_MODULE*/
	pack_nothing,		/*QCA_MMTYPE_DIAG_NETWORK_PROBE*/
	pack_nothing,		/*QCA_MMTYPE_PL_LINK_STATUS*/
	pack_nothing,		/*QCA_MMTYPE_GPIO_STATE*/
	pack_nothing,		/*QCA_MMTYPE_CONN_ADD*/
	pack_nothing,		/*QCA_MMTYPE_CONN_MOD*/
	pack_nothing,		/*QCA_MMTYPE_CONN_REL*/
	pack_nothing,		/*QCA_MMTYPE_CONN_INFO*/
	pack_nothing,		/*QCA_MMTYPE_MULTIPORT_LNK_STA*/
	pack_nothing,		/*QCA_MMTYPE_EM_ID_TABLE*/
	pack_nothing,		/*QCA_MMTYPE_STANDBY*/
	pack_nothing,		/*QCA_MMTYPE_SLEEP_SCHED*/
	pack_nothing,		/*QCA_MMTYPE_SLEEP_SCHED_NOTI*/
	pack_nothing,		/*QCA_MMTYPE_MCU_DIAG*/
	pack_nothing,		/*QCA_MMTYPE_GET_PROERTY*/
	pack_nothing,		/*QCA_MMTYPE_SET_PROERTY*/
	pack_nothing,		/*QCA_MMTYPE_ATTEN*/
};

size_t qca_pack_query(qca_mmtype_t type, const struct qca_mme_entry *entry,
		struct eth *buf, size_t bufsize)
{
	(void)bufsize;
	struct eth *eth = buf;
	struct eth_mme *mme = (struct eth_mme *)eth->payload;
	struct qca_mme *qca = (struct qca_mme *)mme->body;

	set_header(eth, type);

	return pack[type](qca, entry);
}

uint16_t qca_mmtype_to_value(qca_mmtype_t type)
{
	uint16_t base = QCA_MMTYPE_BASE;
	uint16_t offset = 0;

	if (type > QCA_MMTYPE_GET_NVM) {
		offset += 4 * 2;
	}
	if (type > QCA_MMTYPE_LINK_STATS) {
		offset += 4 * 1;
	}
	if (type > QCA_MMTYPE_NW_INFO) {
		offset += 4 * 1;
	}
	if (type > QCA_MMTYPE_CP_RPT) {
		offset += 4 * 3;
	}
	if (type > QCA_MMTYPE_HST_ACTION) {
		offset += 4 * 1;
	}
	if (type > QCA_MMTYPE_FAC_DEFAULT) {
		offset += 4 * 1;
	}
	if (type > QCA_MMTYPE_CLASSIIFCATION) {
		offset += 4 * 1;
	}
	if (type > QCA_MMTYPE_MULTIPORT_LNK_STA) {
		offset += 4 * 2;
	}
	if (type > QCA_MMTYPE_SLEEP_SCHED_NOTI) {
		offset += 4 * 1;
	}
	if (type > QCA_MMTYPE_MCU_DIAG) {
		offset += 4 * 1;
	}
	if (type > QCA_MMTYPE_GET_PROERTY) {
		offset += 4 * 1;
	}
	if (type > QCA_MMTYPE_SET_PROERTY) {
		offset += 0x48;
	}

	offset += (uint16_t)(type * 4);

	return base + offset;
}

size_t qca_pack_pib_read(struct eth *buf, size_t bufsize, uint32_t offset)
{
	(void)bufsize;
	struct eth *eth = buf;
	struct eth_mme *mme = (struct eth_mme *)eth->payload;
	struct qca_mme *qca = (struct qca_mme *)mme->body;

	struct req {
		uint32_t reserved;
		uint8_t num_op_data;
		uint16_t mod_op;
		uint16_t mod_op_data_len;
		uint32_t reserved2;
		uint16_t module_id;
		uint16_t module_sub_id;
		uint16_t data_len;
		uint32_t offset;
	} __attribute__((packed)) *req = (struct req *)qca->body;

	set_header(eth, QCA_MMTYPE_MODULE);

	req->reserved = 0;
	req->num_op_data = 1;
	req->mod_op = MOP_READ_RAM;
	req->mod_op_data_len = 18;
	req->reserved2 = 0;
	req->module_id = MID_PIB;
	req->module_sub_id = 0;
	req->data_len = 1400;
	req->offset = offset;

	return 18;
}
