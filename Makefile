BOARD ?= redv_redboard
BUILD_DIR ?= build/$(BOARD)
TARGET ?= fe310_cli

TOOLCHAIN_PREFIX ?= riscv64-unknown-elf-
CC := $(TOOLCHAIN_PREFIX)gcc
OBJCOPY := $(TOOLCHAIN_PREFIX)objcopy
OBJDUMP := $(TOOLCHAIN_PREFIX)objdump
SIZE := $(TOOLCHAIN_PREFIX)size
GDB := $(TOOLCHAIN_PREFIX)gdb

PORT ?= /dev/ttyACM0
BAUD ?= 115200
CPU_HZ ?= 16000000
LINKER_SCRIPT ?= linker/fe310_flash.ld
OPENOCD ?= openocd
OPENOCD_CFG ?= openocd/redv_redboard.cfg
JLINK ?= JLinkExe
JLINK_DEVICE ?= FE310
JLINK_SPEED ?= 4000

ARCH_FLAGS ?= -march=rv32imac_zicsr -mabi=ilp32 -mcmodel=medany
COMMON_FLAGS := $(ARCH_FLAGS) -Os -g3 -ffunction-sections -fdata-sections \
	-fno-common -Wall -Wextra -Wshadow -Wconversion \
	-DBOARD_$(shell echo $(BOARD) | tr a-z A-Z)=1 \
	-DBOARD_CPU_HZ=$(CPU_HZ)UL \
	-DBOARD_UART_BAUD=$(BAUD)UL
CFLAGS := $(COMMON_FLAGS) -std=c11 \
	-Iinclude \
	-Ithird_party/embedded-cli/lib/include
ASFLAGS := $(COMMON_FLAGS)
LDFLAGS := $(ARCH_FLAGS) -nostartfiles -Wl,--gc-sections \
	-T$(LINKER_SCRIPT) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map
LDLIBS := -lc -lgcc -lnosys

SRC_C := $(shell find src tests startup third_party/embedded-cli/lib/src -name '*.c' | sort)
SRC_S := $(shell find startup -name '*.S' | sort)
OBJ_C := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC_C))
OBJ_S := $(patsubst %.S,$(BUILD_DIR)/%.o,$(SRC_S))
OBJS := $(OBJ_S) $(OBJ_C)

.PHONY: all clean flash debug cli list-tests help

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).lst

$(BUILD_DIR)/$(TARGET).elf: $(OBJS) $(LINKER_SCRIPT)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@
	$(SIZE) $@

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(BUILD_DIR)/$(TARGET).lst: $(BUILD_DIR)/$(TARGET).elf
	$(OBJDUMP) -d -S $< > $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) -MMD -MP -c $< -o $@

flash: $(BUILD_DIR)/$(TARGET).elf
	OPENOCD="$(OPENOCD)" OPENOCD_CFG="$(OPENOCD_CFG)" scripts/flash_openocd.sh $(BUILD_DIR)/$(TARGET).elf

flash-jlink: $(BUILD_DIR)/$(TARGET).elf
	JLINK="$(JLINK)" JLINK_DEVICE="$(JLINK_DEVICE)" JLINK_SPEED="$(JLINK_SPEED)" scripts/flash_jlink.sh $(BUILD_DIR)/$(TARGET).elf

debug: $(BUILD_DIR)/$(TARGET).elf
	OPENOCD="$(OPENOCD)" OPENOCD_CFG="$(OPENOCD_CFG)" scripts/gdb_openocd.sh $(BUILD_DIR)/$(TARGET).elf

cli:
	python3 scripts/serial_cli.py --port $(PORT) --baud $(BAUD)

list-tests:
	@grep -Rho '"[a-z0-9_.-]*", ".*", test_[a-z0-9_]*' tests src | sort || true

clean:
	rm -rf build

help:
	@echo "Targets:"
	@echo "  make                         Build $(TARGET) for $(BOARD)"
	@echo "  make flash                   Flash with OpenOCD"
	@echo "  make flash-jlink             Flash with SEGGER J-Link Commander"
	@echo "  make debug                   Start GDB against OpenOCD"
	@echo "  make cli PORT=<port>         Open UART CLI"
	@echo "  make clean                   Remove build outputs"
	@echo
	@echo "Variables:"
	@echo "  TOOLCHAIN_PREFIX=riscv64-unknown-elf-"
	@echo "  BOARD=redv_redboard"
	@echo "  CPU_HZ=16000000"
	@echo "  BAUD=115200"
	@echo "  OPENOCD=openocd"
	@echo "  OPENOCD_CFG=openocd/redv_redboard.cfg"
	@echo "  JLINK=JLinkExe"
	@echo "  JLINK_DEVICE=FE310"
	@echo "  JLINK_SPEED=4000"

-include $(OBJS:.o=.d)
