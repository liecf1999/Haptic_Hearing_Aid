################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS_DSP/Source/BasicMathFunctions/BasicMathFunctions.c \
../Drivers/CMSIS_DSP/Source/BasicMathFunctions/BasicMathFunctionsF16.c \
../Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_abs_f32.c \
../Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_add_f32.c \
../Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_mult_f32.c \
../Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_negate_f32.c \
../Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_scale_f32.c \
../Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_sub_f32.c 

OBJS += \
./Drivers/CMSIS_DSP/Source/BasicMathFunctions/BasicMathFunctions.o \
./Drivers/CMSIS_DSP/Source/BasicMathFunctions/BasicMathFunctionsF16.o \
./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_abs_f32.o \
./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_add_f32.o \
./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_mult_f32.o \
./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_negate_f32.o \
./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_scale_f32.o \
./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_sub_f32.o 

C_DEPS += \
./Drivers/CMSIS_DSP/Source/BasicMathFunctions/BasicMathFunctions.d \
./Drivers/CMSIS_DSP/Source/BasicMathFunctions/BasicMathFunctionsF16.d \
./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_abs_f32.d \
./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_add_f32.d \
./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_mult_f32.d \
./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_negate_f32.d \
./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_scale_f32.d \
./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_sub_f32.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS_DSP/Source/BasicMathFunctions/%.o Drivers/CMSIS_DSP/Source/BasicMathFunctions/%.su Drivers/CMSIS_DSP/Source/BasicMathFunctions/%.cyclo: ../Drivers/CMSIS_DSP/Source/BasicMathFunctions/%.c Drivers/CMSIS_DSP/Source/BasicMathFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM4 -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/CMSIS_DSP/Include -I../Drivers/CMSIS_DSP/PrivateInclude -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Audio_Input_Driver/Inc -I../Audio_Input_Driver/Src -I../Driver_Driver/Inc -I../Driver_Driver/Src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS_DSP-2f-Source-2f-BasicMathFunctions

clean-Drivers-2f-CMSIS_DSP-2f-Source-2f-BasicMathFunctions:
	-$(RM) ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/BasicMathFunctions.cyclo ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/BasicMathFunctions.d ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/BasicMathFunctions.o ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/BasicMathFunctions.su ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/BasicMathFunctionsF16.cyclo ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/BasicMathFunctionsF16.d ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/BasicMathFunctionsF16.o ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/BasicMathFunctionsF16.su ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_abs_f32.cyclo ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_abs_f32.d ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_abs_f32.o ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_abs_f32.su ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_add_f32.cyclo ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_add_f32.d ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_add_f32.o ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_add_f32.su ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_mult_f32.cyclo ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_mult_f32.d ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_mult_f32.o ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_mult_f32.su ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_negate_f32.cyclo ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_negate_f32.d ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_negate_f32.o ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_negate_f32.su ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_scale_f32.cyclo ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_scale_f32.d ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_scale_f32.o ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_scale_f32.su ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_sub_f32.cyclo ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_sub_f32.d ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_sub_f32.o ./Drivers/CMSIS_DSP/Source/BasicMathFunctions/arm_sub_f32.su

.PHONY: clean-Drivers-2f-CMSIS_DSP-2f-Source-2f-BasicMathFunctions

