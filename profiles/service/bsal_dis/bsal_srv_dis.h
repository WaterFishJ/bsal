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

#ifndef __BSAL_SRV_DIS_H__
#define __BSAL_SRV_DIS_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"

#define GATT_UUID_DEVICE_INFORMATION               BSAL_GATT_SERVICE_DEVICE_INFORMATION
#define GATT_UUID_CHAR_MANUFACTURER_NAME_STRING    BSAL_UUID_CHAR_MANUFACTURER_NAME_STRING
#define GATT_UUID_CHAR_MODEL_NUMBER_STRING         BSAL_UUID_CHAR_MODEL_NUMBER_STRING
#define GATT_UUID_CHAR_SERIAL_NUMBER_STRING        BSAL_UUID_CHAR_SERIAL_NUMBER_STRING
#define GATT_UUID_CHAR_HARDWARE_REVISION_STRING    BSAL_UUID_CHAR_HARDWARE_REVISION_STRING
#define GATT_UUID_CHAR_FIRMWARE_REVISION_STRING    BSAL_UUID_CHAR_FIRMWARE_REVISION_STRING
#define GATT_UUID_CHAR_SOFTWARE_REVISION_STRING    BSAL_UUID_CHAR_SOFTWARE_REVISION_STRING
#define GATT_UUID_CHAR_SYSTEM_ID                   BSAL_UUID_CHAR_SYSTEM_ID
#define GATT_UUID_CHAR_IEEE_R_C_DATA_LIST          BSAL_UUID_CHAR_IEEE_11073_20601_REGULATORY_CERTIFICATION_DATA_LIST
#define GATT_UUID_CHAR_PNP_ID                      BSAL_UUID_CHAR_PNP_ID

struct bsal_dis_data_index
{
    unsigned char manufacturer_name_string_index;
    unsigned char model_number_string_index;
    unsigned char serial_number_string_index;
    unsigned char hardware_revision_string_index;
    unsigned char firmware_revision_string_index;
    unsigned char software_revision_string_index;
    unsigned char system_id_index;
    unsigned char IEEE_R_C_data_list_index;
    unsigned char PnP_id_index;
};
typedef struct bsal_dis_data_index bsal_dis_data_index_t;

struct bsal_dis_config
{
    unsigned char *manufacturer_name_string;
    unsigned char *model_number_string;
    unsigned char *serial_number_string;
    unsigned char *hardware_revision_string;
    unsigned char *firmware_revision_string;
    unsigned char *software_revision_string;
    unsigned char *system_id;
    unsigned char *IEEE_R_C_data_list;
    unsigned char *PnP_id;
};
typedef struct bsal_dis_config bsal_dis_config_t;

/**
* Initialize the Device Infotmation Service data.
*
* @param config the pointer of the data
* @Note The data must be initialized befor register the DIS
*       profile table
*/
void bsal_dis_data_init(bsal_dis_config_t *config);

/**
* Initialize the Device Infotmation Service.
*
* @param stack_ptr  the object of the stack
* @param app_callback the callback function of the app
* @Note register the DIS table
*/
void bsal_le_dis_svr_init(void *stack_ptr, void *app_callback);



#ifdef __cplusplus
}
#endif

#endif



