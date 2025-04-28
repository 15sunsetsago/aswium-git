CFLAGS = -Wall -Werror -Wpedantic -I/usr/include/openssl -I/home/vallislfc/Downloads/libgit2-1.9.0/include
second = -lssl -lcrypto

aswium-git: main.o server.o
	gcc $(CFLAGS) main.o server.o $(second)

main.o: ./main.c
	gcc $(CFLAGS) -c ./main.c 

server.o: ./server.c 
	gcc $(CFLAGS) -c ./server.c