/*
 * task_Flash.h
 *
 *  Created on: Aug 6, 2025
 *      Author: tocru
 */

#ifndef INC_TASK_FLASH_H_
#define INC_TASK_FLASH_H_

#include "main.h"


#define USER_DATA_ADDR  0x0801F800  // 마지막 2KB 시작 주소

#define DEFAULT_CORRECTIONPRESSUREVALUE		75.0f

#define APP_SETTINGS_MAGIC 0xA5A55A5A
 
typedef struct {
    uint32_t signature; // should be APP_SETTINGS_MAGIC when valid
	uint8_t SERIALNO[32];
	float CORRECTIONPRESSUREVALUE;
	float CORRECTIONPRESSUREVALUE2;
} APP_SETTINGS;


extern APP_SETTINGS AppSettings;


#endif /* INC_TASK_FLASH_H_ */
