main: main.o Erlojua.o Loader.o Scheduler.o Timer.o 
	gcc main.o Erlojua.o Scheduler.o Timer.o Loader.o -o main

main.o: main.c aldagai_generalak.h Erlojua.h Loader.h Scheduler.h Timer.h
	gcc -c main.c

Erlojua.o: Erlojua.c Erlojua.h aldagai_generalak.h
	gcc -c Erlojua.c

Loader.o: Loader.c Loader.h aldagai_generalak.h Scheduler.h
	gcc -c Loader.c

Scheduler.o: Scheduler.c Scheduler.h aldagai_generalak.h
	gcc -c Scheduler.c

Timer.o: Timer.c Timer.h aldagai_generalak.h
	gcc -c Timer.c