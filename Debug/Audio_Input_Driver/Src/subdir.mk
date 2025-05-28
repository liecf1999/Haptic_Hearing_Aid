################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Audio_Input_Driver/Src/audio.c 

OBJS += \
./Audio_Input_Driver/Src/audio.o 

C_DEPS += \
./Audio_Input_Driver/Src/audio.d 


# Each subdirectory must supply rules for building sources it contributes
Audio_Input_Driver/Src/%.o Audio_Input_Driver/Src/%.su Audio_Input_Driver/Src/%.cyclo: ../Audio_Input_Driver/Src/%.c Audio_Input_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM4 -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/DSP/Include -I../Drivers/CMSIS/DSP/Source -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Audio_Input_Driver/Inc -I../Audio_Input_Driver/Src -I../Driver_Driver/Inc -I../Driver_Driver/Src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Audio_Input_Driver-2f-Src

clean-Audio_Input_Driver-2f-Src:
	-$(RM) ./Audio_Input_Driver/Src/audio.cyclo ./Audio_Input_Driver/Src/audio.d ./Audio_Input_Driver/Src/audio.o ./Audio_Input_Driver/Src/audio.su

.PHONY: clean-Audio_Input_Driver-2f-Src

