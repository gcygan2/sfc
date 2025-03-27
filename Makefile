CPU = atmega328p
PORT = COM5
SPEED = 115200
F_CPU = 16000000UL
TARGET = main
CC = avr-gcc
CFLAGS =  -O2 -mmcu=$(CPU) -DF_CPU=$(F_CPU)
OBJS = main.o i2c.o

$(TARGET).hex: $(TARGET).bin
	avr-objcopy -j .text -j .data -O ihex $(TARGET).bin $(TARGET).hex
$(TARGET).bin: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
program:
	avrdude -C../../avr/etc/avrdude.conf -c arduino -p $(CPU) \
	-P $(PORT) -b $(SPEED) -V -U flash:w:$(TARGET).hex
flash: program
clean:
	rm -f $(TARGET).hex $(OBJS) $(TARGET).bin
.PHONY:
	program flash clean