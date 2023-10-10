/*
 * SPDX-FileCopyrightText: 2023 Kyunghwan Kwon <k@mononn.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "eth/mme.h"
#include <string.h>

void mme_set_mver(struct eth_mme *mme, uint8_t mmv)
{
	mme->mmv = mmv;
}

void mme_set_mmtype(struct eth_mme *mme, uint16_t mmtype)
{
	mme->mmtype = mmtype;
}
