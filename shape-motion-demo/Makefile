# makfile configuration
GCC_DIR = /opt/ti/msp430_gcc/bin
SUPPORT_FILE_DIRECTORY = /opt/ti/msp430_gcc/include

CPU             	= msp430g2553
#CFLAGS          	= -mmcu=${CPU} -Os -I../h
#LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/ 

CFLAGS          = -I $(SUPPORT_FILE_DIRECTORY) -Os -mmcu=${CPU} -I../h
LDFLAGS		= -L../lib -L $(SUPPORT_FILE_DIRECTORY)
#switch the compiler (for the internal make rules)
CC              = $(GCC_DIR)/msp430-elf-gcc
#CC              = msp430-elf-gcc
AS              = $(GCC_DIR)/msp430-elf-as -mmcu=${CPU} -c
#AS              = msp430-elf-gcc -mmcu=${CPU} -c

all:shapemotion.elf

#additional rules for files
shapemotion.elf: ${COMMON_OBJECTS} shapemotion.o wdt_handler.o buzzer.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd -lShape -lCircle -lp2sw

load: shapemotion.elf
	mspdebug rf2500 "prog $^"

clean:
	rm -f *.o *.elf
