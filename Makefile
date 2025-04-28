CFLAGS = -I/home/vallislfc/libgit2/include
second = -L//home/vallislfc/libgit2/build -Wl,-rpath=/home/vallislfc/libgit2/build -lgit2

aswium: main.o
	gcc $(CLFAGS) -o aswium main.o $(second)

main.o: main.c
	gcc $(CFLAGS) -c main.c $(second)