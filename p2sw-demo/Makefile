all: p2sw-demo.elf

GCC_DIR = /opt/ti/msp430_gcc/bin
SUPPORT_FILE_DIRECTORY = /opt/ti/msp430_gcc/include

CPU             = msp430g2553
CFLAGS          = -I $(SUPPORT_FILE_DIRECTORY) -mmcu=${CPU} -Os -I../h
#CFLAGS          = -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L $(SUPPORT_FILE_DIRECTORY)
#LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = $(GCC_DIR)/msp430-elf-gcc
AS              = $(GCC_DIR)/msp430-elf-as
AR              = $(GCC_DIR)/msp430-elf-ar

p2sw-demo.elf: p2sw-demo.o
	$(CC) $(CFLAGS) ${LDFLAGS} -o $@ $^ -lLcd -lp2sw -lTimer

clean:
	rm -f *.a *.o *.elf

load: p2sw-demo.elf
	mspdebug rf2500 "prog $^"

