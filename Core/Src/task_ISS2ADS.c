/*
 * task_GCS2ADS.c
 *
 *  Created on: Dec 11, 2023
 *      Author: kjkim
 */
#include "task_ISS2ADS.h"
#include "app_config.h"
#include <math.h>
#include "cmsis_os.h"

#include "module_bmp581.h"
#include "alt_speed.h"

extern UART_HandleTypeDef huart1;
static uint8_t ADS2OFP_Buff[256];

extern volatile float ADC_Val_avg[ADC_MAX];


extern BMP581_DEV sensor[ADC_MAX];

ICD_eADC2OFP_TYPE		ICD_eADC2OFP;

extern uint8_t bmp581_status;
uint32_t operationTime;

#define TemperatureComrrection

// 온도보정 : 약 -7m 오차 보정
static double CAL_C0 = 0.0;      // 상수 편향 (m) — 초기엔 0
static double CAL_C1 = 0.7;     // 온도 계수 (m / °C) -> 관측치 기반
const double T_REF = 24.0;       // 기준 온도 (°C)
// ℎ_𝑓𝑙𝑡 상수값
#define PSL_EXPONENT			0.190255
#define HFLT_COEF				(4.4308 * powf(10,4))
#define HFLT_PSL				101325.0	// Pa
// Altitude : ℎ_𝑓𝑙𝑡=4.4308 × 10^4  [ 1 −  (𝑃_𝑓𝑙𝑡/𝑃_𝑆𝐿 )^0.190255 ]
static double Calib_Volt2PS(float psRaw)
{
	double altitude;
	double psDiff = (psRaw / HFLT_PSL);
	double psPowf = powf(psDiff, PSL_EXPONENT);

	altitude = HFLT_COEF * fabs(1 - psPowf);

// 센서 온도 보정 ************************************
	if (altitude < 5000.0)
	{
		altitude *= 1.000568;
		altitude = ((5000 - altitude) * 0.0002) + altitude;
	} else {
		altitude *= 1.00033;
	}


#ifdef TemperatureComrrection
    double dT = (double)ADC_Val_avg[ENUM_TEMP_STATIC] - T_REF;
    double err = CAL_C0 + CAL_C1 * dT;   // 예측 오차 (m)
    double altitude_corr = altitude - err;

    return altitude_corr;
#else
    return altitude;
#endif
}




// 𝑉_𝐼𝐴𝑆 상수값
#define STANDARD_AIR_DENSITY	101.325	// kPa
// Velocity : 𝑉_𝐼𝐴𝑆=√((2 ∆𝑃)/𝜌_0 )
static float Calib_Volt2PT(float psRaw, float ptRaw)
{
	double velo, rootValue;
	double ptpsDiff = ptRaw - psRaw;
	ptpsDiff *= 1000.0;	// Pa -> kPa

	rootValue = fabs((2*ptpsDiff) / STANDARD_AIR_DENSITY);
	velo = sqrt(rootValue);

// 센서 온도 보정 ***************************************
	if (velo < 390.0)
	{
		velo *= pow(1.013779, 2);
		velo += (390.0 - velo) * 0.0028;

		return velo;
	} else {
		velo *= pow(1.008169, 2);

#ifdef TemperatureComrrection
    double dT = (double)ADC_Val_avg[ENUM_TEMP_STATIC] - T_REF;
    double err = CAL_C0 + CAL_C1 * dT;   // 예측 오차 (m)
    double speed_corr = velo - err;

    return speed_corr;
#else
    return velo;
#endif
	}


}





#define AltSpeed_MAX		12
float 	ALTPRESS_r[AltSpeed_MAX];
float 	ALTSPEED_r[AltSpeed_MAX];

