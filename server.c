
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include "echo.h"

int main(int argc,char *argv[])
{
	int sockfd,msg_flags;
	char readbuf[BUFFSIZE];
	char *listenq;
	struct sockaddr_in servaddr,cliaddr;
	struct sctp_sndrcvinfo sri;
	struct sctp_event_subscribe events;
	int stream_increment = 1;
	socklen_t len;
	size_t rd_sz;

	if (argc == 2)
		stream_increment = atoi(argv[1]);
	sockfd = socket(AF_INET,SOCK_SEQPACKET,IPPROTO_SCTP);
	if (sockfd == -1){
		perror("socket");
		return 1;
	}
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	if (bind(sockfd,(struct sockaddr *) &servaddr,sizeof(servaddr)) == -1){
		perror("bind");
		return 1;
	}
	events.sctp_data_io_event = 1;
	if (setsockopt(sockfd,IPPROTO_SCTP,SCTP_EVENTS,&events,sizeof(events)) == -1){
		perror("setsockopt");
		return 1;
	}

	listenq = getenv("LISTENQ");

	if (listen(sockfd,listenq!=NULL?atoi(listenq):1) == -1){
		perror("listen");
		return 1;
	}

	for (;;)
	{
	}
}
