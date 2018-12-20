.PHONY : all clean selectserver selectclient client server

all: client server selectserver selectclient
	@echo all

client :
	g++ -Wall -g -o client -pthread client.cpp

server :
	g++ -pthread server.cpp -o server -Wall -g

selectserver:
	g++ -pthread selectserver.cpp -o selectserver -Wall -g

selectclient:
	g++ -pthread selectclient.cpp -o selectclient -Wall -g

clean:
	rm client server selectclient selectserver

