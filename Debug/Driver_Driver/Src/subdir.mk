################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Driver_Driver/Src/Driver_Driver.c 

OBJS += \
./Driver_Driver/Src/Driver_Driver.o 

C_DEPS += \
./Driver_Driver/Src/Driver_Driver.d 


# Each subdirectory must supply rules for building sources it contributes
Driver_Driver/Src/%.o Driver_Driver/Src/%.su Driver_Driver/Src/%.cyclo: ../Driver_Driver/Src/%.c Driver_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Audio_Input_Driver/Inc -I../Audio_Input_Driver/Src -I../Driver_Driver/Inc -I../Driver_Driver/Src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Driver_Driver-2f-Src

clean-Driver_Driver-2f-Src:
	-$(RM) ./Driver_Driver/Src/Driver_Driver.cyclo ./Driver_Driver/Src/Driver_Driver.d ./Driver_Driver/Src/Driver_Driver.o ./Driver_Driver/Src/Driver_Driver.su

.PHONY: clean-Driver_Driver-2f-Src

