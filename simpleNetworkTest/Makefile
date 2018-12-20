.PHONY : all

all: client server
	@echo all

client :
	g++ -Wall -g -o client -pthread client.cpp

server :
	g++ -pthread server.cpp -o server -Wall -g

