main: main.o Erlojua.o Loader.o Scheduler.o Timer.o 
	gcc main.o Erlojua.o Scheduler.o Timer.o Loader.o -o main

main.o: main.c ../include/aldagai_generalak.h ../include/Erlojua.h ../include/Loader.h ../include/Scheduler.h ../include/Timer.h
	gcc -c main.c

Erlojua.o: Erlojua.c ../include/Erlojua.h ../include/aldagai_generalak.h
	gcc -c Erlojua.c

Loader.o: Loader.c ../include/Loader.h ../include/aldagai_generalak.h ../include/Scheduler.h
	gcc -c Loader.c

Scheduler.o: Scheduler.c ../include/Scheduler.h ../include/aldagai_generalak.h
	gcc -c Scheduler.c

Timer.o: Timer.c ../include/Timer.h ../include/aldagai_generalak.h
	gcc -c Timer.c
clean:
	rm *.o main