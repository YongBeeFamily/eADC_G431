/*
 * app_config.h
 *
 *  Created on: Sep 2, 2022
 *      Author: ybkjk
 */

#ifndef CONFIG_APP_CONFIG_H_
#define CONFIG_APP_CONFIG_H_

#include "main.h"


#ifdef DATA_ADS2OFP
        #define ADS2OFP_EXT
#else
        #define ADS2OFP_EXT		extern
#endif

/************************************************************************/
#define ADS_VERSION				3
/************************************************************************/



#define UART2_RX_BUFFER_SIZE			500

// NUCLEO-F429ZI LED SET ---------------------------------------------
#define LED_TOGGLE()		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0)

#define true		1
#define false		0


#define FACTOR_10_NOR                   10
#define FACTOR_100_NOR                  100
#define FACTOR_1000_NOR                 1000
#define FACTOR_10000_NOR                10000
#define FACTOR_100000_NOR               100000
#define FACTOR_1000000_NOR              1000000
#define FACTOR_10000000_NOR             10000000

#define FACTOR_10_INV                   0.1
#define FACTOR_100_INV                  0.01
#define FACTOR_1000_INV                 0.001
#define FACTOR_10000_INV                0.0001
#define FACTOR_100000_INV               0.00001
#define FACTOR_1000000_INV              0.000001
#define FACTOR_10000000_INV             0.0000001

  //  #define PI 3.14159265358979323846
#define RAD2DEG                         57.295779513082323
#define DEG2RAD                         0.01745329251994329576923690768489		//   pi / 180 = 0.01745329251994329576923690768489
#define MSEC2KMH                        3.6
#define FACTOR_MSEC2KMH                 MSEC2KMH

  // Define Variable For DataPacket_Check Function
#define DATA_READY                      0
#define ERR_CHECKSUM                    1
#define ERR_DATASIZE                    2
#define ERR_HEADER                      3
#define DATA_NOTREADY                   4

  // Define Engage, Disengage
#define ENGAGE                          1
#define DISENGAGE                       0

// Define OFP to GCS MSG Buffer
#define SIZEOF_UINT8                            1
#define SIZEOF_UINT16                           2
#define SIZEOF_UINT32                           4
#define SIZEOF_INT8                             1
#define SIZEOF_INT16                            2
#define SIZEOF_INT32                            4
#define SIZEOF_FLOAT                            4
#define SIZEOF_DOUBLE                           8


#define BMP581_STATUS_OK						112
#define BMP581_FATAL_ERROR						1
#define BMP581_STATUS_OK						112
#define BMP581_FATAL_ERROR						1

#pragma pack(push,1)   // pragma directive to specify the alignment of structs (1 byte)
typedef struct
{
	uint8_t 					HEADER1;
	uint8_t 					HEADER2;
	uint8_t 					Label;
	float						data[20];
	uint8_t 					Checksum;

} Str_uart_tx;

typedef struct
{
	uint8_t 					HEADER1;
	uint8_t 					HEADER2;
	uint8_t 					Label;
	uint8_t 					SERIALNO[32];
	float						data[10];
	uint8_t 					Checksum;
} Str_uart_rx;
#pragma pack(pop)



typedef enum
{
	ENUM_PRESS_STATIC = 0,
	ENUM_TEMP_STATIC,
	ENUM_PRESS_DIFF ,
	ENUM_TEMP_DIFF,
	ENUM_PIT_TEMP ,
	ENUM_AOA ,
	ENUM_AOS ,
	ENUM_MAX_NUM,
} ENUM_ADC_CH_DEF;
#define ADC_MAX			ENUM_MAX_NUM



#pragma pack(push,1)   // pragma directive to specify the alignment of structs (1 byte)
typedef struct
{
	float Altitude;
	float Velocity;
} ICD_ISS_AIRPOSVEL_TYPE, *ptr_ICD_ISS_AIRPOSVEL_TYPE;

typedef struct
{
	float PITOT_Temperature;
	float Pressure_Static;
	float Pressure_Differential;
} ICD_ISS_AIRDATA_TYPE, *ptr_ICD_ISS_AIRDATA_TYPE;

typedef struct
{
	uint16_t PITOT_Temperature_Raw;
	uint32_t Pressure_Static_Raw;
	uint32_t Pressure_Differential_Raw;
} ICD_ISS_AIRDATA_RAW_TYPE, *ptr_ICD_ISS_AIRDATA_RAW_TYPE;

typedef struct
{
	float AOA_Angle;
	float AOS_Angle;
} ICD_ISS_ANGLEDATA_TYPE, *ptr_ICD_ISS_ANGLEDATA_TYPE;

typedef struct
{
	uint16_t AOA_Raw;
	uint16_t AOS_Raw;
} ICD_ISS_ANGLEDATA_RAW_TYPE, *ptr_ICD_ISS_ANGLEDATA_RAW_TYPE;
typedef struct
{
	ICD_ISS_AIRPOSVEL_TYPE			AirPosVel;
	ICD_ISS_AIRDATA_TYPE			AirData;
	ICD_ISS_AIRDATA_RAW_TYPE		AirData_Raw;
	ICD_ISS_ANGLEDATA_TYPE			AngleData;
	ICD_ISS_ANGLEDATA_RAW_TYPE		AngleData_Raw;

} ICD_ISS_DATA_TYPE, *ptr_ICD_ISS_DATA_TYPE;

typedef struct
{
	uint16_t Unique_Start;
	double ALT_COEF[4];
	double VEL_COEF[4];
	double PS_COEF[4];
	double PT_COEF[4];
	double AOA_COEF[3];
	double AOS_COEF[3];
	double TEMP_COEF[3];
	uint8_t PS_Sensor_type;
	uint8_t PT_Sensor_type;
	uint16_t Unique_End;
} ICD_ISS_COEF_TYPE;
#pragma pack(pop)








