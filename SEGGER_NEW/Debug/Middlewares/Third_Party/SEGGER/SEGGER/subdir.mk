################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/SEGGER/SEGGER/SEGGER_RTT.c \
../Middlewares/Third_Party/SEGGER/SEGGER/SEGGER_RTT_printf.c \
../Middlewares/Third_Party/SEGGER/SEGGER/SEGGER_SYSVIEW.c 

S_UPPER_SRCS += \
../Middlewares/Third_Party/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.S 

OBJS += \
./Middlewares/Third_Party/SEGGER/SEGGER/SEGGER_RTT.o \
./Middlewares/Third_Party/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.o \
./Middlewares/Third_Party/SEGGER/SEGGER/SEGGER_RTT_printf.o \
./Middlewares/Third_Party/SEGGER/SEGGER/SEGGER_SYSVIEW.o 

S_UPPER_DEPS += \
./Middlewares/Third_Party/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.d 

C_DEPS += \
./Middlewares/Third_Party/SEGGER/SEGGER/SEGGER_RTT.d \
./Middlewares/Third_Party/SEGGER/SEGGER/SEGGER_RTT_printf.d \
./Middlewares/Third_Party/SEGGER/SEGGER/SEGGER_SYSVIEW.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/SEGGER/SEGGER/%.o Middlewares/Third_Party/SEGGER/SEGGER/%.su: ../Middlewares/Third_Party/SEGGER/SEGGER/%.c Middlewares/Third_Party/SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I"/home/denis/STM32CubeIDE/workspace_1.11.2/SEGGER_NEW/Middlewares/Third_Party/SEGGER/Config" -I"/home/denis/STM32CubeIDE/workspace_1.11.2/SEGGER_NEW/Middlewares/Third_Party/SEGGER/OS" -I"/home/denis/STM32CubeIDE/workspace_1.11.2/SEGGER_NEW/Middlewares/Third_Party/SEGGER/SEGGER" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/SEGGER/SEGGER/%.o: ../Middlewares/Third_Party/SEGGER/SEGGER/%.S Middlewares/Third_Party/SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Middlewares-2f-Third_Party-2f-SEGGER-2f-SEGGER

clean-Middlewares-2f-Third_Party-2f-SEGGER-2f-SEGGER:
	-$(RM) ./Middlewares/Third_Party/SEGGER/SEGGER/SEGGER_RTT.d ./Middlewares/Third_Party/SEGGER/SEGGER/SEGGER_RTT.o ./Middlewares/Third_Party/SEGGER/SEGGER/SEGGER_RTT.su ./Middlewares/Third_Party/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.d ./Middlewares/Third_Party/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.o ./Middlewares/Third_Party/SEGGER/SEGGER/SEGGER_RTT_printf.d ./Middlewares/Third_Party/SEGGER/SEGGER/SEGGER_RTT_printf.o ./Middlewares/Third_Party/SEGGER/SEGGER/SEGGER_RTT_printf.su ./Middlewares/Third_Party/SEGGER/SEGGER/SEGGER_SYSVIEW.d ./Middlewares/Third_Party/SEGGER/SEGGER/SEGGER_SYSVIEW.o ./Middlewares/Third_Party/SEGGER/SEGGER/SEGGER_SYSVIEW.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-SEGGER-2f-SEGGER

