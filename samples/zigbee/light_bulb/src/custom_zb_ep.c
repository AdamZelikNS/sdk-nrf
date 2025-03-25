/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zboss_api.h>
#include <zigbee/zigbee_error_handler.h>
#include <zb_nrf_platform.h>
#include <zcl/zb_zcl_custom_cluster.h>
#include "custom_zb_ep.h"

LOG_MODULE_REGISTER(zigbee_c105p, CONFIG_ZIGBEE_SCENES_LOG_LEVEL);

struct zb_cu0_basic_attr {
	zb_uint8_t zcl_version;
	zb_uint8_t power_source;
};

struct zb_cu0_client_ctx {
	struct zb_cu0_basic_attr   basic_attr;
	zb_uint8_t                 custom0_unused;
};

static struct zb_cu0_client_ctx dev_ctx;

/* Declare attribute list for Basic cluster. */
ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST(ota_basic_attr_list,
				 &dev_ctx.basic_attr.zcl_version,
				 &dev_ctx.basic_attr.power_source);

/* Custom cluster (id 0x1A0A) attributes */
zb_uint8_t g_attr_u8 = ZB_ZCL_CUSTOM_CLUSTER_ATTR_U8_DEFAULT_VALUE;
zb_int16_t g_attr_s16 = ZB_ZCL_CUSTOM_CLUSTER_ATTR_S16_DEFAULT_VALUE;
zb_uint24_t g_attr_24bit = ZB_ZCL_CUSTOM_CLUSTER_ATTR_24BIT_DEFAULT_VALUE;
zb_uint32_t g_attr_32bitmap = ZB_ZCL_CUSTOM_CLUSTER_ATTR_32BITMAP_DEFAULT_VALUE;
zb_ieee_addr_t g_attr_ieee = ZB_ZCL_CUSTOM_CLUSTER_ATTR_IEEE_DEFAULT_VALUE;
zb_char_t g_attr_char_string[ZB_ZCL_CUSTOM_CLUSTER_ATTR_CHAR_STRING_MAX_SIZE] =
  ZB_ZCL_CUSTOM_CLUSTER_ATTR_CHAR_STRING_DEFAULT_VALUE;
zb_time_t g_attr_utc_time = ZB_ZCL_CUSTOM_CLUSTER_ATTR_UTC_TIME_DEFAULT_VALUE;
zb_uint8_t g_attr_byte_array[ZB_ZCL_CUSTOM_CLUSTER_ATTR_BYTE_ARRAY_MAX_SIZE] =
  ZB_ZCL_CUSTOM_CLUSTER_ATTR_BYTE_ARRAY_DEFAULT_VALUE;
zb_bool_t g_attr_bool = ZB_ZCL_CUSTOM_CLUSTER_ATTR_BOOL_DEFAULT_VALUE;
zb_uint8_t g_attr_128_bit_key[ZB_CCM_KEY_SIZE] = ZB_ZCL_CUSTOM_CLUSTER_ATTR_128_BIT_KEY_DEFAULT_VALUE;

ZB_ZCL_DECLARE_CUSTOM_ATTR_CLUSTER_ATTRIB_LIST_MOD(custom_attr_list,
                                                   &g_attr_u8,
                                                   &g_attr_s16,
                                                   &g_attr_24bit,
                                                   &g_attr_32bitmap,
                                                   g_attr_ieee,
                                                   g_attr_char_string,
                                                   &g_attr_utc_time,
                                                   g_attr_byte_array,
                                                   &g_attr_bool,
                                                   g_attr_128_bit_key);

zb_ret_t check_value_custom1_attr(zb_uint16_t attr_id, zb_uint8_t endpoint, zb_uint8_t *value)
{
  ZVUNUSED(attr_id); ZVUNUSED(endpoint); ZVUNUSED(value);

  return RET_OK;
}

void zb_zcl_custom_attr_init_server(void)
{
  /* For correct workflow of ZCL command processing 
   * at least cluster_check_value function should be defined 
   * in zb_zcl_add_cluster_handlers
   */
  zb_zcl_add_cluster_handlers(ZB_ZCL_CLUSTER_ID_CUSTOM,
                              ZB_ZCL_CLUSTER_SERVER_ROLE,
                              (zb_zcl_cluster_check_value_t)check_value_custom1_attr,
                              (zb_zcl_cluster_write_attr_hook_t)NULL,
                              (zb_zcl_cluster_handler_t)NULL);
}

ZB_DECLARE_CU0_CLIENT_CLUSTER_LIST(ota_upgrade_client_clusters,
	ota_basic_attr_list, custom_attr_list); 

ZB_DECLARE_CU0_CLIENT_EP(c105_client_ep,
				    ZIGBEE_CU0_ENDPOINT,
				    ota_upgrade_client_clusters);

int zigbee_c105p_init(void)
{
	/* Basic cluster attributes data. */
	dev_ctx.basic_attr.zcl_version  = ZB_ZCL_VERSION;
	dev_ctx.basic_attr.power_source = ZB_ZCL_BASIC_POWER_SOURCE_UNKNOWN;

	return 0;
}

static zb_uint8_t volatile dbg0_payload_size = 0;
static zb_uint8_t dbg0_payload_dt[64];

static void dbg0_cu0_cmd_ind(zb_bufid_t bufid)
{
    zb_uint8_t aps_payload_size = dbg0_payload_size;
	ZVUNUSED(bufid);

	LOG_INF("cu0 cmd IND called: pl-len %d", aps_payload_size);
    if (aps_payload_size > 0)
    {
        LOG_INF("aps_payload[  0..7 ] = 0x%hx, 0x%hx, 0x%hx, 0x%hx, 0x%hx, 0x%hx, 0x%hx, 0x%hx ",
                (dbg0_payload_dt[0]),
                (dbg0_payload_dt[1]),
                (dbg0_payload_dt[2]),
                (dbg0_payload_dt[3]),
                (dbg0_payload_dt[4]),
                (dbg0_payload_dt[5]),
                (dbg0_payload_dt[6]),
                (dbg0_payload_dt[7]) );
        dbg0_payload_size = 0;
    }
}

zb_uint8_t cu0_data_indicatn(zb_uint8_t param)
{
  zb_uint8_t bufid = param;
  zb_apsde_data_indication_t *data_ind = ZB_BUF_GET_PARAM(bufid, zb_apsde_data_indication_t);
  zb_bool_t processed = ZB_FALSE;

  if ( (data_ind->profileid == ZIGBEE_CU0_PROFILE) && 
       (data_ind->clusterid == ZB_ZCL_CLUSTER_ID_CUSTOM) )
  {
    zb_uint8_t aps_payload_size = 0;
    zb_uint8_t *aps_payload_ptr = zb_aps_get_aps_payload(bufid, &aps_payload_size);
    
    if (dbg0_payload_size == 0)
    {
        for (zb_uint8_t i = 0; i < aps_payload_size; ++i)
        {
          dbg0_payload_dt[i] = (aps_payload_ptr[i]);
        }

        dbg0_payload_size = aps_payload_size;
    }

    ZB_SCHEDULE_APP_CALLBACK(dbg0_cu0_cmd_ind, 0);

    processed = ZB_TRUE;
  }

  return processed;
}
