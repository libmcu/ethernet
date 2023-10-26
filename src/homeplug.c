/*
 * SPDX-FileCopyrightText: 2023 Kyunghwan Kwon <k@mononn.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "eth/homeplug/hp.h"
#include <string.h>

#define HP_MMTYPE_OFFSET_BIT			2

static void set_fmi(struct hp_mme *hp)
{
	hp->fmi = 0;
	hp->fmi_opt = 0;
}

static void set_header(struct eth *eth, uint16_t mmtype)
{
	struct eth_mme *mme = (struct eth_mme *)eth->payload;
	struct hp_mme *hp = (struct hp_mme *)mme->body;

	eth_set_etype(eth, ETH_ETYPE_HOMEPLUG);

	mme_set_mver(mme, 1);
	mme_set_mmtype(mme, mmtype);

	memset(hp, 0, sizeof(*hp));

	set_fmi(hp);
}

static size_t pack_nothing(struct hp_mme *mme, const struct hp_mme_req *req)
{
	(void)mme;
	(void)req;
	return 0;
}

static size_t pack_nothing_cnf(struct hp_mme *mme, const struct hp_mme_cnf *cnf)
{
	(void)mme;
	(void)cnf;
	return 0;
}

static size_t pack_setkey_req(struct hp_mme *mme, const struct hp_mme_req *req)
{
	memcpy(mme->body, req, sizeof(req->msg.setkey));
	return sizeof(req->msg.setkey);
}

static size_t pack_slac_parm_req(struct hp_mme *mme, const struct hp_mme_req *req)
{
	memcpy(mme->body, req, sizeof(req->msg.slac_parm));
	return sizeof(req->msg.slac_parm);
}

static size_t pack_slac_parm_cnf(struct hp_mme *mme, const struct hp_mme_cnf *cnf)
{
	memcpy(mme->body, cnf, sizeof(cnf->msg.slac_parm));
	return sizeof(cnf->msg.slac_parm);
}

static size_t (*pack_req[])(struct hp_mme *mme, const struct hp_mme_req *req) = {
	pack_nothing,		/*HP_MMTYPE_DISCOVER_LIST*/
	pack_nothing,		/*HP_MMTYPE_ENCRYPTED*/
	pack_setkey_req,	/*HP_MMTYPE_SET_KEY*/
	pack_nothing,		/*HP_MMTYPE_GET_KEY*/
	pack_nothing,		/*HP_MMTYPE_BRG_INFO*/
	pack_nothing,		/*HP_MMTYPE_NW_INFO*/
	pack_nothing,		/*HP_MMTYPE_HFID*/
	pack_nothing,		/*HP_MMTYPE_NW_STATS*/
	pack_slac_parm_req,	/*HP_MMTYPE_SLAC_PARM*/
	pack_nothing,		/*HP_MMTYPE_START_ATTEN_CHAR*/
	pack_nothing,		/*HP_MMTYPE_ATTEN_CHAR*/
	pack_nothing,		/*HP_MMTYPE_PKCS_CERT*/
	pack_nothing,		/*HP_MMTYPE_MNBC_SOUND*/
	pack_nothing,		/*HP_MMTYPE_VALIDATE*/
	pack_nothing,		/*HP_MMTYPE_SLAC_MATCH*/
	pack_nothing,		/*HP_MMTYPE_SLAC_USER_DATA*/
	pack_nothing,		/*HP_MMTYPE_ATTEN_PROFILE*/
};
static size_t (*pack_cnf[])(struct hp_mme *mme, const struct hp_mme_cnf *cnf) = {
	pack_nothing_cnf,		/*HP_MMTYPE_DISCOVER_LIST*/
	pack_nothing_cnf,		/*HP_MMTYPE_ENCRYPTED*/
	pack_nothing_cnf,		/*HP_MMTYPE_SET_KEY*/
	pack_nothing_cnf,		/*HP_MMTYPE_GET_KEY*/
	pack_nothing_cnf,		/*HP_MMTYPE_BRG_INFO*/
	pack_nothing_cnf,		/*HP_MMTYPE_NW_INFO*/
	pack_nothing_cnf,		/*HP_MMTYPE_HFID*/
	pack_nothing_cnf,		/*HP_MMTYPE_NW_STATS*/
	pack_slac_parm_cnf,	/*HP_MMTYPE_SLAC_PARM*/
	pack_nothing_cnf,		/*HP_MMTYPE_START_ATTEN_CHAR*/
	pack_nothing_cnf,		/*HP_MMTYPE_ATTEN_CHAR*/
	pack_nothing_cnf,		/*HP_MMTYPE_PKCS_CERT*/
	pack_nothing_cnf,		/*HP_MMTYPE_MNBC_SOUND*/
	pack_nothing_cnf,		/*HP_MMTYPE_VALIDATE*/
	pack_nothing_cnf,		/*HP_MMTYPE_SLAC_MATCH*/
	pack_nothing_cnf,		/*HP_MMTYPE_SLAC_USER_DATA*/
	pack_nothing_cnf,		/*HP_MMTYPE_ATTEN_PROFILE*/
};
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
_Static_assert(sizeof(pack_req) / sizeof(pack_req[0]) == HP_MMTYPE_MAX, "");
_Static_assert(sizeof(pack_cnf) / sizeof(pack_cnf[0]) == HP_MMTYPE_MAX, "");
#pragma GCC diagnostic pop

