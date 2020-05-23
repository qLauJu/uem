# Nome do programa principal
PROG = main

# Defina uma variável DEPS com a lista das dependências .c (sem a extensão)
# Exemplo: DEPS = dep1 dep2 dep3
DEPS = uart pin dht11

# Porta de comunicação com o Arduino
PORT = /dev/cu.usbmodem14101

# Escolha uma otimização dentre as seguintes:
# -O0 -O1 -O2 -O3 -Os
OPTIMIZE = -Os -flto

# Configuração para o Arduino UNO
# -------------------------------
# Altere apenas se for utilizar outro modelo de Arduino
# Consulte o arquivo /arduino/avr/boards.txt para outras configurações
MCU_TARGET = atmega328p
CPU_FREQ = 16000000
UPLOAD_SPEED = 115200
UPLOAD_PROTOCOL = arduino

# Local do arquivo de configuração do avrdude
AVRDUDE_CONF = /Applications/Arduino.app/Contents/Java/hardware/tools/avr/etc/avrdude.conf

# ==========================================
# Não é necessário alterar as regras abaixo.
# ==========================================

OBJS    = $(PROG).o $(DEPS:=.o)
CC      = avr-gcc
AS      = avr-as
OBJCOPY = avr-objcopy
CFLAGS  = $(OPTIMIZE) -Iinclude -g -Wall -Wextra -mmcu=$(MCU_TARGET) -DF_CPU=$(CPU_FREQ)

.PHONY: all install clean

all: $(PROG).hex

test:
	@echo $(OBJS)

$(PROG).elf: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(PROG).hex: $(PROG).elf
	$(OBJCOPY) -O ihex $< $@

$(PROG).eep.hex: $(PROG).elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 --no-change-warnings -O ihex $< $@ \
	|| { echo empty $@ not generated; exit 0; }

install: $(PROG).hex $(PROG).eep.hex
	avrdude -C $(AVRDUDE_CONF) -p $(MCU_TARGET) -c $(UPLOAD_PROTOCOL) -P $(PORT) -b $(UPLOAD_SPEED) -D -U flash:w:$<:i -U eeprom:w:$(word 2,$^):i

clean:
	@rm -f $(PROG).elf $(PROG).eep.hex $(PROG).hex $(OBJS)
