/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "9999" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
	int sockfd, numbytes;  
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];
    char message[1024];
    fd_set master;
    fd_set read_fds;

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

	if (argc != 2) {
	    fprintf(stderr,"usage: client hostname\n");
	    exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			perror("client: connect");
			close(sockfd);
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);
	printf("client: connecting to %s\n", s);

	freeaddrinfo(servinfo); // all done with this structure



    FD_SET(STDIN_FILENO,&master);
    FD_SET(sockfd,&master); // s is a socket descriptor
    int maxfd = STDIN_FILENO;
    if(STDIN_FILENO > sockfd){
        maxfd = STDIN_FILENO;
    } else {
        maxfd = sockfd;
    }

    for(;;){
        read_fds = master;
        if (select(maxfd+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }
        for(int i = 0; i <= maxfd; i++) {
            if (FD_ISSET(i, &read_fds)) { // we got information
                if (i == sockfd) {
                    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
                        perror("recv");
                        exit(1);
                    }
                    buf[numbytes] = '\0';
                    printf("client: received: %s \n",buf);
                } else if(i == STDIN_FILENO){
                    memset(buf,0x00,MAXDATASIZE);
                    if (! fgets(buf, sizeof buf,stdin)) {
                        if (ferror(stdin)) {
                            perror("stdin");
                            exit(1);
                        }
                    }
                    if (send(sockfd, buf, strlen(buf), 0) == -1) {
                        perror("send");
                    }
                }
            }
        }
    }

	close(sockfd);

	return 0;
}