size_t hp_pack_request(hp_mmtype_t type, const struct hp_mme_req *req,
		struct eth *buf, size_t bufsize)
{
	struct eth *eth = buf;
	struct eth_mme *mme = (struct eth_mme *)eth->payload;
	struct hp_mme *hp = (struct hp_mme *)mme->body;

	set_header(eth, hp_mmtype_to_code(type) | HP_MMTYPE_REQ);

	size_t len = pack_req[type](hp, req);

	if ((len + sizeof(*eth) + sizeof(*mme)) > bufsize) {
		return 0;
	}

	return len;
}

size_t hp_pack_confirm(hp_mmtype_t type, const struct hp_mme_cnf *cnf,
		struct eth *buf, size_t bufsize)
{
	struct eth *eth = buf;
	struct eth_mme *mme = (struct eth_mme *)eth->payload;
	struct hp_mme *hp = (struct hp_mme *)mme->body;

	set_header(eth, hp_mmtype_to_code(type) | HP_MMTYPE_CNF);

	size_t len = pack_cnf[type](hp, cnf);

	if ((len + sizeof(*eth) + sizeof(*mme)) > bufsize) {
		return 0;
	}

	return len;
}

hp_mmtype_t hp_mmtype(const struct eth *eth_frame)
{
	const struct eth_mme *mme = (const struct eth_mme *)eth_frame->payload;
	return hp_code_to_mmtype(mme->mmtype);
}

uint16_t hp_mmtype_to_code(hp_mmtype_t type)
{
	uint16_t base = HP_MMTYPE_STA_STA << HP_MMTYPE_MSB_BIT;
	uint16_t offset = (uint16_t)type;

	if (type >= HP_MMTYPE_MAX) {
		return 0;
	} else if (type == HP_MMTYPE_DISCOVER_LIST) {
		base = HP_MMTYPE_STA_CCO;
		offset = 0x05;
	} else if (type >= HP_MMTYPE_SLAC_PARM) {
		offset = (uint16_t)(0x19 + type - HP_MMTYPE_SLAC_PARM);
	} else if (type >= HP_MMTYPE_NW_STATS) {
		offset = (uint16_t)(0x12 + type - HP_MMTYPE_NW_STATS);
	} else if (type >= HP_MMTYPE_HFID) {
		offset = (uint16_t)(0x10 + type - HP_MMTYPE_HFID);
	} else if (type >= HP_MMTYPE_NW_INFO) {
		offset = (uint16_t)(0x0E + type - HP_MMTYPE_NW_INFO);
	} else if (type >= HP_MMTYPE_BRG_INFO) {
		offset = (uint16_t)(0x08 + type - HP_MMTYPE_BRG_INFO);
	}

	return base + (uint16_t)(offset << HP_MMTYPE_OFFSET_BIT);
}

hp_mmtype_t hp_code_to_mmtype(uint16_t code)
{
	hp_mmtype_msb_t msb = (hp_mmtype_msb_t)(code >> HP_MMTYPE_MSB_BIT);
	uint16_t offset = (uint16_t)((code >> HP_MMTYPE_OFFSET_BIT) & 0x7ff);

	switch (msb) {
	case HP_MMTYPE_STA_CCO:
		if (offset == 5) {
			return HP_MMTYPE_DISCOVER_LIST;
		}
		break;
	case HP_MMTYPE_STA_STA:
		if (offset >= 0x19) {
			offset = offset - 0x19 + HP_MMTYPE_SLAC_PARM;
		} else if (offset >= 0x12) {
			offset = offset - 0x12 + HP_MMTYPE_NW_STATS;
		} else if (offset >= 0x10) {
			offset = offset - 0x10 + HP_MMTYPE_HFID;
		} else if (offset >= 0x0E) {
			offset = offset - 0x0E + HP_MMTYPE_NW_INFO;
		} else if (offset >= 0x08) {
			offset = offset - 0x08 + HP_MMTYPE_BRG_INFO;
		}
		return (hp_mmtype_t)offset;
		break;
	case HP_MMTYPE_VENDOR:
		break;
	case HP_MMTYPE_PROXY:
		/* fall through */
	case HP_MMTYPE_CCO_CCO:
		/* fall through */
	case HP_MMTYPE_MANUFACTURE:
		/* fall through */
	default:
		break;
	}

	return HP_MMTYPE_MAX;
}
