################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/Third_Party/LwIP/src/netif/ppp/subdir.mk
-include Middlewares/Third_Party/LwIP/src/netif/subdir.mk
-include Middlewares/Third_Party/LwIP/src/core/ipv6/subdir.mk
-include Middlewares/Third_Party/LwIP/src/core/ipv4/subdir.mk
-include Middlewares/Third_Party/LwIP/src/core/subdir.mk
-include Middlewares/Third_Party/LwIP/src/apps/mqtt/subdir.mk
-include Middlewares/Third_Party/LwIP/src/api/subdir.mk
-include LWIP/Target/subdir.mk
-include LWIP/App/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := UDP_server
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
UDP_server.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
UDP_server.list \

OBJCOPY_BIN += \
UDP_server.bin \


# All Target
all: main-build

# Main-build Target
main-build: UDP_server.elf secondary-outputs

# Tool invocations
UDP_server.elf: $(OBJS) $(USER_OBJS) /home/denis/STM32CubeIDE/workspace_1.8.0/UDP_server/STM32F407VGTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "UDP_server.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/home/denis/STM32CubeIDE/workspace_1.8.0/UDP_server/STM32F407VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="UDP_server.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

UDP_server.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "UDP_server.list"
	@echo 'Finished building: $@'
	@echo ' '

UDP_server.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "UDP_server.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) UDP_server.bin UDP_server.elf UDP_server.list default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
