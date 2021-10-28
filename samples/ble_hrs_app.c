/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-10     WaterFishJ   the first version
 * 2021-10-28     WaterFishJ   add dis
 */


#include "bsal.h"
#include <stdio.h>
#include <string.h>
#include "bsal_osif.h"
#include "bsal_srv_hrs.h"
#include "bsal_srv_dis.h"

#define BSAL_STACK_NAME PKG_BSAL_STACK_NAME

static void *bsal_stack_ptr = NULL;
static uint16_t bsal_app_conn_handle;
static rt_uint8_t gap_conn_state = BSAL_GAP_CONN_STATE_CONNECTED;

uint8_t manufacturer[] = "rt_thread";
uint8_t model[] = "pca10056";
bsal_dis_config_t dis_cfg = 
{
    .manufacturer_name_string = manufacturer,
    .model_number_string = model,
};

bsal_hrs_mdata_t hrs_mdata = {0};
uint8_t heart_rate_flag = 0;
uint8_t sensor_location = 1;
uint8_t control_point = 0;
bsal_hrs_config_t hrs_cfg =
{
    .mdata = &hrs_mdata,
    .cccd = &heart_rate_flag,
    .sensor_location = &sensor_location,
    .control_point = &control_point,
};

static void bsa_app_set_adv_data(void *stack_ptr)
{
    uint8_t tmp_data[32] = {0} ; //must be zero
    bsal_le_adv_data_add_flag(tmp_data, BSAL_GAP_ADTYPE_FLAGS_LIMITED | BSAL_GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED);

    char *adv_name = (char *)bsal_get_device_name(stack_ptr);
    bsal_adv_data_add_name(tmp_data, strlen(adv_name), adv_name);
    //bsal_adv_data_add_uuid16(tmp_data, BSAL_GATT_SERVICE_BATTERY_SERVICE);
    bsal_set_le_adv_data_user(stack_ptr, tmp_data);
}

static void bsal_app_all_callback(void *stack_ptr, uint8_t cb_layer, uint16_t cb_sub_event, uint8_t value_length, void *value)
{
    T_BSAL_GAP_MSG_DATA  *bsal_gap_msg_data = (T_BSAL_GAP_MSG_DATA *)value;
    switch (cb_layer)
    {
    case BSAL_CB_LAYER_GAP:
        switch (cb_sub_event)
        {
        case BSAL_CB_STACK_READY:
            //get mac address

            bsal_osif_printf_info("============stack ready===========\r\n");
            bsa_app_set_adv_data(stack_ptr);
            bsal_stack_start_adv(stack_ptr);
            break;
        case BSAL_CB_CONNECT_STATUS:
            bsal_osif_printf_info("============stack connect id %d===========\r\n", bsal_gap_msg_data->gap_conn_state_change.conn_id);
            if (bsal_gap_msg_data->gap_conn_state_change.new_state == BSAL_GAP_CONN_STATE_CONNECTED)
            {
                bsal_app_conn_handle = bsal_gap_msg_data->gap_conn_state_change.conn_id;
            }
            else if (bsal_gap_msg_data->gap_conn_state_change.new_state == BSAL_GAP_CONN_STATE_DISCONNECTED)
            {
                bsal_stack_start_adv(stack_ptr);
                heart_rate_flag = 0;
            }
            bsal_osif_printf_info("BSAL: conn_id %d old_state %d new_state %d, disc_cause 0x%x",
                                  bsal_gap_msg_data->gap_conn_state_change.conn_id, gap_conn_state, bsal_gap_msg_data->gap_conn_state_change.new_state, bsal_gap_msg_data->gap_conn_state_change.disc_cause);

            break;
        default:
            break;
        }

        if (cb_sub_event == BSAL_CB_STACK_READY)
        {
            //stack ready
        }

        break;
    case BSAL_CB_LAYER_GATT_PROFILE:
        switch (cb_sub_event)
        {
            //save the service start_handle
            //case uuid profile save start_handle
            //case SRV_CALLBACK66
            //save the identity
        }
        break;
    case BSAL_CB_LAYER_SM:
        break;
    case BSAL_CB_LAYER_COMMON:
        //connected save the connect id

        break;
    case BSAL_CB_LAYER_UNKNOWN:
        break;
    default:
        break;
    }

}

