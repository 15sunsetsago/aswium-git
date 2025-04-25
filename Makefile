CFLAGS = -Wall -Werror -Wpedantic -I/usr/include/openssl 
second = -lssl -lcrypto

aswium-git: main.o server.o
	gcc $(CFLAGS) main.o server.o $(second)

main.o: ./main.c
	gcc $(CFLAGS) -c ./main.c 

server.o: ./server.c 
	gcc $(CFLAGS) -c ./server.c