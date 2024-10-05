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


BUILD_ARTIFACT_NAME := ExternalMemoryLoader
BUILD_ARTIFACT_EXTENSION := stldr
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
ExternalMemoryLoader.stldr \

MAP_FILES += \
ExternalMemoryLoader.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
ExternalMemoryLoader.list \


# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: ExternalMemoryLoader.stldr secondary-outputs

# Tool invocations
ExternalMemoryLoader.stldr ExternalMemoryLoader.map: $(OBJS) $(USER_OBJS) M:\11_work_STM32CubeIDE\ExternalMemoryLoader\STM32H743IITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "ExternalMemoryLoader.stldr" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"M:\11_work_STM32CubeIDE\ExternalMemoryLoader\STM32H743IITX_FLASH.ld" -Wl,-Map="ExternalMemoryLoader.map" -Wl,--cref -nostartfiles -nodefaultlibs -nostdlib -static -Wl,-cref,-u,Reset_Handler, --entry=Init --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

ExternalMemoryLoader.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "ExternalMemoryLoader.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) ExternalMemoryLoader.list ExternalMemoryLoader.map ExternalMemoryLoader.stldr default.size.stdout
	-@echo ' '

post-build:
	cmd.exe /C copy/Y  "ExternalMemoryLoader.elf" "..\QSPI_flashloader_CSP.stldr"
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified post-build

-include ../makefile.targets
