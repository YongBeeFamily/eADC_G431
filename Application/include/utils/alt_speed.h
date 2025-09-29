/*
 * alt_speed.h
 *
 *  Created on: Aug 5, 2024
 *      Author: kjkim
 */

#ifndef INCLUDE_UTILS_ALT_SPEED_H_
#define INCLUDE_UTILS_ALT_SPEED_H_

#include "main.h"


void KalmanInit();
float getVSpeedKalman(float P_meas, float deltaTime);

#endif /* INCLUDE_UTILS_ALT_SPEED_H_ */
