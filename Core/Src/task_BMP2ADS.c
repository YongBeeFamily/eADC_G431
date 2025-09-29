/*
 * task_BMP2ADS.c
 *
 *  Created on: Oct 7, 2024
 *      Author: cbpark
 */
#include "task_BMP2ADS.h"
#include "math.h"
#include "app_config.h"
#include "module_bmp581.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "task_Flash.h"


BMP581_DEV sensor[4];
uint8_t bmp581_status;
LOG_DATA_TYPE LOGData;

void Adc_avg_func(uint8_t ch, float value);

#define ADC_AVG_CNT	50//32
volatile float ADC_Val_avg[ADC_MAX];
volatile float ADC_r[ADC_MAX][ADC_AVG_CNT];

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;



void task_BMP2ADS(void const *argument) {
	/* USER CODE BEGIN task_BMP2ADS */
	TickType_t xLastWakeTime = xTaskGetTickCount();

	int8_t err;

	delay_init();
	bmp581_init(sensor);

	/* Infinite loop */
	for (;;) {
		bmp581_status = 0;

		for (int i = 0; i < BME581_COUNT; i++) {
			err = get_data(&sensor[i]);
			if (err < 0)
				bmp581_status |= (1 << i);

			if (err == BMP5_OK)
			{
				if (i < 2)		// PS
				{
					Adc_avg_func(ENUM_PRESS_STATIC, sensor[i].sensor_data.pressure - AppSettings.CORRECTIONPRESSUREVALUE);
					Adc_avg_func(ENUM_TEMP_STATIC, sensor[i].sensor_data.temperature);
				} else {
					Adc_avg_func(ENUM_PRESS_DIFF, sensor[i].sensor_data.pressure - AppSettings.CORRECTIONPRESSUREVALUE);
					Adc_avg_func(ENUM_TEMP_DIFF, sensor[i].sensor_data.temperature);
				}
			}
		}
		LOGData.BMP581_FATALERROR_CODE = bmp581_status;
		vTaskDelayUntil(&xLastWakeTime, 20);	// 50Hz


		LED_TOGGLE();
	}

	/* USER CODE END task_AOAAOS */
}

void Adc_avg_func(uint8_t ch, float value) {
	float adcr;

	if (ch < ADC_MAX) {
		for (uint8_t i = ADC_AVG_CNT - 1; i > 0; i--) {
			ADC_r[ch][i] = ADC_r[ch][i - 1];
		}
		ADC_r[ch][0] = value;

		adcr = 0;

		for (uint8_t i = 0; i < ADC_AVG_CNT; i++) {
			adcr += ADC_r[ch][i];
		}

		ADC_Val_avg[ch] = (float) (adcr / ADC_AVG_CNT);
	}
}
