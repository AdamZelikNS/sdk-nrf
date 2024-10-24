/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef _FP_STORAGE_PRIV_H_
#define _FP_STORAGE_PRIV_H_

#include "fp_common.h"

/**
 * @defgroup fp_storage_priv Fast Pair storage module private
 * @brief Private data of the Fast Pair storage module
 *
 * Private data structures, definitions and functionalities of the Fast Pair
 * storage module. Used only by the module and by the unit test to prepopulate
 * settings with mocked data.
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/** Settings subtree name for Fast Pair storage.  */
#define SETTINGS_AK_SUBTREE_NAME "fp"

/** Settings key name prefix for Account Key.  */
#define SETTINGS_AK_NAME_PREFIX "ak_data"

/** String used as a connector in settings keys. */
#define SETTINGS_NAME_CONNECTOR "/"

/** Full settings key name prefix for Account Key (including subtree name). */
#define SETTINGS_AK_FULL_PREFIX \
	(SETTINGS_AK_SUBTREE_NAME SETTINGS_NAME_CONNECTOR SETTINGS_AK_NAME_PREFIX)

/** Max length of suffix (key ID) in settings key name for Account Key. */
#define SETTINGS_AK_NAME_MAX_SUFFIX_LEN 1

/** Max length of settings key for Account Key. */
#define SETTINGS_AK_NAME_MAX_SIZE \
	(sizeof(SETTINGS_AK_FULL_PREFIX) + SETTINGS_AK_NAME_MAX_SUFFIX_LEN)

/** Max number of Account Keys. */
#define ACCOUNT_KEY_CNT CONFIG_BT_FAST_PAIR_STORAGE_ACCOUNT_KEY_MAX

/** Min valid Acccount Key ID. */
#define ACCOUNT_KEY_MIN_ID 1

/** Max valid Acccount Key ID. */
#define ACCOUNT_KEY_MAX_ID (2 * ACCOUNT_KEY_CNT)

BUILD_ASSERT(ACCOUNT_KEY_MAX_ID < UINT8_MAX);
BUILD_ASSERT(ACCOUNT_KEY_CNT <= 10);

/** Account Key settings record data format. */
struct account_key_data {
	/** Internal Account Key ID. */
	uint8_t account_key_id;

	/** Account Key value. */
	struct fp_account_key account_key;
};


/** Get Account Key index for given Account Key ID.
 *
 * @param[in] account_key_id	ID of an Account Key.
 *
 * @return Account Key index.
 */
static inline uint8_t account_key_id_to_idx(uint8_t account_key_id)
{
        __ASSERT_NO_MSG(account_key_id >= ACCOUNT_KEY_MIN_ID);

        return (account_key_id - ACCOUNT_KEY_MIN_ID) % ACCOUNT_KEY_CNT;
}

/** Generate next Account Key ID.
 *
 * @param[in] account_key_id		Current Account Key ID.
 *
 * @return Next Account Key ID.
 */
static inline uint8_t next_account_key_id(uint8_t account_key_id)
{
        if (account_key_id == ACCOUNT_KEY_MAX_ID) {
                return ACCOUNT_KEY_MIN_ID;
        }

        return account_key_id + 1;
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* _FP_STORAGE_PRIV_H_ */
