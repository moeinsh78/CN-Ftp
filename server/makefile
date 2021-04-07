CC := g++ -std=c++11 -pthread
all: server.out

server.out: main.o commandhandler.o server.o user.o login.o 
	$(CC) -o server.out main.o commandhandler.o server.o user.o login.o

user.o: user.cpp user.hpp
	$(CC) -c user.cpp -o user.o

login.o: login.cpp login.hpp
	$(CC) -c login.cpp -o login.o

commandhandler.o: commandhandler.cpp commandhandler.hpp
	$(CC) -c commandhandler.cpp -o commandhandler.o

server.o: server.cpp server.hpp
	$(CC) -c server.cpp -o server.o

main.o: main.cpp server.hpp 
	$(CC) -c main.cpp -o main.o

.PHONY: clean
clean:
	rm *.o
	rm server.out
	rm *.log