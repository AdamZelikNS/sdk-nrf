/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef ZIGBEE_CU0_H__
#define ZIGBEE_CU0_H__

#define ZIGBEE_CU0_ENDPOINT 11
#define ZIGBEE_CU0_PROFILE  0xC105U /* Some manufacturer specific */
#define CUSTOM_CLUSTER_MANUFACTURER_CODE  0x1294U

#define ZB_DEVICE_VER_CU0_CLIENT        0
#define ZB_CU0_CLIENT_DEVICE_ID         0xfff0

/* CU0 client test input clusters number. */
#define ZB_CU0_CLIENT_IN_CLUSTER_NUM    1
/* CU0 client test output clusters number. */
#define ZB_CU0_CLIENT_OUT_CLUSTER_NUM   1


/**
 *  @brief Declare cluster list for "CU0" device.
 *  @param cluster_list_name [IN] - cluster list variable name.
 *  @param basic_attr_list [IN] - attribute list for Basic cluster.
 *  @param custom1_attr_list [OUT] - attribute list for "CU0" client
 *                                       cluster
 */
#define ZB_DECLARE_CU0_CLIENT_CLUSTER_LIST(cluster_list_name,  \
						      basic_attr_list,     \
						      custom1_attr_list)   \
		zb_zcl_cluster_desc_t cluster_list_name[] =                    \
		{                                                              \
		  ZB_ZCL_CLUSTER_DESC(                                         \
		    ZB_ZCL_CLUSTER_ID_BASIC,                                   \
		    ZB_ZCL_ARRAY_SIZE(basic_attr_list, zb_zcl_attr_t),         \
		    (basic_attr_list),                                         \
		    ZB_ZCL_CLUSTER_SERVER_ROLE,                                \
		    ZB_ZCL_MANUF_CODE_INVALID                                  \
		  ),                                                           \
  		  ZB_ZCL_CLUSTER_DESC(                                         \
            ZB_ZCL_CLUSTER_ID_CUSTOM,                                         \
            ZB_ZCL_ARRAY_SIZE(custom1_attr_list, zb_zcl_attr_t),              \
            (custom1_attr_list),                                              \
            ZB_ZCL_CLUSTER_SERVER_ROLE,                                       \
            CUSTOM_CLUSTER_MANUFACTURER_CODE                                  \
  		  )                                                                   \
		}

/**
 *  @brief Declare simple descriptor for "CU0" device.
 *  @param ep_name - endpoint variable name.
 *  @param ep_id [IN] - endpoint ID.
 *  @param in_clust_num [IN] - number of supported input clusters.
 *  @param out_clust_num [IN] - number of supported output clusters.
 *  @note in_clust_num, out_clust_num should be defined by numeric constants,
 *        not variables or any definitions, because these values are used to
 *        form simple descriptor type name.
 */
#define ZB_ZCL_DECLARE_CU0_CLIENT_SIMPLE_DESC(                         \
				ep_name, ep_id, in_clust_num, out_clust_num)   \
ZB_AF_SIMPLE_DESC_TYPE(in_clust_num, out_clust_num) simple_desc_##ep_name = {  \
		ep_id,                                                         \
		ZIGBEE_CU0_PROFILE,                                           \
		ZB_CU0_CLIENT_DEVICE_ID,                            \
		ZB_DEVICE_VER_CU0_CLIENT,                           \
		0,                                                             \
		in_clust_num,                                                  \
		out_clust_num,                                                 \
		{                                                              \
		    ZB_ZCL_CLUSTER_ID_BASIC,                                   \
		    ZB_ZCL_CLUSTER_ID_CUSTOM                                   \
		}                                                              \
	}

/**
 *  @brief Declare endpoint for "CU0" device.
 *  @param ep_name [IN] - endpoint variable name.
 *  @param ep_id [IN] - endpoint ID.
 *  @param cluster_list [IN] - endpoint cluster list.
 */
