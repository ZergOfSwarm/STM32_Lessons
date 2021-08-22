################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../my_driver/Src/stm32f407xx_gpio_driver.c 

OBJS += \
./my_driver/Src/stm32f407xx_gpio_driver.o 

C_DEPS += \
./my_driver/Src/stm32f407xx_gpio_driver.d 


# Each subdirectory must supply rules for building sources it contributes
my_driver/Src/%.o: ../my_driver/Src/%.c my_driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"/home/denis/STM32CubeIDE/workspace_1.7.0/My_driver/my_driver/Inc" -I"/home/denis/STM32CubeIDE/workspace_1.7.0/My_driver/my_driver/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

