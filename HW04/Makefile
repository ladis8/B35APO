CC=gcc
#CC=arm-linux-gnueabihf-gcc
#CC=mips-elf-gcc
CFLAGS=-g -std=c99 -pedantic -Wall

%.o:%.c
	$(CC) $(CFLAGS) -c $<

all: program

program: main.o  subroutine.o  toplevel.o
	$(CC) $(CFLAGS) $(LDFLAGS) -L. $^ -o $@


.PHONY : depend dep all

dep:
	$(CC) $(CFLAGS) $(CPPFLAGS) -w -E -M *.c \
	  > depend

depend:
	@touch depend

clean:
	rm -f *.o *.a program depend

-include depend
