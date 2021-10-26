/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-27     WaterFishJ   the first version
 * 2021-10-26     WaterFishJ   complete all characteristics
 */

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "bsal.h"
#include "bsal_osif.h"
#include "bsal_srv_dis.h"

bsal_dis_config_t dis_config = {NULL};

static P_SRV_GENERAL_CB pfn_bas_cb = NULL;

static void dis_profile_callback(void *p)
{
    bsal_callbak_data_t *p_param = (bsal_callbak_data_t *)p;
    bool is_app_cb = false;

    if (p_param->msg_type == BSAL_CALLBACK_TYPE_READ_CHAR_VALUE)
    {
        #ifdef DIS_USING_MANUFACTURER_NAME_STRING
        if (MANUFACTURER_NAME_STRING_VALUE_INDEX == p_param->off_handle)
        {
            bsal_srv_write_data(p_param->stack_ptr, p_param->start_handle, p_param->off_handle,
            strlen((const char *)dis_config.manufacturer_name_string), dis_config.manufacturer_name_string);
        }
        #endif
        #ifdef DIS_USING_MODEL_NUMBER_STRING
        if (MODEL_NUMBER_STRING_VALUE_INDEX == p_param->off_handle)
        {
            bsal_srv_write_data(p_param->stack_ptr, p_param->start_handle, p_param->off_handle,
            strlen((const char *)dis_config.model_number_string), dis_config.model_number_string);
        }
        #endif
        #ifdef DIS_USING_SERIAL_NUMBER_STRING
        if (SERIAL_NUMBER_STRING_VALUE_INDEX == p_param->off_handle)
        {
            bsal_srv_write_data(p_param->stack_ptr, p_param->start_handle, p_param->off_handle,
            strlen((const char *)dis_config.serial_number_string), dis_config.serial_number_string);
        }
        #endif
        #ifdef DIS_USING_HARDWARE_REVISION_STRING
        if (HARDWARE_REVISION_STRING_VALUE_INDEX == p_param->off_handle)
        {
            bsal_srv_write_data(p_param->stack_ptr, p_param->start_handle, p_param->off_handle,
            strlen((const char *)dis_config.hardware_revision_string), dis_config.hardware_revision_string);
        }
        #endif
        #ifdef DIS_USING_FIRMWARE_REVISION_STRING
        if (FIRMWARE_REVISION_STRING_VALUE_INDEX == p_param->off_handle)
        {
            bsal_srv_write_data(p_param->stack_ptr, p_param->start_handle, p_param->off_handle,
            strlen((const char *)dis_config.firmware_revision_string), dis_config.firmware_revision_string);
        }
        #endif
        #ifdef DIS_USING_SOFTWARE_REVISION_STRING
        if (SOFTWARE_REVISION_STRING_VALUE_INDEX == p_param->off_handle)
        {
            bsal_srv_write_data(p_param->stack_ptr, p_param->start_handle, p_param->off_handle,
            strlen((const char *)dis_config.software_revision_string), dis_config.software_revision_string);
        }
        #endif
        #ifdef DIS_USING_SYSTEM_ID
        if (SYSTEM_ID_VALUE_INDEX == p_param->off_handle)
        {
            bsal_srv_write_data(p_param->stack_ptr, p_param->start_handle, p_param->off_handle,
            strlen((const char *)dis_config.system_id), dis_config.system_id);
        }
        #endif
        #ifdef DIS_USING_IEEE_R_C_DATA_LIST
        if (IEEE_R_C_DATA_LIST_VALUE_INDEX == p_param->off_handle)
        {
            bsal_srv_write_data(p_param->stack_ptr, p_param->start_handle, p_param->off_handle,
            strlen((const char *)dis_config.IEEE_R_C_data_list), dis_config.IEEE_R_C_data_list);
        }
        #endif
        #ifdef DIS_USING_PNP_ID
        if (PNP_ID_VALUE_INDEX == p_param->off_handle)
        {
            bsal_srv_write_data(p_param->stack_ptr, p_param->start_handle, p_param->off_handle,
            strlen((const char *)dis_config.PnP_id), dis_config.PnP_id);
        }
        #endif
        is_app_cb = true;
    }
    if (is_app_cb && (pfn_bas_cb != NULL))
    {
        pfn_bas_cb(p_param);
    }
}