float VerticalPress_avg_func(float value) {
	float adcr;

	for (uint8_t i = AltSpeed_MAX - 1; i > 0; i--) {
		ALTPRESS_r[i] = ALTPRESS_r[i - 1];
	}
	ALTPRESS_r[0] = value;

	adcr = 0;

	for (uint8_t i = 0; i < AltSpeed_MAX; i++) {
		adcr += ALTPRESS_r[i];
	}

	return (float)(adcr / AltSpeed_MAX);
}
float VerticalSpeed_avg_func(float value) {
	float adcr;

	for (uint8_t i = AltSpeed_MAX - 1; i > 0; i--) {
		ALTSPEED_r[i] = ALTSPEED_r[i - 1];
	}
	ALTSPEED_r[0] = value;

	adcr = 0;

	for (uint8_t i = 0; i < AltSpeed_MAX; i++) {
		adcr += ALTSPEED_r[i];
	}

	return (float)(adcr / AltSpeed_MAX);
}


float preAlt, verticalSpeed;
uint32_t preTime, deltaTime;


void ADS2OFP_DATA(void)
{
  uint8_t loop = 0;
  ICD_eADC2OFP.HEADER1 = HEADER_ADS2OFP0;
  ICD_eADC2OFP.HEADER2 = HEADER_ADS2OFP1;

  ICD_eADC2OFP.CMD_Counter++;

  ICD_eADC2OFP.BMP581_STATUS = bmp581_status;

  ICD_eADC2OFP.AirPosVel.Altitude						= VerticalPress_avg_func((float)(Calib_Volt2PS(ADC_Val_avg[ENUM_PRESS_STATIC])));
  ICD_eADC2OFP.AirPosVel.Velocity						= (float)(Calib_Volt2PT(ADC_Val_avg[ENUM_PRESS_STATIC], ADC_Val_avg[ENUM_PRESS_DIFF]));

/**************************** Vertical Speed Calculate Start ****************************/
  deltaTime = HAL_GetTick() - preTime;
  preTime = HAL_GetTick();
  verticalSpeed = ((ICD_eADC2OFP.AirPosVel.Altitude - preAlt) / (deltaTime/1000.0)) * 3.6;
  preAlt = ICD_eADC2OFP.AirPosVel.Altitude;

  ICD_eADC2OFP.AirPosVel.VerticalVelocity				= getVSpeedKalman(VerticalSpeed_avg_func(verticalSpeed), deltaTime);
  /*************************** AVerticallt Speed Calculate Stop ***************************/

  ICD_eADC2OFP.AirData_Raw.Pressure_PT_Raw				= (uint32_t)((ADC_Val_avg[ENUM_PRESS_DIFF] * FACTOR_100_NOR));
  ICD_eADC2OFP.AirData_Raw.Pressure_PS_Raw				= (uint32_t)((ADC_Val_avg[ENUM_PRESS_STATIC] * FACTOR_100_NOR));

  ICD_eADC2OFP.PTPStemp.PS_Temp							= ADC_Val_avg[ENUM_TEMP_STATIC];
  ICD_eADC2OFP.PTPStemp.PT_Temp							= ADC_Val_avg[ENUM_TEMP_DIFF];

  memcpy(&ADS2OFP_Buff[0], & ICD_eADC2OFP, sizeof(ICD_eADC2OFP));

  ICD_eADC2OFP.Checksum = 0;
  for(loop = 2; loop < sizeof(ICD_eADC2OFP) - 1; loop++)
  {
	  ICD_eADC2OFP.Checksum ^=  ADS2OFP_Buff[loop];
  }
  ADS2OFP_Buff[sizeof(ICD_eADC2OFP) - 1] = ICD_eADC2OFP.Checksum;


  HAL_UART_Transmit_DMA(&huart1, (uint8_t *)&ADS2OFP_Buff, sizeof(ADS2OFP_Buff));
}



void task_ISS2ADS(void const * argument)
{
	/* USER CODE BEGIN task_GCS2ADS */
	TickType_t xLastWakeTime = xTaskGetTickCount();
	const TickType_t xFrequency = 10;

	vTaskDelay(1000);

//	kal_init();
//	myKalmanInit();
	KalmanInit();

	/* Infinite loop */
	for(;;)
	{
		ADS2OFP_DATA();

		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
	/* USER CODE END task_GCS2ADS */
}