#define HEADER_ADS2OFP0				        'A'
#define HEADER_ADS2OFP1				        'O'

#pragma pack(push,1)   // pragma directive to specify the alignment of structs (1 byte)

	typedef struct
	{
		float 						Altitude;
		float 						Velocity;
	} AIRPOSVEL_TYPE, *ptr_AIRPOSVEL_TYPE;

	typedef struct
	{
		float 					PITOT_Temperature;
		float 					Pressure_Static;
		float 					Pressure_Differential;
	} AIRDATA_TYPE, *ptr_AIRDATA_TYPE;

	typedef struct
	{
		uint32_t 					PITOT_Temperature_Raw;
		uint32_t 					Pressure_Static_Raw;
		uint32_t 					Pressure_Differential_Raw;
		uint32_t					OperationTime;
	} AIRDATA_RAW_TYPE, *ptr_AIRDATA_RAW_TYPE;

	typedef struct
	{
		float 					AOA_Angle;
		float 					AOS_Angle;
	} ANGLEDATA_TYPE, *ptr_ANGLEDATA_TYPE;

	typedef struct
	{
		float 					PS_Temp;
		float 					PT_Temp;
	} BMP390TEMP, *ptr_BMP390TEMP;

	typedef struct
	{
		uint32_t 					AOA_Raw;
		uint32_t 					AOS_Raw;
	} ANGLEDATA_RAW_TYPE, *ptr_ANGLEDATA_RAW_TYPE;

	typedef struct
	{
		uint8_t 					HEADER1;
		uint8_t 					HEADER2;

		uint8_t 					CMD_Counter;

		uint8_t 					BMP581_STATUS;

		AIRPOSVEL_TYPE 				AirPosVel;
		AIRDATA_TYPE 				AirData;
		AIRDATA_RAW_TYPE 			AirData_Raw;
		ANGLEDATA_TYPE 				AngleData;
		ANGLEDATA_RAW_TYPE 			AngleData_Raw;
		BMP390TEMP					PTPStemp;

		int16_t 					version;
		uint8_t 					Checksum;
	} ICD_ADS2OFP_TYPE, *ptr_ICD_ADS2OFP_TYPE;
#pragma pack(pop)

#pragma pack(push,1)   // pragma directive to specify the alignment of structs (1 byte)
typedef struct
{
	uint8_t 					HEADER1;
	uint8_t 					HEADER2;

	uint8_t 					CMD_Counter;
	uint8_t 					BMP581_STATUS;

	float						Pressures[8];
	float						Temperature[8];
	float						PressTimeAvg[8];
	float						TempTimeAvg[8];
	float						psPressEnsembleAvg;
	float						ptPressEnsembleAvg;
	float						psTempEnsembleAvg;
	float						ptTempEnsembleAvg;

	int16_t 					version;
	uint8_t 					Checksum;
} ICD_ADS2OFP_RAW_TYPE, *ptr_ICD_ADS2OFP_RAW_TYPE;
#pragma pack(pop)

// FM25V01 LOG DATA
#define HEADER_LOG0				        'L'	// 0x4C
#define HEADER_LOG1				        'G'	// 0x47
#pragma pack(push,1)   // pragma directive to specify the alignment of structs (1 byte)
typedef struct
{
	uint32_t					TIME;

	uint8_t						BMP581_FATALERROR_CODE;

	uint32_t 					BMP581_PRESSURE_Raw[8];

	uint32_t 					Pressure_Static_Raw;
	uint32_t 					Pressure_Differential_Raw;

	float 						Altitude;
	float 						Velocity;
	uint32_t 					AOA_Raw;
	uint32_t 					AOS_Raw;

} LOG_DATA_TYPE, *ptr_LOG_DATA_TYPE;

typedef struct
{
	uint8_t 					HEADER1;
	uint8_t 					HEADER2;
	uint8_t 					label;

	LOG_DATA_TYPE 				LOGDATA;

	uint8_t 					Checksum;
} LOG_DATA_UART_TYPE, *ptr_LOG_DAT_UART_TYPE;
#pragma pack(pop)

#pragma pack(push,1)   // pragma directive to specify the alignment of structs (1 byte)
	typedef struct
	{
		float 						Altitude;
		float 						Velocity;
		float 						VerticalVelocity;
	} eADC_AIRPOSVEL_TYPE, *ptr_eADC_AIRPOSVEL_TYPE;

	typedef struct
	{
		uint32_t 					Pressure_PT_Raw;
		uint32_t 					Pressure_PS_Raw;
	} eADC_AIRDATA_RAW_TYPE, *ptr_eADC_AIRDATA_RAW_TYPE;

	typedef struct
	{
		float 					PT_Temp;
		float 					PS_Temp;
	} eADC_BMP390TEMP, *ptr_eADC_BMP390TEMP;


	typedef struct
	{
		uint8_t 					HEADER1;
		uint8_t 					HEADER2;

		uint8_t 					CMD_Counter;

		uint8_t 					BMP581_STATUS;		// 4

		eADC_AIRPOSVEL_TYPE 		AirPosVel;			// 12
		eADC_AIRDATA_RAW_TYPE 		AirData_Raw;		// 8
		eADC_BMP390TEMP				PTPStemp;			// 8

		uint8_t 					Checksum;			// 1
	} ICD_eADC2OFP_TYPE, *ptr_ICD_eADCeADC2OFP_TYPE;	// 33 = 4 + 12 + 8 + 8 + 1
#pragma pack(pop)


#endif /* CONFIG_APP_CONFIG_H_ */


