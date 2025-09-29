################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/modules/modules/common.c \
../Application/modules/modules/module_bmp581.c 

OBJS += \
./Application/modules/modules/common.o \
./Application/modules/modules/module_bmp581.o 

C_DEPS += \
./Application/modules/modules/common.d \
./Application/modules/modules/module_bmp581.d 


# Each subdirectory must supply rules for building sources it contributes
Application/modules/modules/%.o Application/modules/modules/%.su Application/modules/modules/%.cyclo: ../Application/modules/modules/%.c Application/modules/modules/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"E:/_YongBeeAT_Works/14. ADS/5. eADC/firmware/eADC_G431/Application" -I"E:/_YongBeeAT_Works/14. ADS/5. eADC/firmware/eADC_G431/Application/config" -I"E:/_YongBeeAT_Works/14. ADS/5. eADC/firmware/eADC_G431/Application/include" -I"E:/_YongBeeAT_Works/14. ADS/5. eADC/firmware/eADC_G431/Application/include/drivers" -I"E:/_YongBeeAT_Works/14. ADS/5. eADC/firmware/eADC_G431/Application/include/modules" -I"E:/_YongBeeAT_Works/14. ADS/5. eADC/firmware/eADC_G431/Application/include/utils" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-modules-2f-modules

clean-Application-2f-modules-2f-modules:
	-$(RM) ./Application/modules/modules/common.cyclo ./Application/modules/modules/common.d ./Application/modules/modules/common.o ./Application/modules/modules/common.su ./Application/modules/modules/module_bmp581.cyclo ./Application/modules/modules/module_bmp581.d ./Application/modules/modules/module_bmp581.o ./Application/modules/modules/module_bmp581.su

.PHONY: clean-Application-2f-modules-2f-modules

