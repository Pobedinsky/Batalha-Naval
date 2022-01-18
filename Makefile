compilador=gcc
SOURCEFILES = $(wildcard *.c)
OBJECTS = $(SOURCEFILES: %.c=%.o)
main_execute = main.exe
.PHONY: all clean execute

all: execute
execute : main.exe
	./main.exe

main.exe : $(OBJECTS)
	$(compilador) -o $@ $^ 

%.o : %.c
	$(compilador) -c $<

clean :
	rm *.o
