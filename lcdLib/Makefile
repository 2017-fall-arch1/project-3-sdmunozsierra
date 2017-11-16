all: libLcd.a lcddemo.elf

GCC_DIR = /opt/ti/msp430_gcc/bin
SUPPORT_FILE_DIRECTORY = /opt/ti/msp430_gcc/include

CPU             = msp430g2553
CFLAGS          = -I $(SUPPORT_FILE_DIRECTORY) -mmcu=${CPU} -Os -I../h
#CFLAGS          = -mmcu=${CPU} -Os -I../h 
LDFLAGS		= -L $(SUPPORT_FILE_DIRECTORY) -L../lib
#LDFLAGS 	= -L/opt/ti/msp430_gcc/include -L../lib 
#switch the compiler (for the internal make rules)
CC              = $(GCC_DIR)/msp430-elf-gcc
AS              = $(GCC_DIR)/msp430-elf-as
AR              = $(GCC_DIR)/msp430-elf-ar

libLcd.a: font-11x16.o font-5x7.o font-8x12.o lcdutils.o lcddraw.o
	$(AR) crs $@ $^

lcddraw.o: lcddraw.c lcddraw.h lcdutils.h
lcdutils.o: lcdutils.c lcdutils.h

install: libLcd.a
	mkdir -p ../h ../lib
	mv $^ ../lib
	cp *.h ../h

clean:
	rm -f libLcd.a *.o *.elf

lcddemo.elf: lcddemo.o libLcd.a 
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -lTimer 

load: lcddemo.elf
	mspdebug rf2500 "prog $^"
