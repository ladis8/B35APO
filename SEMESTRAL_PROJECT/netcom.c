#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include "netcom.h"

#define BIND_PORT 44444 

int initCommunication(){
	struct sockaddr_in bindaddr;
	
		
	int sockfd = 0;

   	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        	perror("socket failed");
        	return 0;
    	}
	
	
	memset(&bindaddr, 0, sizeof(bindaddr));
	bindaddr.sin_family = AF_INET;
	bindaddr.sin_port = htons(BIND_PORT);
	bindaddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr*)&bindaddr, sizeof(bindaddr)) == -1) {
		perror("bind");
		return 0;
	}
	
	int yes=1;

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
		perror("setsockopt (SO_REUSEADDR)");
		return 0;
	}
	
	int broadcast = 1;

    	if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast) == -1) {
        	perror("setsockopt (SO_BROADCAST)");
        	return 0;
    }
    
	return sockfd;
}



int receiveBytes(int SOCKET, char* buffer, char* address){
		
	struct sockaddr_storage sender;
	
	int senderlen = sizeof(sender);	//size of senders address length
	
	//max len 255
	int len = recvfrom(SOCKET, buffer, 256-1, 0, (struct sockaddr*) &sender, &senderlen);
	senderlen= sizeof (sender);

	if (len < 0){
		printf("ERROR\n");
		return -1;
	}
	buffer[len-1] = 0;
	printf("OK received len %d content %s\n",len,buffer);
	inet_ntop(sender.ss_family, &(((struct sockaddr_in *)&sender)->sin_addr),address, INET_ADDRSTRLEN);
	
	return len;
}