#define ZB_DECLARE_CU0_CLIENT_EP(ep_name, ep_id, cluster_list)  \
	ZB_ZCL_DECLARE_CU0_CLIENT_SIMPLE_DESC(                         \
		ep_name,                                                       \
		ep_id,                                                         \
		ZB_CU0_CLIENT_IN_CLUSTER_NUM,                       \
		ZB_CU0_CLIENT_OUT_CLUSTER_NUM);                     \
		ZB_AF_DECLARE_ENDPOINT_DESC(                                   \
			ep_name,                                               \
			ep_id,                                                 \
			ZIGBEE_CU0_PROFILE,                                    \
			0,                                                     \
			NULL,                                                  \
			ZB_ZCL_ARRAY_SIZE(                                     \
				cluster_list,                                  \
				zb_zcl_cluster_desc_t),                        \
			cluster_list,                                          \
			(zb_af_simple_desc_1_1_t *)&simple_desc_##ep_name,     \
			0, NULL, /* No reporting ctx */                        \
			0, NULL)

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_CUSTOM_CLUSTER_ATTR_U8_ID_MOD(data_ptr)           \
{                                                                                       \
  ZB_ZCL_CUSTOM_CLUSTER_ATTR_U8_ID,                                                     \
  ZB_ZCL_ATTR_TYPE_U8,                                                                  \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_MANUF_SPEC | ZB_ZCL_ATTR_ACCESS_REPORTING, \
  (CUSTOM_CLUSTER_MANUFACTURER_CODE),                                                   \
  (void*) data_ptr                                                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_CUSTOM_CLUSTER_ATTR_S16_ID_MOD(data_ptr)          \
{                                                                                       \
  ZB_ZCL_CUSTOM_CLUSTER_ATTR_S16_ID,                                                    \
  ZB_ZCL_ATTR_TYPE_S16,                                                                 \
  ZB_ZCL_ATTR_ACCESS_WRITE_ONLY | ZB_ZCL_ATTR_MANUF_SPEC | ZB_ZCL_ATTR_ACCESS_REPORTING, \
  (CUSTOM_CLUSTER_MANUFACTURER_CODE),                                                   \
  (void*) data_ptr                                                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_CUSTOM_CLUSTER_ATTR_24BIT_ID_MOD(data_ptr)        \
{                                                                                       \
  ZB_ZCL_CUSTOM_CLUSTER_ATTR_24BIT_ID,                                                  \
  ZB_ZCL_ATTR_TYPE_24BIT,                                                               \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE | ZB_ZCL_ATTR_MANUF_SPEC | ZB_ZCL_ATTR_ACCESS_REPORTING, \
  (CUSTOM_CLUSTER_MANUFACTURER_CODE),                                                   \
  (void*) data_ptr                                                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_CUSTOM_CLUSTER_ATTR_32BITMAP_ID_MOD(data_ptr)     \
{                                                                                       \
  ZB_ZCL_CUSTOM_CLUSTER_ATTR_32BITMAP_ID,                                               \
  ZB_ZCL_ATTR_TYPE_32BITMAP,                                                            \
  ZB_ZCL_ATTR_ACCESS_WRITE_OPTIONAL | ZB_ZCL_ATTR_MANUF_SPEC | ZB_ZCL_ATTR_ACCESS_REPORTING, \
  (CUSTOM_CLUSTER_MANUFACTURER_CODE),                                                   \
  (void*) data_ptr                                                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_CUSTOM_CLUSTER_ATTR_IEEE_ID_MOD(data_ptr)         \
{                                                                                       \
  ZB_ZCL_CUSTOM_CLUSTER_ATTR_IEEE_ID,                                                   \
  ZB_ZCL_ATTR_TYPE_IEEE_ADDR,                                                           \
  ZB_ZCL_ATTR_ACCESS_WRITE_OPTIONAL | ZB_ZCL_ATTR_ACCESS_WRITE_ONLY | ZB_ZCL_ATTR_MANUF_SPEC, \
  (CUSTOM_CLUSTER_MANUFACTURER_CODE),                                                   \
  (void*) data_ptr                                                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_CUSTOM_CLUSTER_ATTR_CHAR_STRING_ID_MOD(data_ptr)  \
{                                                                                       \
  ZB_ZCL_CUSTOM_CLUSTER_ATTR_CHAR_STRING_ID,                                            \
  ZB_ZCL_ATTR_TYPE_CHAR_STRING,                                                         \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_MANUF_SPEC | ZB_ZCL_ATTR_ACCESS_REPORTING, \
  (CUSTOM_CLUSTER_MANUFACTURER_CODE),                                                   \
  (void*) data_ptr                                                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_CUSTOM_CLUSTER_ATTR_UTC_TIME_ID_MOD(data_ptr)     \
{                                                                                       \
  ZB_ZCL_CUSTOM_CLUSTER_ATTR_UTC_TIME_ID,                                               \
  ZB_ZCL_ATTR_TYPE_UTC_TIME,                                                            \
  ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_MANUF_SPEC | ZB_ZCL_ATTR_ACCESS_REPORTING, \
  (CUSTOM_CLUSTER_MANUFACTURER_CODE),                                                   \
  (void*) data_ptr                                                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_CUSTOM_CLUSTER_ATTR_OCTET_STRING_ID_MOD(data_ptr) \
{                                                                                       \
  ZB_ZCL_CUSTOM_CLUSTER_ATTR_OCTET_STRING_ID,                                           \
  ZB_ZCL_ATTR_TYPE_OCTET_STRING,                                                        \
  ZB_ZCL_ATTR_ACCESS_WRITE_ONLY | ZB_ZCL_ATTR_MANUF_SPEC | ZB_ZCL_ATTR_ACCESS_REPORTING, \
  (CUSTOM_CLUSTER_MANUFACTURER_CODE),                                                   \
  (void*) data_ptr                                                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_CUSTOM_CLUSTER_ATTR_BOOL_ID_MOD(data_ptr)         \
{                                                                                       \
  ZB_ZCL_CUSTOM_CLUSTER_ATTR_BOOL_ID,                                                   \
  ZB_ZCL_ATTR_TYPE_BOOL,                                                                \
  ZB_ZCL_ATTR_ACCESS_READ_WRITE | ZB_ZCL_ATTR_MANUF_SPEC | ZB_ZCL_ATTR_ACCESS_SCENE | ZB_ZCL_ATTR_ACCESS_REPORTING, \
  (CUSTOM_CLUSTER_MANUFACTURER_CODE),                                                   \
  (void*) data_ptr                                                                      \
}

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_CUSTOM_CLUSTER_ATTR_128_BIT_KEY_ID_MOD(data_ptr)  \
{                                                                                       \
  ZB_ZCL_CUSTOM_CLUSTER_ATTR_128_BIT_KEY_ID,                                            \
  ZB_ZCL_ATTR_TYPE_128_BIT_KEY,                                                         \
  ZB_ZCL_ATTR_MANUF_SPEC,                                                               \
  (CUSTOM_CLUSTER_MANUFACTURER_CODE),                                                   \
  (void*) data_ptr                                                                      \
}

/** @internal @brief Declare attribute list for Custom Attributes cluster
*/
#define ZB_ZCL_DECLARE_CUSTOM_ATTR_CLUSTER_ATTRIB_LIST_MOD(                                     \
  attr_list, u8_attr, s16_attr, _24bit_attr, _32bitmap_attr, ieee_attr,                         \
  char_string_attr, utc_time_attr, octet_string_attr, bool_attr, _128_bit_key_attr)             \
    ZB_ZCL_START_DECLARE_ATTRIB_LIST(attr_list)                                                 \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_CUSTOM_CLUSTER_ATTR_U8_ID_MOD, (u8_attr))                       \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_CUSTOM_CLUSTER_ATTR_S16_ID_MOD, (s16_attr))                     \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_CUSTOM_CLUSTER_ATTR_24BIT_ID_MOD, (_24bit_attr))                \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_CUSTOM_CLUSTER_ATTR_32BITMAP_ID_MOD, (_32bitmap_attr))          \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_CUSTOM_CLUSTER_ATTR_IEEE_ID_MOD, (ieee_attr))                   \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_CUSTOM_CLUSTER_ATTR_CHAR_STRING_ID_MOD, (char_string_attr))     \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_CUSTOM_CLUSTER_ATTR_UTC_TIME_ID_MOD, (utc_time_attr))           \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_CUSTOM_CLUSTER_ATTR_OCTET_STRING_ID_MOD, (octet_string_attr))   \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_CUSTOM_CLUSTER_ATTR_BOOL_ID_MOD, (bool_attr))                   \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_CUSTOM_CLUSTER_ATTR_128_BIT_KEY_ID_MOD, (_128_bit_key_attr))    \
    ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

int zigbee_c105p_init(void);
zb_uint8_t cu0_data_indicatn(zb_uint8_t param);

#endif /* ZIGBEE_CU0_H__ */
