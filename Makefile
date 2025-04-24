CFLAGS = -Wall -Werror -Wpedantic

aswium-git: main.o 
	gcc $(CFLAGS) main.o

main.o: ./main.c
	gcc $(CFLAGS) -c ./main.c 