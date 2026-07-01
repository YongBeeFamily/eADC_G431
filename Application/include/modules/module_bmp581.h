/*
 * module_bmp581.h
 *
 *  Created on: Oct 31, 2023
 *      Author: kjkim
 */

#ifndef INCLUDE_MODULES_MODULE_BMP581_H_
#define INCLUDE_MODULES_MODULE_BMP581_H_

#include "main.h"

#include "bmp5.h"
#include "bmp5_defs.h"
#include "common.h"


#define BME581_COUNT		4


void bmp581_init(BMP581_DEV *sensor);



int8_t get_data(BMP581_DEV *sensor);


#endif /* INCLUDE_MODULES_MODULE_BMP581_H_ */
