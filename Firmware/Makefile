TARGET := morse_keycard
TARGET_MCU := CH32V203G6U6
TARGET_MCU_PACKAGE := CH32V203G6U6
CH32FUN ?= ch32fun/ch32fun

.DEFAULT_GOAL := build

ifneq ($(wildcard $(CH32FUN)/ch32fun.mk),)
include $(CH32FUN)/ch32fun.mk

.PHONY: all flash clean
all: build
flash: cv_flash
clean: cv_clean
else
.PHONY: all build flash clean setup
all build flash:
	@echo "ch32fun is missing; run 'make setup' first."
	@false
clean:
	@rm -f $(TARGET).elf $(TARGET).bin $(TARGET)_ext.bin $(TARGET).hex \
		$(TARGET).lst $(TARGET).map
setup:
	git clone --depth 1 https://github.com/cnlohr/ch32fun.git ch32fun
endif
