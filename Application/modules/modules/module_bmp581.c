/*
 * module_bmp581.c
 *
 *  Created on: Oct 31, 2023
 *      Author: kjkim
 */
#include "../../../Application/include/modules/module_bmp581.h"
#include "app_config.h"

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;

static int8_t set_config(struct bmp5_fifo *fifo, struct bmp5_dev *dev);
static int8_t get_fifo_data(struct bmp5_fifo *fifo, struct bmp5_dev *dev);

#define LOOP_COUNT                  UINT8_C(20)
#define BMP5_FIFO_DATA_BUFFER_SIZE  UINT8_C(96)
#define BMP5_FIFO_DATA_USER_LENGTH  UINT8_C(96)
#define BMP5_FIFO_P_T_FRAME_COUNT   UINT8_C(16)
#define BMP5_FIFO_T_FRAME_COUNT     UINT8_C(32)
#define BMP5_FIFO_P_FRAME_COUNT     UINT8_C(32)

extern str_bit CBIT, IBIT, PBIT;

void bmp581_init(BMP581_DEV *sensor)
{
	int8_t rslt = BMP5_OK;

	sensor[0].intf_ptr = &hi2c1;
	sensor[1].intf_ptr = &hi2c1;
	sensor[2].intf_ptr = &hi2c2;
	sensor[3].intf_ptr = &hi2c2;

	sensor[0].dev_addr = (BMP5_I2C_ADDR_PRIM<<1);
	sensor[1].dev_addr = (BMP5_I2C_ADDR_SEC<<1);
	sensor[2].dev_addr = (BMP5_I2C_ADDR_PRIM<<1);
	sensor[3].dev_addr = (BMP5_I2C_ADDR_SEC<<1);

	for (int i = 0; i < BME581_COUNT; i++)
	{
		bmp5_interface_init(&sensor[i], BMP5_I2C_INTF);
		rslt = bmp5_soft_reset(&sensor[i]);
		sensor[i].soft_reset_error = rslt;
		HAL_Delay(3);

	    if (rslt == BMP5_OK)
	    {
	        rslt = bmp5_init(&sensor[i]);
	        sensor[i].init_error = rslt;
	        PBIT.sensor_status[i] = rslt;

	        if (rslt == BMP5_OK)
	        {
	            rslt = set_config(&sensor[i].fifo, &sensor[i]);
	            sensor[i].config_error = rslt;
	            IBIT.sensor_status[i] = rslt;
	        }
	    }
	}
}


int8_t get_data(BMP581_DEV *sensor)
{
	int8_t rslt = BMP5_OK;

	rslt = get_fifo_data(&sensor->fifo, sensor);

    return rslt;
}