static void bsal_app_profile_callback(void *p)
{
    bsal_callbak_data_t *bsal_param = (bsal_callbak_data_t *)p;

    if (bsal_param->msg_type == BSAL_CALLBACK_TYPE_READ_CHAR_VALUE)
    {
        bsal_osif_printf_info("========callback read from %x====%x=======\r\n", bsal_param->off_handle, bsal_param->srv_uuid.u16.value);
    }
    else if (bsal_param->msg_type == BSAL_CALLBACK_TYPE_INDIFICATION_NOTIFICATION)
    {
        uint16_t  cccbits = bsal_param->value;
        bsal_osif_printf_info("======callback notify from %x===data cccd %x====%x=====\r\n", bsal_param->off_handle, cccbits, bsal_param->srv_uuid.u16.value);
        if (bsal_param->srv_uuid.u16.value == GATT_UUID_HEART_RATE)//heart_rate_uuid
        {
            if (cccbits & BSAL_GATT_CCC_NOTIFY)
            {
                bsal_osif_printf_info("=========NOTIFY ENABLE from %x===data cccd %x====%x=====\r\n", bsal_param->off_handle, cccbits, bsal_param->srv_uuid.u16.value);
            }
            else
            {
                bsal_osif_printf_info("========NOTIFY DISABLE from %x===data cccd %x====%x=====\r\n", bsal_param->off_handle, cccbits, bsal_param->srv_uuid.u16.value);
            }
        }
    }
    else if (bsal_param->msg_type == BSAL_CALLBACK_TYPE_WRITE_CHAR_VALUE)
    {
        bsal_osif_printf_info("\r\n BSAL: THE DATA IS :%s\r\n", bsal_param->data);
    }
}

static void bsal_ble_loop(void *p_param)
{
    uint16_t heart_rate = 90;
    while (1)
    {
        bsal_osif_delay(1000);
        bsal_osif_printf_info("====hello world===%d=\r\n", heart_rate_flag);
        
        hrs_mdata.energy_status = BSAL_HRS_ENERGY_PRESENT;
        hrs_mdata.energy_expended_field += 10;
        if (control_point == 0)
        {
            bsal_osif_printf_info("====clear energy expended====\n");
            control_point = 1;
            hrs_mdata.energy_expended_field = 0;
        }
        if (heart_rate_flag == 1)
        {
            if (heart_rate <= 120)
            {
                heart_rate++;
            }
            else
            {
                heart_rate = 90;
            }
            hrs_mdata.sensor_status = BSAL_HRS_SENSOR_STATUS_CONTACT;
            hrs_mdata.heart_rate_field = heart_rate;
            bsal_hrs_send_notify_level(bsal_stack_ptr, bsal_app_conn_handle, &hrs_mdata);
        }
    }
}

int bsal_hrs_app(void)
{
    void *stack_ptr = bsal_find_stack_ptr(BSAL_STACK_NAME);
    if (stack_ptr == NULL)
    {
        //print error;
        return 1;
    }
    //set iocapability

    bsal_stack_ptr  = stack_ptr;
    //1. init stack
    bsal_stack_init(stack_ptr, bsal_app_all_callback);  // init param not start stack
    // set device name
    char *device_name = "ble_rtt_hrs";
    bsal_set_device_name(stack_ptr, strlen(device_name), (uint8_t *)device_name);
    //2. bond type
    bsal_set_device_le_bond_type(stack_ptr, false, BSAL_NO_INPUT, BSAL_NO_OUTPUT, BSAL_GAP_AUTHEN_BIT_NO_BONDING, false);
    //set the bond flag:
    //3. service begin
    bsal_stack_le_srv_begin(stack_ptr, 2, bsal_app_profile_callback);  //will add 2 service
    //4. hrs_init
    bsal_hrs_data_init(&hrs_cfg);
    bsal_le_hrs_svr_init(stack_ptr, bsal_app_profile_callback); //add heart rate servcie
    //5. dis_init
    bsal_dis_data_init(&dis_cfg);
    bsal_le_dis_svr_init(stack_ptr, bsal_app_profile_callback); //add device information servcie

    //5. srv_end
    bsal_stack_le_srv_end(stack_ptr);    //end srv add

    //start stack
    bsal_stack_startup(stack_ptr);    //start she

    bsal_ble_loop(stack_ptr);

    return 0;
}
MSH_CMD_EXPORT_ALIAS(bsal_hrs_app, bsal_hrs_app, "bluetoooth heart rate sample");

