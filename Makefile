######################################
# target
######################################
TARGET = Low-Level

######################################
# building variables
######################################
DEBUG = 1
OPT = -Og

#######################################
# paths
#######################################
BUILD_DIR = build

######################################
# source
######################################
C_SOURCES = \
Core/Src/main.c \
lib/CMSIS/Device/Source/system_stm32l4xx.c \
COre/Src/USART.c 

ASM_SOURCES = \
lib/CMSIS/Device/Source/startup_stm32l432xx.s

ASMM_SOURCES =

#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################
CPU = -mcpu=cortex-m4
FPU = -mfpu=fpv4-sp-d16
FLOAT-ABI = -mfloat-abi=hard
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

AS_DEFS =
C_DEFS = \
-DSTM32L432xx

AS_INCLUDES =
C_INCLUDES = \
-ICore/Inc \
-Ilib/CMSIS/Core/Include \
-Ilib/CMSIS/Device/Include \

ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################
LDSCRIPT = STM32L432KCUx_FLASH.ld
LIBS = -lc -lm -lnosys
LIBDIR =
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) \
	-Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

#######################################
# build the application
#######################################
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASMM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASMM_SOURCES)))

.PHONY: all clean flash compile_commands

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin compile_commands

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

flash: $(BUILD_DIR)/$(TARGET).bin
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x8000000

#######################################
# compile_commands.json for clangd / IntelliSense
#######################################
CC_PATH := $(shell which $(CC))
PROJECT_DIR := $(abspath .)

compile_commands: compile_commands.json

compile_commands.json: Makefile
	@python3 -c 'import json, os, shlex, pathlib; \
sources = """$(C_SOURCES)""".split(); \
flags = shlex.split("""$(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections""") + (["-g", "-gdwarf-2"] if $(DEBUG) else []); \
root = pathlib.Path(r"$(PROJECT_DIR)"); \
cc = r"$(CC_PATH)"; \
entries = []; \
[entries.append({"directory": str(root), "file": str(root / src), "output": str(root / "build" / (pathlib.Path(src).stem + ".o")), "arguments": [cc, "-c", *flags, "-o", str(root / "build" / (pathlib.Path(src).stem + ".o")), str(root / src)]}) for src in sources]; \
pathlib.Path("compile_commands.json").write_text(json.dumps(entries, indent=2) + "\n"); \
print("Wrote compile_commands.json ({} entries)".format(len(entries)))'

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)
