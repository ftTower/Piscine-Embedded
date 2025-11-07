# ================================================================================
COLOR_RESET = \033[0m\n
COLOR_RED = \033[31m
COLOR_GREEN = \033[32m
COLOR_YELLOW = \033[33m
COLOR_BLUE = \033[34m
COLOR_MAGENTA = \033[35m
COLOR_CYAN = \033[36m
COLOR_WHITE = \033[37m

# ================================================================================

TARGET = main
MCU = atmega328p
F_CPU = 16000000


CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

CFLAGS = -Wall -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU)
LDFLAGS = -mmcu=$(MCU)

PROGRAMMER = arduino
PORT = /dev/ttyUSB0

all: clear hex flash clean screen

# ! HEX : COMPILATION & CONVERSION
hex: $(TARGET).hex

$(TARGET).bin: $(TARGET).c
	@echo "$(COLOR_BLUE)Compiling $< to $@ $(COLOR_RESET)"
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

$(TARGET).hex: $(TARGET).bin
	@echo "$(COLOR_BLUE)Generating $@ from $<$(COLOR_RESET)"
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# ! FLASH : MEMORY COPY

flash: $(TARGET).hex
	@echo "$(COLOR_GREEN)Flashing $< to $(MCU) via $(PROGRAMMER) on $(PORT) $(COLOR_RESET)"
	$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -U flash:w:$<:i

# ! CLEAN
clean:
	@echo "$(COLOR_RED)Cleaning up generated files: $(TARGET).hex and $(TARGET).bin$(COLOR_RESET)"
	rm -f $(TARGET).hex $(TARGET).bin

screen:
	sleep 1
	screen $(PORT) 115200

clear : 
	clear