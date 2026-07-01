################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/modules/utils/alt_speed.c \
../Application/modules/utils/delay.c \
../Application/modules/utils/iic.c 

OBJS += \
./Application/modules/utils/alt_speed.o \
./Application/modules/utils/delay.o \
./Application/modules/utils/iic.o 

C_DEPS += \
./Application/modules/utils/alt_speed.d \
./Application/modules/utils/delay.d \
./Application/modules/utils/iic.d 


# Each subdirectory must supply rules for building sources it contributes
Application/modules/utils/%.o Application/modules/utils/%.su Application/modules/utils/%.cyclo: ../Application/modules/utils/%.c Application/modules/utils/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/_YongBeeAT KjKIM/_YongBeeAT_Works/14. ADS/5. eADC/firmware/eADC_G431/Application" -I"C:/_YongBeeAT KjKIM/_YongBeeAT_Works/14. ADS/5. eADC/firmware/eADC_G431/Application/config" -I"C:/_YongBeeAT KjKIM/_YongBeeAT_Works/14. ADS/5. eADC/firmware/eADC_G431/Application/include" -I"C:/_YongBeeAT KjKIM/_YongBeeAT_Works/14. ADS/5. eADC/firmware/eADC_G431/Application/include/drivers" -I"C:/_YongBeeAT KjKIM/_YongBeeAT_Works/14. ADS/5. eADC/firmware/eADC_G431/Application/include/modules" -I"C:/_YongBeeAT KjKIM/_YongBeeAT_Works/14. ADS/5. eADC/firmware/eADC_G431/Application/include/utils" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-modules-2f-utils

clean-Application-2f-modules-2f-utils:
	-$(RM) ./Application/modules/utils/alt_speed.cyclo ./Application/modules/utils/alt_speed.d ./Application/modules/utils/alt_speed.o ./Application/modules/utils/alt_speed.su ./Application/modules/utils/delay.cyclo ./Application/modules/utils/delay.d ./Application/modules/utils/delay.o ./Application/modules/utils/delay.su ./Application/modules/utils/iic.cyclo ./Application/modules/utils/iic.d ./Application/modules/utils/iic.o ./Application/modules/utils/iic.su

.PHONY: clean-Application-2f-modules-2f-utils

