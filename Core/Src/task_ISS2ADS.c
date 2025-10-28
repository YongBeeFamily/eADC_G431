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
extern str_bit CBIT, IBIT, PBIT;

extern BMP581_DEV sensor[ADC_MAX];

ICD_eADC2OFP_TYPE		ICD_eADC2OFP;

extern uint8_t bmp581_status;
uint32_t operationTime;

#define TemperatureComrrection

// 온도보정 기본값 (원래 있던 값들)
static double aCAL_C0 = 0.0;      // 상수 편향 (m)
static double aCAL_C1 = 0.7;      // 온도 계수 (m / °C)
const double aT_REF = 24.0;       // 기준 온도 (°C)

// ℎ_𝑓𝑙𝑡 상수값
#define PSL_EXPONENT            0.190255
#define HFLT_COEF               (4.4308 * pow(10.0,4))  // 44308.0
#define HFLT_PSL                101325.0                // Pa

static double Calib_Volt2PS(float psRaw)
{
    double altitude;
    double psPa = (double)psRaw;

    // ---------- 1) 단위 검사: psRaw가 hPa로 들어오는 경우 대비 ----------
    if (psPa < 2000.0) { // 보통 hPa 값은 1000~1100 범위
        psPa *= 100.0;   // hPa -> Pa
    }

    // ---------- 2) 기본 고도 계산 ----------
    double psDiff = (psPa / HFLT_PSL);
    double psPowf = pow(psDiff, PSL_EXPONENT);
    altitude = HFLT_COEF * fabs(1.0 - psPowf); // 보정 전 고도 (m)

    // ---------- 3) altitude 기반 스케일 보정 (선형 보간: 0m->coef1, 5000m->coef2) ----------
    double alt1 = 0.0;
    double alt2 = 5000.0;
    double coef1 = 1.0006;    // 저고도 보정계수 (예시, 필요시 조정)
    double coef2 = 1.00033;   // 고고도 보정계수 (예시, 필요시 조정)

    double scale;
    if (altitude <= alt1) {
        scale = coef1;
    } else if (altitude >= alt2) {
        scale = coef2;
    } else {
        scale = coef1 + (altitude - alt1) * (coef2 - coef1) / (alt2 - alt1);
    }
    altitude *= scale;

#ifdef TemperatureComrrection
    // ---------- 4) 온도 보정량 계산 (기존 선형식: CAL_C0 + CAL_C1 * dT) ----------
    double temp = (double)ADC_Val_avg[ENUM_TEMP_STATIC]; // 현재 온도 (°C)
    double dT = temp - aT_REF;
    double temp_err = aCAL_C0 + aCAL_C1 * dT; // 온도에 따른 예측 오차 (m), 전체 적용량

    // ---------- 5) 고도에 따른 온도 보정 적용 비율 (선형) ----------
    // 고도 0 근처에서는 보정 0 (적용 안 함), 고도 5000m 이상에서는 보정 100% 적용
    double corr_alt_min = 0.0;      // 보정 시작 고도 (여기서는 0m에서 0%)
    double corr_alt_max = 5000.0;   // 보정이 완전 적용되는 고도 (5000m에서 100%)

    double corr_factor;
    if (altitude <= corr_alt_min) {
        corr_factor = 0.0;
    } else if (altitude >= corr_alt_max) {
        corr_factor = 1.0;
    } else {
        corr_factor = (altitude - corr_alt_min) / (corr_alt_max - corr_alt_min); // 선형 보간 0..1
    }

    // ---------- 6) 최종 보정 적용 ----------
    double total_err = corr_factor * temp_err;   // 적용 비율을 곱해 점진적 보정
    double altitude_corr = altitude - total_err;

    return altitude_corr;
#else
    // TemperatureComrrection이 정의되지 않으면 온도 보정 안 함
    return altitude;
#endif
}





