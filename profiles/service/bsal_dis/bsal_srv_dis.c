/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-27     WaterFishJ   the first version
 * 2021-10-27     WaterFishJ   complete all characteristics
 */

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "bsal.h"
#include "bsal_osif.h"
#include "bsal_srv_dis.h"

static bsal_dis_config_t dis_config = {NULL};
static bsal_dis_data_index_t dis_index = {0};

static P_SRV_GENERAL_CB pfn_bas_cb = NULL;

static void dis_profile_callback(void *p)
{
    bsal_callbak_data_t *p_param = (bsal_callbak_data_t *)p;
    bool is_app_cb = false;

    if (p_param->msg_type == BSAL_CALLBACK_TYPE_READ_CHAR_VALUE)
    {
        if (dis_index.manufacturer_name_string_index == p_param->off_handle)
        {
            is_app_cb = true;
            bsal_srv_write_data(p_param->stack_ptr, p_param->start_handle, p_param->off_handle,
            strlen((const char *)dis_config.manufacturer_name_string), dis_config.manufacturer_name_string);
        }
        if (dis_index.model_number_string_index == p_param->off_handle)
        {
            is_app_cb = true;
            bsal_srv_write_data(p_param->stack_ptr, p_param->start_handle, p_param->off_handle,
            strlen((const char *)dis_config.model_number_string), dis_config.model_number_string);
        }
        if (dis_index.serial_number_string_index == p_param->off_handle)
        {
            is_app_cb = true;
            bsal_srv_write_data(p_param->stack_ptr, p_param->start_handle, p_param->off_handle,
            strlen((const char *)dis_config.serial_number_string), dis_config.serial_number_string);
        }
        if (dis_index.hardware_revision_string_index == p_param->off_handle)
        {
            is_app_cb = true;
            bsal_srv_write_data(p_param->stack_ptr, p_param->start_handle, p_param->off_handle,
            strlen((const char *)dis_config.hardware_revision_string), dis_config.hardware_revision_string);
        }
        if (dis_index.firmware_revision_string_index == p_param->off_handle)
        {
            is_app_cb = true;
            bsal_srv_write_data(p_param->stack_ptr, p_param->start_handle, p_param->off_handle,
            strlen((const char *)dis_config.firmware_revision_string), dis_config.firmware_revision_string);
        }
        if (dis_index.software_revision_string_index == p_param->off_handle)
        {
            is_app_cb = true;
            bsal_srv_write_data(p_param->stack_ptr, p_param->start_handle, p_param->off_handle,
            strlen((const char *)dis_config.software_revision_string), dis_config.software_revision_string);
        }
        if (dis_index.system_id_index == p_param->off_handle)
        {
            is_app_cb = true;
            bsal_srv_write_data(p_param->stack_ptr, p_param->start_handle, p_param->off_handle,
            8, dis_config.system_id);
        }
        if (dis_index.IEEE_R_C_data_list_index == p_param->off_handle)
        {
            is_app_cb = true;
            bsal_srv_write_data(p_param->stack_ptr, p_param->start_handle, p_param->off_handle,
            strlen((const char *)dis_config.IEEE_R_C_data_list), dis_config.IEEE_R_C_data_list);
        }
        if (dis_index.PnP_id_index == p_param->off_handle)
        {
            is_app_cb = true;
            bsal_srv_write_data(p_param->stack_ptr, p_param->start_handle, p_param->off_handle,
            7, dis_config.PnP_id);
        }
    }
    if (is_app_cb && (pfn_bas_cb != NULL))
    {
        pfn_bas_cb(p_param);
    }
}

uint8_t bsal_dis_count_cha(void)
{
    uint8_t i = 0;
    int *p = (int *)&dis_config;
    for (uint8_t j = 0; j < 9; j++)
    {
        if (*(p + j) != NULL)
        {
            i++;
        }
    }
    return i;
}

