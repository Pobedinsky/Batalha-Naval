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

perfilagem: gmon.out
        cat gmon.out > profile.txt
        rm gmon.out

gmon.out: $(OBJECTS)
        $(compilador) -o main.exe $^ -pg
        ./main.exe
        gprof main.exe gmon.out
        rm main.exe

test: $(OBJECTS)
        gcc -o demo batalha.c -Wall -std=c99
        rm demo

clean :
        rm *.o

