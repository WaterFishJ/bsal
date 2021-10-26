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

#ifndef __BSAL_SRV_DIS_H__
#define __BSAL_SRV_DIS_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"

/**
*   Optional characteristics in DIS
*   How to use:
*
*   Comment the macro definition to indicate that this characteristics is not used:
*   //#define DIS_USING_SERIAL_NUMBER_STRING
*   //#define DIS_USING_SOFTWARE_REVISION_STRING
*   
*   Uncomment the macro definition to indicate that this characteristics is used:
*   #define DIS_USING_SERIAL_NUMBER_STRING
*   #define DIS_USING_SOFTWARE_REVISION_STRING
*
*/
#define DIS_USING_MANUFACTURER_NAME_STRING
#define DIS_USING_MODEL_NUMBER_STRING
#define DIS_USING_SERIAL_NUMBER_STRING
#define DIS_USING_HARDWARE_REVISION_STRING
#define DIS_USING_FIRMWARE_REVISION_STRING
#define DIS_USING_SOFTWARE_REVISION_STRING
#define DIS_USING_SYSTEM_ID
#define DIS_USING_IEEE_R_C_DATA_LIST
#define DIS_USING_PNP_ID

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

#ifdef DIS_USING_MANUFACTURER_NAME_STRING
#define MANUFACTURER_NAME_STRING_VALUE_INDEX       2
#else
#define MANUFACTURER_NAME_STRING_VALUE_INDEX       0
#endif
#ifdef DIS_USING_MODEL_NUMBER_STRING
#define MODEL_NUMBER_STRING_VALUE_INDEX            MANUFACTURER_NAME_STRING_VALUE_INDEX + 2
#else
#define MODEL_NUMBER_STRING_VALUE_INDEX            MANUFACTURER_NAME_STRING_VALUE_INDEX
#endif
#ifdef DIS_USING_SERIAL_NUMBER_STRING
#define SERIAL_NUMBER_STRING_VALUE_INDEX           MODEL_NUMBER_STRING_VALUE_INDEX + 2
#else
#define SERIAL_NUMBER_STRING_VALUE_INDEX           MODEL_NUMBER_STRING_VALUE_INDEX
#endif
#ifdef DIS_USING_HARDWARE_REVISION_STRING
#define HARDWARE_REVISION_STRING_VALUE_INDEX       SERIAL_NUMBER_STRING_VALUE_INDEX + 2
#else
#define HARDWARE_REVISION_STRING_VALUE_INDEX       SERIAL_NUMBER_STRING_VALUE_INDEX
#endif
#ifdef DIS_USING_FIRMWARE_REVISION_STRING
#define FIRMWARE_REVISION_STRING_VALUE_INDEX       HARDWARE_REVISION_STRING_VALUE_INDEX + 2
#else
#define FIRMWARE_REVISION_STRING_VALUE_INDEX       HARDWARE_REVISION_STRING_VALUE_INDEX
#endif
#ifdef DIS_USING_SOFTWARE_REVISION_STRING
#define SOFTWARE_REVISION_STRING_VALUE_INDEX       FIRMWARE_REVISION_STRING_VALUE_INDEX + 2
#else
#define SOFTWARE_REVISION_STRING_VALUE_INDEX       FIRMWARE_REVISION_STRING_VALUE_INDEX
#endif
#ifdef DIS_USING_SYSTEM_ID
#define SYSTEM_ID_VALUE_INDEX                      SOFTWARE_REVISION_STRING_VALUE_INDEX + 2
#else
#define SYSTEM_ID_VALUE_INDEX                      SOFTWARE_REVISION_STRING_VALUE_INDEX
#endif
#ifdef DIS_USING_IEEE_R_C_DATA_LIST
#define IEEE_R_C_DATA_LIST_VALUE_INDEX             SYSTEM_ID_VALUE_INDEX + 2
#else
#define IEEE_R_C_DATA_LIST_VALUE_INDEX             SYSTEM_ID_VALUE_INDEX
#endif
#ifdef DIS_USING_PNP_ID
#define PNP_ID_VALUE_INDEX                         IEEE_R_C_DATA_LIST_VALUE_INDEX + 2
#else
#define PNP_ID_VALUE_INDEX                         IEEE_R_C_DATA_LIST_VALUE_INDEX
#endif

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

void bsal_le_dis_svr_init(void *stack_ptr, void *app_callback);

void bsal_dis_data_init(bsal_dis_config_t *config);

#ifdef __cplusplus
}
#endif

#endif

