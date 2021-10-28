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

#ifndef __BSAL_SRV_HRS_H__
#define __BSAL_SRV_HRS_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdbool.h>
#include "bsal.h"
#define GATT_UUID_HEART_RATE                        BSAL_GATT_SERVICE_HEART_RATE
#define GATT_UUID_HRS_MEASUREMENT                   BSAL_UUID_CHAR_HEART_RATE_MEASUREMENT
#define GATT_UUID_CHAR_BODY_SENSOR_LOCATION         BSAL_UUID_CHAR_BODY_SENSOR_LOCATION
#define GATT_UUID_CHA_HEART_RATE_CONTROL_POINT      BSAL_UUID_CHAR_HEART_RATE_CONTROL_POINT

struct bsal_hrs_mdata
{
    uint8_t sensor_status;
    uint8_t energy_status;
    uint8_t rr_bit;
    uint16_t heart_rate_field;
    uint16_t energy_expended_field;
};
typedef struct bsal_hrs_mdata bsal_hrs_mdata_t;

#define BSAL_HRS_DATA_FORMATE_8BIT                  0
#define BSAL_HRS_DATA_FORMATE_16BIT                 1

#define BSAL_HRS_SENSOR_STATUS_NOTSUPPORTED         0
#define BSAL_HRS_SENSOR_STATUS_NO_CONTACT           2 << 1
#define BSAL_HRS_SENSOR_STATUS_CONTACT              3 << 1

#define BSAL_HRS_ENERGY_NOT_PRESENT                 0
#define BSAL_HRS_ENERGY_PRESENT                     1 << 3

#define BSAL_HRS_RR_NOT_PRESENT                     0
#define BSAL_HRS_RR_PRESENT                         1 << 4

struct bsal_hrs_config
{
    bsal_hrs_mdata_t *mdata;
    uint8_t *cccd;
    uint8_t *sensor_location;
    uint8_t *control_point;
};
typedef struct bsal_hrs_config bsal_hrs_config_t;

struct bsal_hrs_data_index
{
    uint8_t heart_rate_data_index;
    uint8_t heart_rate_cccd_index;
    uint8_t sensor_location_index;
    uint8_t control_point_index;
};
typedef struct bsal_hrs_data_index bsal_hrs_data_index_t;

/**
* Initialize the Heart Rate Service data.
*
* @param config the pointer of the data
* @Note The data must be initialized befor register the HRS
*       profile table
*/
void bsal_hrs_data_init(bsal_hrs_config_t *config);

/**
* Initialize the Heart Rate Service.
*
* @param stack_ptr  the object of the stack
* @param app_callback the callback function of the app
* @Note register the HRS table
*/
void bsal_le_hrs_svr_init(void *stack_ptr, void *app_callback);

/**
* Notify the data.
*
* @param stack_ptr  the object of the stack
* @param conn_id the connect id of central
* @param hr_measurement the pointer of the measurement data
*/
void bsal_hrs_send_notify_level(void *stack_ptr, uint16_t conn_id, bsal_hrs_mdata_t *hr_measurement);

#endif