void bsal_le_dis_svr_init(void *stack_ptr, void *app_callback)
{
    uint8_t cha_count = bsal_dis_count_cha();
    bsal_gatt_chr_def_t *cha = bsal_osif_malloc(sizeof(bsal_gatt_chr_def_t) * (cha_count + 1));
    bsal_uuid_any_t *cha_uuid = bsal_osif_malloc(sizeof(bsal_uuid_any_t) * cha_count);

    struct bsal_gatt_app_srv_def ble_svc_dis_defs[2] = {0};
    /*** Device Information Service. */
    ble_svc_dis_defs[0].type = BSAL_GATT_UUID_PRIMARY_SERVICE;
    ble_svc_dis_defs[0].uuid = BSAL_UUID16_DECLARE(GATT_UUID_DEVICE_INFORMATION);
        
    if (cha != NULL && cha_uuid != NULL)
    {
        uint8_t i = 0;
        uint8_t index = 0;
        
        /*** Manufacturer Name String characteristic */
        if (dis_config.manufacturer_name_string != NULL && i < cha_count)
        {
            index += 2;
            cha_uuid[i].u_type = BSAL_UUID_TYPE_16BIT;
            cha_uuid[i].u16.u_type = BSAL_UUID_TYPE_16BIT;
            cha_uuid[i].u16.value = GATT_UUID_CHAR_MANUFACTURER_NAME_STRING;
            cha[i].uuid = &cha_uuid[i];
            cha[i].properties = BSAL_ATT_P_READ;
            cha[i].permission = BSAL_GATT_PERM_READ_NONE;
            cha[i].value_length = 10;
            dis_index.manufacturer_name_string_index = index;
            i++;
        }
        /*** Model Number String characteristic */
        if (dis_config.model_number_string != NULL && i < cha_count)
        {
            index += 2;
            cha_uuid[i].u_type = BSAL_UUID_TYPE_16BIT;
            cha_uuid[i].u16.u_type = BSAL_UUID_TYPE_16BIT;
            cha_uuid[i].u16.value = GATT_UUID_CHAR_MODEL_NUMBER_STRING;
            cha[i].uuid = &cha_uuid[i];
            cha[i].properties = BSAL_ATT_P_READ;
            cha[i].permission = BSAL_GATT_PERM_READ_NONE;
            cha[i].value_length = 10;
            dis_index.model_number_string_index = index;
            i++;
        }
        /*** Serial Number String characteristic */
        if (dis_config.serial_number_string != NULL && i < cha_count)
        {
            index += 2;
            cha_uuid[i].u_type = BSAL_UUID_TYPE_16BIT;
            cha_uuid[i].u16.u_type = BSAL_UUID_TYPE_16BIT;
            cha_uuid[i].u16.value = GATT_UUID_CHAR_SERIAL_NUMBER_STRING;
            cha[i].uuid = &cha_uuid[i];
            cha[i].properties = BSAL_ATT_P_READ;
            cha[i].permission = BSAL_GATT_PERM_READ_NONE;
            cha[i].value_length = 10;
            dis_index.serial_number_string_index = index;
            i++;
        }
        /*** Hardware Revision String characteristic */
        if (dis_config.hardware_revision_string != NULL && i < cha_count)
        {
            index += 2;
            cha_uuid[i].u_type = BSAL_UUID_TYPE_16BIT;
            cha_uuid[i].u16.u_type = BSAL_UUID_TYPE_16BIT;
            cha_uuid[i].u16.value = GATT_UUID_CHAR_HARDWARE_REVISION_STRING;
            cha[i].uuid = &cha_uuid[i];
            cha[i].properties = BSAL_ATT_P_READ;
            cha[i].permission = BSAL_GATT_PERM_READ_NONE;
            cha[i].value_length = 10;
            dis_index.hardware_revision_string_index = index;
            i++;
        }
        /*** Firmware Revision String characteristic */
        if (dis_config.firmware_revision_string != NULL && i < cha_count)
        {
            index += 2;
            cha_uuid[i].u_type = BSAL_UUID_TYPE_16BIT;
            cha_uuid[i].u16.u_type = BSAL_UUID_TYPE_16BIT;
            cha_uuid[i].u16.value = GATT_UUID_CHAR_FIRMWARE_REVISION_STRING;
            cha[i].uuid = &cha_uuid[i];
            cha[i].properties = BSAL_ATT_P_READ;
            cha[i].permission = BSAL_GATT_PERM_READ_NONE;
            cha[i].value_length = 10;
            dis_index.firmware_revision_string_index = index;
            i++;
        }
        /*** Software Revision String characteristic */
        if (dis_config.software_revision_string != NULL && i < cha_count)
        {
            index += 2;
            cha_uuid[i].u_type = BSAL_UUID_TYPE_16BIT;
            cha_uuid[i].u16.u_type = BSAL_UUID_TYPE_16BIT;
            cha_uuid[i].u16.value = GATT_UUID_CHAR_SOFTWARE_REVISION_STRING;
            cha[i].uuid = &cha_uuid[i];
            cha[i].properties = BSAL_ATT_P_READ;
            cha[i].permission = BSAL_GATT_PERM_READ_NONE;
            cha[i].value_length = 10;
            dis_index.software_revision_string_index = index;
            i++;
        }
        /*** System ID characteristic */
        if (dis_config.system_id != NULL && i < cha_count)
        {
            index += 2;
            cha_uuid[i].u_type = BSAL_UUID_TYPE_16BIT;
            cha_uuid[i].u16.u_type = BSAL_UUID_TYPE_16BIT;
            cha_uuid[i].u16.value = GATT_UUID_CHAR_SYSTEM_ID;
            cha[i].uuid = &cha_uuid[i];
            cha[i].properties = BSAL_ATT_P_READ;
            cha[i].permission = BSAL_GATT_PERM_READ_NONE;
            cha[i].value_length = 8;
            dis_index.system_id_index = index;
            i++;
        }
        /*** IEEE 11073-20601 Regulatory Certification Data List characteristic */
        if (dis_config.IEEE_R_C_data_list != NULL && i < cha_count)
        {
            index += 2;
            cha_uuid[i].u_type = BSAL_UUID_TYPE_16BIT;
            cha_uuid[i].u16.u_type = BSAL_UUID_TYPE_16BIT;
            cha_uuid[i].u16.value = GATT_UUID_CHAR_IEEE_R_C_DATA_LIST;
            cha[i].uuid = &cha_uuid[i];
            cha[i].properties = BSAL_ATT_P_READ;
            cha[i].permission = BSAL_GATT_PERM_READ_NONE;
            cha[i].value_length = 10;
            dis_index.IEEE_R_C_data_list_index = index;
            i++;
        }
        /*** PnP ID characteristic */
        if (dis_config.PnP_id != NULL && i < cha_count)
        {
            index += 2;
            cha_uuid[i].u_type = BSAL_UUID_TYPE_16BIT;
            cha_uuid[i].u16.u_type = BSAL_UUID_TYPE_16BIT;
            cha_uuid[i].u16.value = GATT_UUID_CHAR_PNP_ID;
            cha[i].uuid = &cha_uuid[i];
            cha[i].properties = BSAL_ATT_P_READ;
            cha[i].permission = BSAL_GATT_PERM_READ_NONE;
            cha[i].value_length = 7;
            dis_index.PnP_id_index = index;
            i++;
        }
        
        ble_svc_dis_defs[0].characteristics = cha;
    }
    
    bsal_stack_le_srv_reg_func(stack_ptr, &ble_svc_dis_defs, (P_SRV_GENERAL_CB *)dis_profile_callback);
    pfn_bas_cb = (P_SRV_GENERAL_CB)app_callback;
    
    bsal_osif_free(cha);
    bsal_osif_free(cha_uuid);
}

void bsal_dis_data_init(bsal_dis_config_t *config)
{
    BSAL_ASSERT_PTR(config);
    memcpy(&dis_config, config, sizeof(bsal_dis_config_t));
}


