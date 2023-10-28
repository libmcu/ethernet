#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "eth/homeplug/hp.h"

TEST_GROUP(HomePlug) {
	void setup(void) {
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(HomePlug, mmtype_to_code_ShouldReturnMmtypeCode) {
	LONGS_EQUAL(0x0014, hp_mmtype_to_code(HP_MMTYPE_DISCOVER_LIST));
	LONGS_EQUAL(0x6004, hp_mmtype_to_code(HP_MMTYPE_ENCRYPTED));
	LONGS_EQUAL(0x6008, hp_mmtype_to_code(HP_MMTYPE_SET_KEY));
	LONGS_EQUAL(0x600C, hp_mmtype_to_code(HP_MMTYPE_GET_KEY));
	LONGS_EQUAL(0x6020, hp_mmtype_to_code(HP_MMTYPE_BRG_INFO));
	LONGS_EQUAL(0x6038, hp_mmtype_to_code(HP_MMTYPE_NW_INFO));
	LONGS_EQUAL(0x6040, hp_mmtype_to_code(HP_MMTYPE_HFID));
	LONGS_EQUAL(0x6048, hp_mmtype_to_code(HP_MMTYPE_NW_STATS));
	LONGS_EQUAL(0x6064, hp_mmtype_to_code(HP_MMTYPE_SLAC_PARM));
	LONGS_EQUAL(0x6068, hp_mmtype_to_code(HP_MMTYPE_START_ATTEN_CHAR));
	LONGS_EQUAL(0x606C, hp_mmtype_to_code(HP_MMTYPE_ATTEN_CHAR));
	LONGS_EQUAL(0x6070, hp_mmtype_to_code(HP_MMTYPE_PKCS_CERT));
	LONGS_EQUAL(0x6074, hp_mmtype_to_code(HP_MMTYPE_MNBC_SOUND));
	LONGS_EQUAL(0x6078, hp_mmtype_to_code(HP_MMTYPE_VALIDATE));
	LONGS_EQUAL(0x607C, hp_mmtype_to_code(HP_MMTYPE_SLAC_MATCH));
	LONGS_EQUAL(0x6080, hp_mmtype_to_code(HP_MMTYPE_SLAC_USER_DATA));
	LONGS_EQUAL(0x6084, hp_mmtype_to_code(HP_MMTYPE_ATTEN_PROFILE));
}

TEST(HomePlug, code_to_mmtype_ShouldReturnMmtype) {
	LONGS_EQUAL(HP_MMTYPE_DISCOVER_LIST, hp_code_to_mmtype(0x0014));
	LONGS_EQUAL(HP_MMTYPE_ENCRYPTED, hp_code_to_mmtype(0x6004));
	LONGS_EQUAL(HP_MMTYPE_SET_KEY, hp_code_to_mmtype(0x6008));
	LONGS_EQUAL(HP_MMTYPE_GET_KEY, hp_code_to_mmtype(0x600c));
	LONGS_EQUAL(HP_MMTYPE_BRG_INFO, hp_code_to_mmtype(0x6020));
	LONGS_EQUAL(HP_MMTYPE_NW_INFO, hp_code_to_mmtype(0x6038));
	LONGS_EQUAL(HP_MMTYPE_HFID, hp_code_to_mmtype(0x6040));
	LONGS_EQUAL(HP_MMTYPE_NW_STATS, hp_code_to_mmtype(0x6048));
	LONGS_EQUAL(HP_MMTYPE_SLAC_PARM, hp_code_to_mmtype(0x6064));
	LONGS_EQUAL(HP_MMTYPE_START_ATTEN_CHAR, hp_code_to_mmtype(0x6068));
	LONGS_EQUAL(HP_MMTYPE_ATTEN_CHAR, hp_code_to_mmtype(0x606C));
	LONGS_EQUAL(HP_MMTYPE_PKCS_CERT, hp_code_to_mmtype(0x6070));
	LONGS_EQUAL(HP_MMTYPE_MNBC_SOUND, hp_code_to_mmtype(0x6074));
	LONGS_EQUAL(HP_MMTYPE_VALIDATE, hp_code_to_mmtype(0x6078));
	LONGS_EQUAL(HP_MMTYPE_SLAC_MATCH, hp_code_to_mmtype(0x607C));
	LONGS_EQUAL(HP_MMTYPE_SLAC_USER_DATA, hp_code_to_mmtype(0x6080));
	LONGS_EQUAL(HP_MMTYPE_ATTEN_PROFILE, hp_code_to_mmtype(0x6084));
}

TEST(HomePlug, mmtype_variant) {
	uint8_t req[] = {
		0xff,0xff,0xff,0xff,0xff,0xff,0x00,0xe0,
		0x4c,0x36,0x07,0x91,0x88,0xe1,0x01,0x64,
		0x60,0x00,0x00,0x00,0x00,0x00,0xe0,0x4c,
		0x36,0x07,0x91,0x00,0x01,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00
	};
	uint8_t cnf[] = {
		0x00,0xe0,0x4c,0x36,0x07,0x91,0x02,0x00,
		0x04,0x69,0x00,0x00,0x88,0xe1,0x01,0x65,
		0x60,0x00,0x00,0xff,0xff,0xff,0xff,0xff,
		0xff,0x0a,0x06,0x00,0x00,0xe0,0x4c,0x36,
		0x07,0x91,0x00,0x00,0x00,0xe0,0x4c,0x36,
		0x07,0x91,0x00,0x01,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
	};
	LONGS_EQUAL(HP_MMTYPE_REQ, hp_mmtype_variant((const struct eth *)req));
	LONGS_EQUAL(HP_MMTYPE_CNF, hp_mmtype_variant((const struct eth *)cnf));
}

TEST(HomePlug, SET_KEY_REQ) {
	uint8_t expected[] = { 0x00, 0xb0, 0x52, 0x00, 0x00, 0x01, 0x02, 0x00,
		0x04, 0x69, 0x00, 0x00, 0x88, 0xe1, 0x01, 0x08,
		0x60, 0x00, 0x00, 0x01, 0xaa, 0xaa, 0xaa, 0xaa,
		0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
		0x00, 0x02, 0x6b, 0xcb, 0xa5, 0x35, 0x4e, 0x08,
		0x01, 0xb5, 0x93, 0x19, 0xd7, 0xe8, 0x15, 0x7b,
		0xa0, 0x01, 0xb0, 0x18, 0x66, 0x9c, 0xce, 0xe3,
		0x0d, 0x00, 0x00, 0x00,
	};
	uint8_t buf[64] = { 0, };
	struct hp_mme_setkey_req req = {
		.key_type = 1, /* NMK */
		.my_nonce = 0xaaaaaaaa,
		.your_nonce = 0,
		.pid = 4, /* HLE */
		.prn = 0,
		.pmn = 0,
		.cco = 0, /* station */
		.nid = { 0x02,0x6b,0xcb,0xa5,0x35,0x4e,0x08 },
		.key_selection = 1, /* nmk known to station */
		.key = { 0xb5,0x93,0x19,0xd7,0xe8,0x15,0x7b,0xa0,0x01,0xb0,0x18,0x66,0x9c,0xce,0xe3,0x0d },
	};
	size_t len = hp_pack_request(HP_MMTYPE_SET_KEY,
			(const struct hp_mme_req *)&req,
			(struct eth *)buf, sizeof(buf));
	LONGS_EQUAL(38, len);

	struct eth *eth = (struct eth *)buf;
	eth_set_dst(eth, &expected[0]);
	eth_set_src(eth, &expected[6]);

	MEMCMP_EQUAL(expected, buf, sizeof(expected));
}

TEST(HomePlug, SLAC_PARM_REQ) {
	uint8_t expected[] = {
		0xff,0xff,0xff,0xff,0xff,0xff,0x00,0xe0,
		0x4c,0x36,0x07,0x91,0x88,0xe1,0x01,0x64,
		0x60,0x00,0x00,0x00,0x00,0x00,0xe0,0x4c,
		0x36,0x07,0x91,0x00,0x01,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00
	};
	uint8_t buf[64] = { 0, };
	struct hp_mme_slac_parm_req req = {
		.app_type = 0,
		.sec_type = 0,
		.run_id = { 0x00, 0xe0, 0x4c, 0x36, 0x07, 0x91, 0x00, 0x01 },
		.ciphersuite_size = 0,
	};
	size_t len = hp_pack_request(HP_MMTYPE_SLAC_PARM,
			(const struct hp_mme_req *)&req,
			(struct eth *)buf, sizeof(buf));
	LONGS_EQUAL(11, len);

	struct eth *eth = (struct eth *)buf;
	eth_set_dst(eth, &expected[0]);
	eth_set_src(eth, &expected[6]);

	MEMCMP_EQUAL(expected, buf, sizeof(expected));
}

TEST(HomePlug, SLAC_PARM_CNF) {
	uint8_t expected[] = {
		0x00,0xe0,0x4c,0x36,0x07,0x91,0x02,0x00,
		0x04,0x69,0x00,0x00,0x88,0xe1,0x01,0x65,
		0x60,0x00,0x00,0xff,0xff,0xff,0xff,0xff,
		0xff,0x0a,0x06,0x00,0x00,0xe0,0x4c,0x36,
		0x07,0x91,0x00,0x00,0x00,0xe0,0x4c,0x36,
		0x07,0x91,0x00,0x01,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,
	};
	uint8_t buf[64] = { 0, };
	struct hp_mme_slac_parm_cnf cnf = {
		.target_mac = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
		.nr_sounds = 10,
		.timeout_ms_hundredth = 6,
		.forwarding_type = 0,
		.forwarding_mac = { 0x00, 0xe0, 0x4c, 0x36, 0x07, 0x91 },
		.app_type = 0,
		.sec_type = 0,
		.run_id = { 0x00, 0xe0, 0x4c, 0x36, 0x07, 0x91, 0x00, 0x01 },
		.ciphersuite = 0,
	};
	size_t len = hp_pack_confirm(HP_MMTYPE_SLAC_PARM,
			(const struct hp_mme_cnf *)&cnf,
			(struct eth *)buf, sizeof(buf));
	LONGS_EQUAL(27, len);

	struct eth *eth = (struct eth *)buf;
	eth_set_dst(eth, &expected[0]);
	eth_set_src(eth, &expected[6]);

	MEMCMP_EQUAL(expected, buf, sizeof(expected));
}

TEST(HomePlug, ATTEN_CHAR) {
	uint8_t expected[] = {
		0x00,0xe0,0x4c,0x36,0x07,0x91,0x02,0x00,0x04,0x69,0x00,0x00,0x88,0xe1,0x01,0x6e,
		0x60,0x00,0x00,0x00,0x00,0x00,0xe0,0x4c,0x36,0x07,0x91,0x00,0xe0,0x4c,0x36,0x07,
		0x91,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x03,0x3a,0x16,0x15,0x16,0x1a,0x17,0x18,0x1c,0x18,0x18,
		0x19,0x17,0x17,0x17,0x17,0x17,0x17,0x16,0x17,0x17,0x17,0x17,0x17,0x16,0x17,0x16,
		0x16,0x16,0x16,0x17,0x17,0x17,0x17,0x17,0x17,0x18,0x18,0x18,0x19,0x18,0x19,0x19,
		0x1a,0x19,0x19,0x19,0x1a,0x18,0x18,0x18,0x1b,0x1a,0x19,0x19,0x18,0x17,0x17,0x18,
		0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00
	};
	uint8_t buf[512] = { 0, };
	struct hp_mme_atten_char_ind ind = {
		.atten = {
			.mac_src = { 0x00, 0xe0, 0x4c, 0x36, 0x07, 0x91 },
			.run_id = { 0x00, 0xe0, 0x4c, 0x36, 0x07, 0x91, 0x00, 0x01 },
			.nr_sounds = 3,
			.nr_groups = 58,
			.aag = { 0x16,0x15,0x16,0x1a,0x17,0x18,0x1c,0x18,0x18,
				0x19,0x17,0x17,0x17,0x17,0x17,0x17,0x16,
				0x17,0x17,0x17,0x17,0x17,0x16,0x17,0x16,
				0x16,0x16,0x16,0x17,0x17,0x17,0x17,0x17,
				0x17,0x18,0x18,0x18,0x19,0x18,0x19,0x19,
				0x1a,0x19,0x19,0x19,0x1a,0x18,0x18,0x18,
				0x1b,0x1a,0x19,0x19,0x18,0x17,0x17,0x18,
				0x24,
			},
		},
	};
	size_t len = hp_pack_indication(HP_MMTYPE_ATTEN_CHAR,
			(const struct hp_mme_ind *)&ind,
			(struct eth *)buf, sizeof(buf));
	LONGS_EQUAL(110, len);

	struct eth *eth = (struct eth *)buf;
	eth_set_dst(eth, &expected[0]);
	eth_set_src(eth, &expected[6]);

	MEMCMP_EQUAL(expected, buf, sizeof(expected));
}