// 𝑉_𝐼𝐴𝑆 상수값
// 주의: STANDARD_AIR_DENSITY는 여기서 kPa 단위(101.325 kPa)로 적어두었으나,
// 실제 식에서는 ΔP와 ρ 단위가 일치해야 합니다. (아래 구현은 ΔP를 kPa 단위로 사용)
#define STANDARD_AIR_DENSITY    101.325   // kPa  (주의: 단위 일관성 확인 권장)
const double sT_REF = 24.0;     // 기준 온도 (°C)
static double sCAL_C0 = 0.0;    // 온도 보정 상수 (사용자 정의)
static double sCAL_C1 = 0.7;    // 온도 계수 (단위: speed per °C — 필요 시 조정)

// Velocity : 𝑉_𝐼𝐴𝑆 = sqrt((2 * ΔP) / ρ_0)  (단위 일치 필요)
static float Calib_Volt2PT(float psRaw, float ptRaw)
{
    double velo;
    double ptpsDiff = (double)ptRaw - (double)psRaw;

    // -------------------------
    // 단위 주의:
    // - 만약 psRaw/ptRaw가 kPa 단위이면 여기서 추가 변환 불필요
    // - 만약 psRaw/ptRaw가 Pa 단위이면 아래에서 kPa로 변환(나누기 1000)하거나
    //   STANDARD_AIR_DENSITY를 Pa 단위로 바꿔야 합니다.
    //
    // (원래 코드에서 ptpsDiff *= 1000.0; 로 변환했는데 주석과 혼동이 있어 제거함.
    //  단위가 Pa인지 kPa인지 확실하면 해당 줄을 복원/수정하세요.)
    // -------------------------

    // 절대값 사용 (정압보다 정압계가 낮은 경우 음수 방지)
    double deltaP = fabs(ptpsDiff); // 단위: (센서 단위 — 일관성 필요)

    // 예전 코드 구조를 살려 "rootValue" 계산 (단위 일치 전제)
    double rootValue = fabs((2.0 * deltaP) / STANDARD_AIR_DENSITY);
    velo = sqrt(rootValue);

    // -------------------------
    // 1) 속도 기반 스케일 보정 (선형 보간)
    //    - 저속(예: 0)에서 약간의 보정계수 coef_low
    //    - 고속(예: 390 이상)에서 coef_high
    //    이 구간 내에서 선형 보간 적용 (계수는 실측치로 튜닝)
    // -------------------------
    double speed1 = 0.0;
    double speed2 = 390.0; // 기존 분기값을 참고하여 전 구간 보정 최대값으로 설정
    double coef_low = pow(1.013779, 2);  // 기존 저속계수 (원래 코드에서 사용)
    double coef_high = pow(1.008169, 2); // 기존 고속계수 (원래 코드에서 사용)

    double scale;
    if (velo <= speed1) {
        scale = coef_low;
    } else if (velo >= speed2) {
        scale = coef_high;
    } else {
        scale = coef_low + (velo - speed1) * (coef_high - coef_low) / (speed2 - speed1);
    }
    double velo_scaled = velo * scale;

#ifdef TemperatureComrrection
    // -------------------------
    // 2) 온도 보정량 계산 (기존 선형식 사용)
    //    temp_err는 "속도 단위로의 오차"를 나타낸다고 가정
    // -------------------------
    double temp = (double)ADC_Val_avg[ENUM_TEMP_STATIC];
    double dT = temp - sT_REF;
    double temp_err = sCAL_C0 + sCAL_C1 * dT; // (단위: same as velo, 조정 필요)

    // -------------------------
    // 3) 속도에 따른 온도보정 적용 비율 (선형)
    //    - 속도 <= corr_speed_min  : 보정 0%
    //    - 속도 >= corr_speed_max  : 보정 100%
    //    - 그 사이는 선형적으로 확대
    // -------------------------
    double corr_speed_min = 0.0;    // 보정 시작 속도 (예: 0 m/s)
    double corr_speed_max = 390.0;  // 보정이 완전 적용되는 속도 (예: 390 m/s)

    double corr_factor;
    if (velo_scaled <= corr_speed_min) {
        corr_factor = 0.0;
    } else if (velo_scaled >= corr_speed_max) {
        corr_factor = 1.0;
    } else {
        corr_factor = (velo_scaled - corr_speed_min) / (corr_speed_max - corr_speed_min);
    }

    // -------------------------
    // 4) 최종 보정 적용
    //    - 온도에 따른 보정량(temp_err)을 corr_factor로 스케일
    // -------------------------
    double total_err = corr_factor * temp_err;
    double speed_corr = velo_scaled - total_err;

    return (float)speed_corr;
#else
    // TemperatureComrrection이 정의되지 않은 경우 변환된 속도만 반환
    return (float)velo_scaled;
#endif
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
extern str_bit CBIT, IBIT, PBIT;

void ADS2OFP_DATA(void)
{
  uint8_t loop = 0;
  ICD_eADC2OFP.HEADER1 = HEADER_ADS2OFP0;	// 65
  ICD_eADC2OFP.HEADER2 = HEADER_ADS2OFP1;	// 79

  ICD_eADC2OFP.CMD_Counter++;

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

  ICD_eADC2OFP.bit.PBIT = 0;
  ICD_eADC2OFP.bit.IBIT = 0;
  ICD_eADC2OFP.bit.CBIT = 0;
  for (int i = 0; i < BME581_COUNT; i++)
  {
	  if (i < BME581_COUNT)
	  {
		  if (PBIT.sensor_status[i] != BMP5_OK) ICD_eADC2OFP.bit.PBIT |= (0x1 << i);
		  if (IBIT.sensor_status[i] != BMP5_OK) ICD_eADC2OFP.bit.IBIT |= (0x1 << i);
		  if (CBIT.sensor_status[i] != BMP5_OK) ICD_eADC2OFP.bit.CBIT |= (0x1 << i);
	  }
  }
  if (PBIT.uart_status != BMP5_OK) ICD_eADC2OFP.bit.PBIT |= (0x1 << 4);
  if (IBIT.uart_status != BMP5_OK) ICD_eADC2OFP.bit.IBIT |= (0x1 << 4);
  if (CBIT.uart_status != BMP5_OK) ICD_eADC2OFP.bit.CBIT |= (0x1 << 4);

  if (PBIT.flash_status != BMP5_OK) ICD_eADC2OFP.bit.PBIT |= (0x1 << 5);
  if (IBIT.flash_status != BMP5_OK) ICD_eADC2OFP.bit.IBIT |= (0x1 << 5);
  if (CBIT.flash_status != BMP5_OK) ICD_eADC2OFP.bit.CBIT |= (0x1 << 5);

  if (PBIT.watchdog_status != BMP5_OK) ICD_eADC2OFP.bit.PBIT |= (0x1 << 6);
  if (IBIT.watchdog_status != BMP5_OK) ICD_eADC2OFP.bit.IBIT |= (0x1 << 6);
  if (CBIT.watchdog_status != BMP5_OK) ICD_eADC2OFP.bit.CBIT |= (0x1 << 6);

  memcpy(&ADS2OFP_Buff[0], & ICD_eADC2OFP, sizeof(ICD_eADC2OFP));

  ICD_eADC2OFP.Checksum = 0;
  for(loop = 2; loop < sizeof(ICD_eADC2OFP) - 1; loop++)
  {
	  ICD_eADC2OFP.Checksum ^=  ADS2OFP_Buff[loop];
  }
  ADS2OFP_Buff[sizeof(ICD_eADC2OFP) - 1] = ICD_eADC2OFP.Checksum;


  CBIT.uart_status = HAL_UART_Transmit_DMA(&huart1, (uint8_t *)&ADS2OFP_Buff, sizeof(ICD_eADC2OFP));
}


void task_ISS2ADS(void const * argument)
{
	/* USER CODE BEGIN task_GCS2ADS */
	TickType_t xLastWakeTime = xTaskGetTickCount();
	const TickType_t xFrequency = 20;

	vTaskDelay(1000);

//	kal_init();
//	myKalmanInit();
	KalmanInit();

	if(huart1.gState == HAL_UART_STATE_READY)
	{
		PBIT.uart_status = 0;
	}
	else
	{
		PBIT.uart_status = 1;
	}

	/* Infinite loop */
	for(;;)
	{
		ADS2OFP_DATA();

		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
	/* USER CODE END task_GCS2ADS */
}