void bsal_le_dis_svr_init(void *stack_ptr, void *app_callback)
{
    int i;
    struct bsal_gatt_app_srv_def ble_svc_dis_defs[] =
    {
        {
            /*** Device Information Service. */
            .type = BSAL_GATT_UUID_PRIMARY_SERVICE,
            .uuid = BSAL_UUID16_DECLARE(GATT_UUID_DEVICE_INFORMATION),
            .characteristics = (bsal_gatt_chr_def_t[])
            {
                #ifdef DIS_USING_MANUFACTURER_NAME_STRING
                {
                    /*** Manufacturer Name String characteristic */
                    .uuid = BSAL_UUID16_DECLARE(GATT_UUID_CHAR_MANUFACTURER_NAME_STRING),
                    .properties = BSAL_ATT_P_READ,
                    .permission = BSAL_GATT_PERM_READ_NONE,
                    .value_length = 10,
                },
                #endif
                #ifdef DIS_USING_MODEL_NUMBER_STRING
                {
                    /*** Model Number String characteristic */
                    .uuid = BSAL_UUID16_DECLARE(GATT_UUID_CHAR_MODEL_NUMBER_STRING),
                    .properties = BSAL_ATT_P_READ,
                    .permission = BSAL_GATT_PERM_READ_NONE,
                    .value_length = 10,
                },
                #endif
                #ifdef DIS_USING_SERIAL_NUMBER_STRING
                {
                    /*** Serial Number String characteristic */
                    .uuid = BSAL_UUID16_DECLARE(GATT_UUID_CHAR_SERIAL_NUMBER_STRING),
                    .properties = BSAL_ATT_P_READ,
                    .permission = BSAL_GATT_PERM_READ_NONE,
                    .value_length = 10,
                },
                #endif
                #ifdef DIS_USING_HARDWARE_REVISION_STRING
                {
                    /*** Hardware Revision String characteristic */
                    .uuid = BSAL_UUID16_DECLARE(GATT_UUID_CHAR_HARDWARE_REVISION_STRING),
                    .properties = BSAL_ATT_P_READ,
                    .permission = BSAL_GATT_PERM_READ_NONE,
                    .value_length = 10,
                },
                #endif
                #ifdef DIS_USING_FIRMWARE_REVISION_STRING
                {
                    /*** Firmware Revision String characteristic */
                    .uuid = BSAL_UUID16_DECLARE(GATT_UUID_CHAR_FIRMWARE_REVISION_STRING),
                    .properties = BSAL_ATT_P_READ,
                    .permission = BSAL_GATT_PERM_READ_NONE,
                    .value_length = 10,
                },
                #endif
                #ifdef DIS_USING_SOFTWARE_REVISION_STRING
                {
                    /*** Software Revision String characteristic */
                    .uuid = BSAL_UUID16_DECLARE(GATT_UUID_CHAR_SOFTWARE_REVISION_STRING),
                    .properties = BSAL_ATT_P_READ,
                    .permission = BSAL_GATT_PERM_READ_NONE,
                    .value_length = 10,
                },
                #endif
                #ifdef DIS_USING_SYSTEM_ID
                {
                    /*** System ID characteristic */
                    .uuid = BSAL_UUID16_DECLARE(GATT_UUID_CHAR_SYSTEM_ID),
                    .properties = BSAL_ATT_P_READ,
                    .permission = BSAL_GATT_PERM_READ_NONE,
                    .value_length = 10,
                },
                #endif
                #ifdef DIS_USING_IEEE_R_C_DATA_LIST
                {
                    /*** IEEE 11073-20601 Regulatory Certification Data List characteristic */
                    .uuid = BSAL_UUID16_DECLARE(GATT_UUID_CHAR_IEEE_R_C_DATA_LIST),
                    .properties = BSAL_ATT_P_READ,
                    .permission = BSAL_GATT_PERM_READ_NONE,
                    .value_length = 10,
                },
                #endif
                #ifdef DIS_USING_PNP_ID
                {
                    /*** PnP ID characteristic */
                    .uuid = BSAL_UUID16_DECLARE(GATT_UUID_CHAR_PNP_ID),
                    .properties = BSAL_ATT_P_READ,
                    .permission = BSAL_GATT_PERM_READ_NONE,
                    .value_length = 7,
                },
                #endif
                {
                    0, /* No more characteristics in this service. */
                }
            },
        },
        {
            0, /* No more services. */
        },
    };
    bsal_stack_le_srv_reg_func(stack_ptr, &ble_svc_dis_defs, (P_SRV_GENERAL_CB *)dis_profile_callback);
    pfn_bas_cb = (P_SRV_GENERAL_CB)app_callback;
}

void bsal_dis_data_init(bsal_dis_config_t *config)
{
    BSAL_ASSERT_PTR(config);
    memcpy(&dis_config, config, sizeof(bsal_dis_config_t));
}
