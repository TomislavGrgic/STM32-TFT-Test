CC = arm-none-eabi-gcc

PROJECT_DIR := $(subst \,/,$(CURDIR))
CFLAGS = -g -fdebug-prefix-map=./=$(PROJECT_DIR) -mcpu=cortex-m0 -mthumb -nostdlib

CINCLUDES = -DSTM32F072xB\
	 -ICore/CMSIS/inc\
	 -ICore/ST/inc\
	 -IDrivers/inc\
	 -IModules/inc\

BUILD_PATH = .build
OBJECTS_PATH = $(BUILD_PATH)/objs

SOURCE_FILES = main.c \
	Core/ST/src/startup_stm32f072xb.s \
	Core/ST/src/system_stm32f0xx.c \
	Drivers/src/systick.c \
	Drivers/src/gpio.c \
	Drivers/src/spi1.c \
	Drivers/src/clock_config.c \
	Drivers/src/st7735.c \
	Drivers/src/tft.c \
	Drivers/src/graphics.c

OBJECT_FILES = $(addprefix $(OBJECTS_PATH)/, $(patsubst %.s, %.o, $(patsubst %.c, %.o, $(notdir $(SOURCE_FILES)))))

VPATH = . \
	$(dir $(SOURCE_FILES)) \

LINKER_FILE = Core/STM32F072RBTx_FLASH.ld
LDFLAGS =-T $(LINKER_FILE)

OPENOCD_PATH = OpenOCD
OCD = $(OPENOCD_PATH)/openocd.exe
INTERFACE = -f $(OPENOCD_PATH)/stlink.cfg
TARGET = -f $(OPENOCD_PATH)/stm32f0x.cfg
OCD_OPTIONS = -c "program $(BUILD_PATH)/firmware.elf verify reset exit"


.PHONY: all
all: clean build flash

.PHONY: build
build: $(OBJECTS_PATH) $(BUILD_PATH)/firmware.elf
	@echo "Build complete!"
	@echo "Firmware: $(BUILD_PATH)/firmware.elf"


.PHONY: clean
clean:
	@-rm -rf $(BUILD_PATH)
	@echo "Clean complete!"


.PHONY: flash
flash:
	@echo "Flashing firmware..."
	@$(OCD) $(INTERFACE) $(TARGET) $(OCD_OPTIONS)


.PHONY: build-and-flash
build-and-flash: build flash


$(BUILD_PATH)/firmware.elf: $(OBJECT_FILES)
	@$(CC) $(CFLAGS) $(CINCLUDES) $(LDFLAGS) $^ -o $@


$(OBJECTS_PATH):
	@mkdir -p $(subst \,/,$(OBJECTS_PATH))


$(OBJECTS_PATH)/%.o: %.c
	@$(CC) $(CFLAGS) $(CINCLUDES) -c $< -o $@
	@echo "Compiling $(notdir $<) to $(notdir $@)"


$(OBJECTS_PATH)/%.o: %.s
	@$(CC) $(CFLAGS) $(CINCLUDES) -c $< -o $@
	@echo "Compiling $(notdir $<) to $(notdir $@)"