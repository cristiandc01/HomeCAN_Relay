################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
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


BUILD_ARTIFACT_NAME := HomeCAN_6chOut_Node
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
HomeCAN_6chOut_Node.elf \

MAP_FILES += \
HomeCAN_6chOut_Node.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
HomeCAN_6chOut_Node.list \

OBJCOPY_HEX += \
HomeCAN_6chOut_Node.hex \

OBJCOPY_BIN += \
HomeCAN_6chOut_Node.bin \

OBJCOPY_VERILOG += \
HomeCAN_6chOut_Node.verilog \

OBJCOPY_SREC += \
HomeCAN_6chOut_Node.srec \

OBJCOPY_SYMBOLSREC += \
HomeCAN_6chOut_Node.symbolsrec \


# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: HomeCAN_6chOut_Node.elf secondary-outputs

# Tool invocations
HomeCAN_6chOut_Node.elf HomeCAN_6chOut_Node.map: $(OBJS) $(USER_OBJS) C:\Users\crist\git\repository\HomeCAN_6chOut_Node\STM32F103C8TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "HomeCAN_6chOut_Node.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\crist\git\repository\HomeCAN_6chOut_Node\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="HomeCAN_6chOut_Node.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

HomeCAN_6chOut_Node.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "HomeCAN_6chOut_Node.list"
	@echo 'Finished building: $@'
	@echo ' '

HomeCAN_6chOut_Node.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "HomeCAN_6chOut_Node.hex"
	@echo 'Finished building: $@'
	@echo ' '

HomeCAN_6chOut_Node.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "HomeCAN_6chOut_Node.bin"
	@echo 'Finished building: $@'
	@echo ' '

HomeCAN_6chOut_Node.verilog: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O verilog $(EXECUTABLES) "HomeCAN_6chOut_Node.verilog"
	@echo 'Finished building: $@'
	@echo ' '

HomeCAN_6chOut_Node.srec: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O srec $(EXECUTABLES) "HomeCAN_6chOut_Node.srec"
	@echo 'Finished building: $@'
	@echo ' '

HomeCAN_6chOut_Node.symbolsrec: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O symbolsrec $(EXECUTABLES) "HomeCAN_6chOut_Node.symbolsrec"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) HomeCAN_6chOut_Node.bin HomeCAN_6chOut_Node.elf HomeCAN_6chOut_Node.hex HomeCAN_6chOut_Node.list HomeCAN_6chOut_Node.map HomeCAN_6chOut_Node.srec HomeCAN_6chOut_Node.symbolsrec HomeCAN_6chOut_Node.verilog default.size.stdout
	-@echo ' '

post-build:
	arm-none-eabi-objcopy -O ihex HomeCAN_6chOut_Node.elf HomeCAN_6chOut_Node.hex
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN) $(OBJCOPY_VERILOG) $(OBJCOPY_SREC) $(OBJCOPY_SYMBOLSREC)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified post-build

-include ../makefile.targets
