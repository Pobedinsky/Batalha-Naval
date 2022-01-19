compilador=gcc
SOURCEFILES = $(wildcard *.c)
OBJECTS = $(SOURCEFILES: %.c=%.o)
main_execute = main.exe
.PHONY: all clean execute test

all: execute
execute : main.exe
	./main.exe

main.exe : $(OBJECTS)
	$(compilador) -o $@ $^ 

%.o : %.c
	$(compilador) -c $<

perfilagem: $(OBJECTS)
	$(compilador) -o $@ $^ -pg
	./main.exe
	gprof main.exe gmon.out 
	cat gmon.out
test: $(OBJECTS)
	gcc -o demo batalha.c -Wall -std=c99

clean :
	rm *.o
