#!/bin/sh
gcc -c main.c
gcc -c Erlojua.c
gcc -c Loader.c
gcc -c Scheduler.c
gcc -c Timer.c
gcc main.o Erlojua.o Scheduler.o Timer.o Loader.o -o main
