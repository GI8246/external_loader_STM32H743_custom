################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Drivers/STM32H7xx_HAL_Driver/Src/subdir.mk
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


BUILD_ARTIFACT_NAME := external_loader_STM32H743_custom
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
external_loader_STM32H743_custom.elf \

MAP_FILES += \
external_loader_STM32H743_custom.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
external_loader_STM32H743_custom.list \

OBJCOPY_BIN += \
external_loader_STM32H743_custom.bin \


# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: external_loader_STM32H743_custom.elf secondary-outputs

# Tool invocations
external_loader_STM32H743_custom.elf external_loader_STM32H743_custom.map: $(OBJS) $(USER_OBJS) D:\work_STM32_CUBE_IDE\external\ loader\external_loader_STM32H743_custom\STM32H743IITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "external_loader_STM32H743_custom.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"D:\work_STM32_CUBE_IDE\external loader\external_loader_STM32H743_custom\STM32H743IITX_FLASH.ld" -Wl,-Map="external_loader_STM32H743_custom.map" -Wl,--cref -nostartfiles -nodefaultlibs -nostdlib -static -u,Reset_Handler,--specs=nosys.specs --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

external_loader_STM32H743_custom.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "external_loader_STM32H743_custom.list"
	@echo 'Finished building: $@'
	@echo ' '

external_loader_STM32H743_custom.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "external_loader_STM32H743_custom.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout external_loader_STM32H743_custom.bin external_loader_STM32H743_custom.elf external_loader_STM32H743_custom.list external_loader_STM32H743_custom.map
	-@echo ' '

post-build:
	cmd.exe /C copy/Y  "external_loader_STM32H743_custom.elf" "external_loader_SMU_DISP_H743.stldr"
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified post-build

-include ../makefile.targets
