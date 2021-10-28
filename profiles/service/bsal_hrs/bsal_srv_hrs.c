/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-10     WaterFishJ   the first version
 * 2021-10-28     WaterFishJ   complete all characteristics
 */

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "bsal.h"
#include "bsal_osif.h"
#include "bsal_srv_hrs.h"

bsal_hrs_config_t hrs_config = {NULL};
bsal_hrs_data_index_t hrs_index = {0};

static P_SRV_GENERAL_CB pfn_bas_cb = NULL;
static void hrs_profile_callback(void *p)
{
    bsal_callbak_data_t *p_param = (bsal_callbak_data_t *)p;
    bool is_app_cb = false;
    
    if (p_param->msg_type == BSAL_CALLBACK_TYPE_READ_CHAR_VALUE)
    {
        if (hrs_index.heart_rate_cccd_index == p_param->off_handle)
        {
            is_app_cb = true;
            bsal_srv_write_data(p_param->stack_ptr, p_param->start_handle, p_param->off_handle,
            1, hrs_config.cccd);
        }
        if (hrs_index.sensor_location_index == p_param->off_handle)
        {
            is_app_cb = true;
            bsal_srv_write_data(p_param->stack_ptr, p_param->start_handle, p_param->off_handle,
            1, hrs_config.sensor_location);
        }
    }
    else if (p_param->msg_type == BSAL_CALLBACK_TYPE_WRITE_CHAR_VALUE)
    {
        if (hrs_index.control_point_index == p_param->off_handle)
        {
            is_app_cb = true;
            *hrs_config.control_point = (uint8_t)p_param->value;
        }
    }
    else if (p_param->msg_type == BSAL_CALLBACK_TYPE_INDIFICATION_NOTIFICATION)
    {
        if (hrs_index.heart_rate_cccd_index == p_param->off_handle)
        {
            if (p_param->length == 2)
            {
                is_app_cb = true;
                *hrs_config.cccd = p_param->value & BSAL_GATT_CCC_NOTIFY;
            }
        }
    }
    if (is_app_cb && (pfn_bas_cb != NULL))
    {
        pfn_bas_cb(p_param);
    }
}

uint8_t bsal_hrs_count_cha(void)
{
    uint8_t i = 1;
    bsal_hrs_config_t *p = &hrs_config;
    if (p->sensor_location != NULL)
    {
        i++;
    }
    if (p->control_point != NULL)
    {
        i++;
    }
    return i;
}

void bsal_le_hrs_svr_init(void *stack_ptr, void *app_callback)
{
    uint8_t cha_count = bsal_hrs_count_cha();
    bsal_gatt_chr_def_t *cha = bsal_osif_malloc(sizeof(bsal_gatt_chr_def_t) * (cha_count + 1));
    bsal_uuid_any_t *cha_uuid = bsal_osif_malloc(sizeof(bsal_uuid_any_t) * cha_count);
    
    struct bsal_gatt_app_srv_def ble_svc_hrs_defs[2] = {0};
    /*** Heart Rate Service. */
    ble_svc_hrs_defs[0].type = BSAL_GATT_UUID_PRIMARY_SERVICE;
    ble_svc_hrs_defs[0].uuid = BSAL_UUID16_DECLARE(GATT_UUID_HEART_RATE);
    
    if (cha != NULL && cha_uuid != NULL)
    {
        uint8_t i = 1;
        uint8_t index = 2;
        
        /*** Heart Rate Measurement characteristic */
        cha_uuid[0].u_type = BSAL_UUID_TYPE_16BIT;
        cha_uuid[0].u16.u_type = BSAL_UUID_TYPE_16BIT;
        cha_uuid[0].u16.value = GATT_UUID_HRS_MEASUREMENT;
        cha[0].uuid = &cha_uuid[0];
        cha[0].properties = BSAL_ATT_P_NOTIFY;
        cha[0].permission = BSAL_GATT_PERM_READ_NONE;
        cha[0].value_length = 5;
        hrs_index.heart_rate_data_index = index;
        
        index++;
        hrs_index.heart_rate_cccd_index = index;
        /*** Body Sensor Location characteristic */
        if (hrs_config.sensor_location != NULL && i < cha_count)
        {
            index += 2;
            cha_uuid[i].u_type = BSAL_UUID_TYPE_16BIT;
            cha_uuid[i].u16.u_type = BSAL_UUID_TYPE_16BIT;
            cha_uuid[i].u16.value = GATT_UUID_CHAR_BODY_SENSOR_LOCATION;
            cha[i].uuid = &cha_uuid[i];
            cha[i].properties = BSAL_ATT_P_READ;
            cha[i].permission = BSAL_GATT_PERM_READ_NONE;
            cha[i].value_length = 1;
            hrs_index.sensor_location_index = index;
            i++;
        }
        /*** Heart Rate Control Point characteristic*/
        if (hrs_config.control_point != NULL && i < cha_count)
        {
            index += 2;
            cha_uuid[i].u_type = BSAL_UUID_TYPE_16BIT;
            cha_uuid[i].u16.u_type = BSAL_UUID_TYPE_16BIT;
            cha_uuid[i].u16.value = GATT_UUID_CHA_HEART_RATE_CONTROL_POINT;
            cha[i].uuid = &cha_uuid[i];
            cha[i].properties = BSAL_ATT_P_WRITE;
            cha[i].permission = BSAL_GATT_PERM_READ_NONE;
            cha[i].value_length = 1;
            hrs_index.control_point_index = index;
            i++;
        }
        ble_svc_hrs_defs[0].characteristics = cha;
    }
    
    bsal_stack_le_srv_reg_func(stack_ptr, &ble_svc_hrs_defs, (P_SRV_GENERAL_CB *)hrs_profile_callback);
    pfn_bas_cb = (P_SRV_GENERAL_CB)app_callback;
    
    bsal_osif_free(cha);
    bsal_osif_free(cha_uuid);
}

void bsal_hrs_send_notify_level(void *stack_ptr, uint16_t conn_id,  bsal_hrs_mdata_t *hr_measurement)
{
    bsal_uuid_any_t uuid_srv;
    uuid_srv.u_type = 16;
    uuid_srv.u16.value = GATT_UUID_HEART_RATE;
    uint16_t start_handle = bsal_srv_get_start_handle(stack_ptr, uuid_srv);
    
    uint8_t data[5] = {0};
    uint8_t length = 0;
    /*** Heart rate field */
    if (hr_measurement->heart_rate_field > 0xff)
    {
        data[length++] = hr_measurement->rr_bit | hr_measurement->energy_status | hr_measurement->sensor_status | 1;
        data[length++] = 0xff & hr_measurement->heart_rate_field;
        data[length++] = 0xff & (hr_measurement->heart_rate_field >> 8);
    }
    else
    {
        data[length++] = hr_measurement->rr_bit | hr_measurement->energy_status | hr_measurement->sensor_status;
        data[length++] = hr_measurement->heart_rate_field;
    }
    /*** Energy expended field */
    if (hr_measurement->energy_status)
    {
        data[length++] = 0xff & hr_measurement->energy_expended_field;
        data[length++] = 0xff & (hr_measurement->energy_expended_field >> 8);
    }
    /*TODO: RR-Interval field*/
    
    bsal_srv_send_notify_data(stack_ptr, conn_id, start_handle, hrs_index.heart_rate_data_index, length, data);
}

void bsal_hrs_data_init(bsal_hrs_config_t *config)
{
    BSAL_ASSERT_PTR(config);
    memcpy(&hrs_config, config, sizeof(bsal_hrs_config_t));
}

