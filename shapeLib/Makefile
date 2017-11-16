all: libShape.a shapedemo.elf shapedemo2.elf shapedemo3.elf
GCC_DIR = /opt/ti/msp430_gcc/bin
SUPPORT_FILE_DIRECTORY = /opt/ti/msp430_gcc/include

CPU             = msp430g2553
CFLAGS          = -I $(SUPPORT_FILE_DIRECTORY) -Os -mmcu=${CPU} -I../h
LDFLAGS		= -L../lib -L $(SUPPORT_FILE_DIRECTORY)
#CFLAGS          = -mmcu=${CPU} -Os -I../h 
#LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/

#switch the compiler (for the internal make rules)
CC              = $(GCC_DIR)/msp430-elf-gcc
AS              = $(GCC_DIR)/msp430-elf-as
AR              = $(GCC_DIR)/msp430-elf-ar

OBJECTS         = shape.o region.o rect.o vec2.o layer.o rarrow.o

libShape.a: $(OBJECTS)
	$(AR) crs $@ $^

$(OBJECTS): shape.h

install: libShape.a
	mkdir -p ../h ../lib
	mv $^ ../lib
	cp *.h ../h

clean:
	rm -f libShape.a *.o *.elf

shapedemo.elf: shapedemo.o libShape.a 
	$(CC) $(CFLAGS) ${LDFLAGS} $^ -L../lib -lTimer -lLcd -o $@

shapedemo2.elf: shapedemo2.o libShape.a 
	$(CC) $(CFLAGS) ${LDFLAGS} $^ -L../lib -lTimer -lLcd -o $@

shapedemo3.elf: shapedemo3.o libShape.a 
	$(CC) $(CFLAGS) ${LDFLAGS} $^ -L../lib -lTimer -lLcd -o $@

load: shapedemo.elf
	mspdebug rf2500 "prog $^"

load2: shapedemo2.elf
	mspdebug rf2500 "prog $^"

load3: shapedemo3.elf
	mspdebug rf2500 "prog $^"
