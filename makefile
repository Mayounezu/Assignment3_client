CXX := g++
CFLAGS := -c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS := -lboost_system -lpthread

SRC_DIR := src
BIN_DIR := bin

all: StompEMIClient

StompEMIClient: bin/ConnectionHandler.o bin/Client.o bin/Command.o bin/StompProtocol.o bin/event.o bin/Subscriptions.o bin/StompClient.o
	g++ -o bin/StompEMIClient bin/ConnectionHandler.o bin/Client.o bin/Command.o bin/StompProtocol.o bin/event.o bin/Subscriptions.o bin/StompClient.o $(LDFLAGS)

bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/Client.o: src/Client.cpp
	g++ $(CFLAGS) -o bin/Client.o src/Client.cpp

bin/Command.o: src/Command.cpp
	g++ $(CFLAGS) -o bin/Command.o src/Command.cpp

bin/StompProtocol.o: src/StompProtocol.cpp
	g++ $(CFLAGS) -o bin/StompProtocol.o src/StompProtocol.cpp

bin/event.o: src/event.cpp
	g++ $(CFLAGS) -o bin/event.o src/event.cpp

bin/Subscriptions.o: src/Subscriptions.cpp
	g++ $(CFLAGS) -o bin/Subscriptions.o src/Subscriptions.cpp

bin/StompClient.o: src/StompClient.cpp
	g++ $(CFLAGS) -o bin/StompClient.o src/StompClient.cpp

.PHONY: clean
clean:
	rm -f bin/*
