################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F0xx_HAL_Driver/Src/subdir.mk
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


BUILD_ARTIFACT_NAME := Coral_stepper_control_v1_LED_tst
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Coral_stepper_control_v1_LED_tst.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Coral_stepper_control_v1_LED_tst.list \

OBJCOPY_BIN += \
Coral_stepper_control_v1_LED_tst.bin \


# All Target
all: main-build

# Main-build Target
main-build: Coral_stepper_control_v1_LED_tst.elf secondary-outputs

# Tool invocations
Coral_stepper_control_v1_LED_tst.elf: $(OBJS) $(USER_OBJS) C:\DATA\Documents\Lab\github\Coral\Firmware\Stepper_control\Coral_stepper_control_v1_LED_tst\STM32F031F6PX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Coral_stepper_control_v1_LED_tst.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0 -T"C:\DATA\Documents\Lab\github\Coral\Firmware\Stepper_control\Coral_stepper_control_v1_LED_tst\STM32F031F6PX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Coral_stepper_control_v1_LED_tst.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Coral_stepper_control_v1_LED_tst.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Coral_stepper_control_v1_LED_tst.list"
	@echo 'Finished building: $@'
	@echo ' '

Coral_stepper_control_v1_LED_tst.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Coral_stepper_control_v1_LED_tst.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Coral_stepper_control_v1_LED_tst.bin Coral_stepper_control_v1_LED_tst.elf Coral_stepper_control_v1_LED_tst.list default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
