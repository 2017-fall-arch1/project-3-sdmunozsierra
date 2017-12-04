all: myDemo.elf fallingBalls.elf

GCC_DIR = /opt/ti/msp430_gcc/bin
SUPPORT_FILE_DIRECTORY = /opt/ti/msp430_gcc/include

CPU             = msp430g2553
CFLAGS          = -I $(SUPPORT_FILE_DIRECTORY) -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L $(SUPPORT_FILE_DIRECTORY)

#switch the compiler (for the internal make rules)
CC              = $(GCC_DIR)/msp430-elf-gcc
AS              = $(GCC_DIR)/msp430-elf-as -mmcu=${CPU} -c
AR              = $(GCC_DIR)/msp430-elf-ar

myDemo.elf: myDemo.o
	$(CC) $(CFLAGS) ${LDFLAGS} -o $@ $^ -lLcd -lTimer -lShape

fallingBalls.elf: fallingBalls.o wdt_handler.o
	$(CC) $(CFLAGS) ${LDFLAGS} -o $@ $^ -lLcd -lTimer -lShape -lCircle -lp2sw

clean:
	rm -f *.a *.o *.elf

load: myDemo.elf
	mspdebug rf2500 "prog $^"

load2: fallingBalls.elf
	mspdebug rf2500 "prog $^"

