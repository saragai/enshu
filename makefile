all: main

main: main.c myfunction.h
	gcc -o main -g main.c `pkg-config opencv --cflags` `pkg-config opencv --libs`
