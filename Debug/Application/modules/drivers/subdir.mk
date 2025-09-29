################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/modules/drivers/bmp5.c \
../Application/modules/drivers/drv_AD7799v2.c \
../Application/modules/drivers/drv_FM25V01.c \
../Application/modules/drivers/drv_ad7799.c \
../Application/modules/drivers/drv_ad7799chat.c 

OBJS += \
./Application/modules/drivers/bmp5.o \
./Application/modules/drivers/drv_AD7799v2.o \
./Application/modules/drivers/drv_FM25V01.o \
./Application/modules/drivers/drv_ad7799.o \
./Application/modules/drivers/drv_ad7799chat.o 

C_DEPS += \
./Application/modules/drivers/bmp5.d \
./Application/modules/drivers/drv_AD7799v2.d \
./Application/modules/drivers/drv_FM25V01.d \
./Application/modules/drivers/drv_ad7799.d \
./Application/modules/drivers/drv_ad7799chat.d 


# Each subdirectory must supply rules for building sources it contributes
Application/modules/drivers/%.o Application/modules/drivers/%.su Application/modules/drivers/%.cyclo: ../Application/modules/drivers/%.c Application/modules/drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"E:/_YongBeeAT_Works/14. ADS/5. eADC/firmware/eADC_G431/Application" -I"E:/_YongBeeAT_Works/14. ADS/5. eADC/firmware/eADC_G431/Application/config" -I"E:/_YongBeeAT_Works/14. ADS/5. eADC/firmware/eADC_G431/Application/include" -I"E:/_YongBeeAT_Works/14. ADS/5. eADC/firmware/eADC_G431/Application/include/drivers" -I"E:/_YongBeeAT_Works/14. ADS/5. eADC/firmware/eADC_G431/Application/include/modules" -I"E:/_YongBeeAT_Works/14. ADS/5. eADC/firmware/eADC_G431/Application/include/utils" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-modules-2f-drivers

clean-Application-2f-modules-2f-drivers:
	-$(RM) ./Application/modules/drivers/bmp5.cyclo ./Application/modules/drivers/bmp5.d ./Application/modules/drivers/bmp5.o ./Application/modules/drivers/bmp5.su ./Application/modules/drivers/drv_AD7799v2.cyclo ./Application/modules/drivers/drv_AD7799v2.d ./Application/modules/drivers/drv_AD7799v2.o ./Application/modules/drivers/drv_AD7799v2.su ./Application/modules/drivers/drv_FM25V01.cyclo ./Application/modules/drivers/drv_FM25V01.d ./Application/modules/drivers/drv_FM25V01.o ./Application/modules/drivers/drv_FM25V01.su ./Application/modules/drivers/drv_ad7799.cyclo ./Application/modules/drivers/drv_ad7799.d ./Application/modules/drivers/drv_ad7799.o ./Application/modules/drivers/drv_ad7799.su ./Application/modules/drivers/drv_ad7799chat.cyclo ./Application/modules/drivers/drv_ad7799chat.d ./Application/modules/drivers/drv_ad7799chat.o ./Application/modules/drivers/drv_ad7799chat.su

.PHONY: clean-Application-2f-modules-2f-drivers