static int8_t set_config(BMP5_FIFO *fifo, BMP581_DEV *dev)
{
    int8_t rslt;
    struct bmp5_iir_config set_iir_cfg;
    struct bmp5_osr_odr_press_config osr_odr_press_cfg;
    struct bmp5_int_source_select int_source_select;

    rslt = bmp5_set_power_mode(BMP5_POWERMODE_STANDBY, dev);

    if (rslt == BMP5_OK)
    {
        /* Get default odr */
        rslt = bmp5_get_osr_odr_press_config(&osr_odr_press_cfg, dev);

        if (rslt == BMP5_OK)
        {
            /* Set ODR as 50Hz */
            osr_odr_press_cfg.odr = BMP5_ODR_50_HZ;

            /* Enable pressure */

            /* NOTE: If temperature data only is required then pressure enable(press_en) is not required
             * or can be disabled. */
            osr_odr_press_cfg.press_en = BMP5_ENABLE;

            /* Set Over-sampling rate with respect to odr */
            osr_odr_press_cfg.osr_t = BMP5_OVERSAMPLING_8X;
            osr_odr_press_cfg.osr_p = BMP5_OVERSAMPLING_64X;

            rslt = bmp5_set_osr_odr_press_config(&osr_odr_press_cfg, dev);

            if (rslt == BMP5_OK)
            {
                set_iir_cfg.set_iir_t = BMP5_IIR_FILTER_COEFF_127;
                set_iir_cfg.set_iir_p = BMP5_IIR_FILTER_COEFF_127;

                rslt = bmp5_set_iir_config(&set_iir_cfg, dev);
            }
        }

        if (rslt == BMP5_OK)
        {
            rslt = bmp5_get_fifo_configuration(fifo, dev);

            if (rslt == BMP5_OK)
            {
                fifo->mode = BMP5_FIFO_MODE_STREAMING;

                /* Frame selection can be used to select data frames,
                 * pressure data only(32 frames) - BMP5_FIFO_PRESSURE_DATA
                 * temperature data only(32 frames) - BMP5_FIFO_TEMPERATURE_DATA
                 * both pressure and temperature data(16 frames) - BMP5_FIFO_PRESS_TEMP_DATA
                 * Here, both pressure and temperature data is selected(BMP5_FIFO_PRESS_TEMP_DATA)
                 */
                fifo->frame_sel = BMP5_FIFO_PRESS_TEMP_DATA;

                fifo->dec_sel = BMP5_FIFO_NO_DOWNSAMPLING;
                fifo->set_fifo_iir_t = BMP5_ENABLE;
                fifo->set_fifo_iir_p = BMP5_ENABLE;

                rslt = bmp5_set_fifo_configuration(fifo, dev);
            }
        }

        if (rslt == BMP5_OK)
        {
            rslt = bmp5_configure_interrupt(BMP5_PULSED, BMP5_ACTIVE_HIGH, BMP5_INTR_PUSH_PULL, BMP5_INTR_ENABLE, dev);

            if (rslt == BMP5_OK)
            {
                /* Note : Select INT_SOURCE after configuring interrupt */
                int_source_select.fifo_full_en = BMP5_ENABLE;
                rslt = bmp5_int_source_select(&int_source_select, dev);
            }
        }

        /*
         * FIFO example can be executed on,
         * normal mode - BMP5_POWERMODE_NORMAL
         * continuous mode - BMP5_POWERMODE_CONTINOUS
         * Here, used normal mode (BMP5_POWERMODE_NORMAL)
         */
        rslt = bmp5_set_power_mode(BMP5_POWERMODE_NORMAL, dev);
    }

    return rslt;
}


static int8_t get_fifo_data(BMP5_FIFO *fifo, BMP581_DEV *dev)
{
    int8_t rslt = 0;
    uint8_t idx = 0;
    uint8_t int_status;
    uint8_t fifo_buffer[BMP5_FIFO_DATA_BUFFER_SIZE];
    uint8_t loop = 0;
    struct bmp5_sensor_data sensor_data[BMP5_FIFO_P_T_FRAME_COUNT] = { { 0 } };

    while (loop < LOOP_COUNT)
    {
        rslt = bmp5_get_interrupt_status(&int_status, dev);

        if (int_status & BMP5_INT_ASSERTED_FIFO_FULL)
        {
            fifo->length = BMP5_FIFO_DATA_USER_LENGTH;
            fifo->data = fifo_buffer;

            rslt = bmp5_get_fifo_data(fifo, dev);

            if (rslt == BMP5_OK)
            {
                rslt = bmp5_extract_fifo_data(fifo, sensor_data);

                if (rslt == BMP5_OK)
                {
                    for (idx = 0; idx < fifo->fifo_count; idx++)
                    {
#ifdef BMP5_USE_FIXED_POINT
                        printf("%d, %lu, %ld\n",
                               idx,
                               (long unsigned int)sensor_data[idx].pressure,
                               (long int)sensor_data[idx].temperature);
#else
//                        printf("%d, %f, %f\n", idx, sensor_data[idx].pressure, sensor_data[idx].temperature);//ㅡ
                        dev->sensor_data.pressure = sensor_data[idx].pressure;
                        dev->sensor_data.temperature = sensor_data[idx].temperature;
#endif
                    }


                }

                loop++;
            }
        }
        loop++;
    }

    return rslt;
}
