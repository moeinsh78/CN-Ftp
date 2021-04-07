CC := g++ -std=c++11 -pthread
all: client.out

client.out: main.o client.o 
	$(CC) -o client.out main.o client.o 

client.o: client.cpp client.hpp
	$(CC) -c client.cpp -o client.o

main.o: main.cpp client.hpp 
	$(CC) -c main.cpp -o main.o

.PHONY: clean
clean:
	rm *.o
	rm client.out