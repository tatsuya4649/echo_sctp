
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>
#include "echo.h"

void sctpstr_cli(FILE *fp,int sockfd,struct sockaddr *to,socklen_t tolen)
{
	struct sockaddr_in peeraddr;
	struct sctp_sndrcvinfo sri;
	char sendline[MAXLINE],recvline[MAXLINE];
	socklen_t len;
	int out_sz,rd_sz;
	int msg_flags;

	memset(&sri,0,sizeof(sri));
	while(fgets(sendline,MAXLINE,fp) != NULL){
		if (sendline[0] != '['){
			fprintf(stderr,"Error, line must be of the form '[streamnum] text'\n");
			continue;
		}
		sri.sinfo_stream = strtol(&sendline[1],NULL,0);
		out_sz = strlen(sendline);
		if (sctp_sendmsg(sockfd,sendline,out_sz,to,tolen,0,0,sri.sinfo_stream,0,0) == -1){
			perror("sctp_sendmsg");
			continue;
		}
		len = sizeof(peeraddr);
		rd_sz = sctp_recvmsg(sockfd,recvline,sizeof(recvline),(struct sockaddr *) &peeraddr,&len,&sri,&msg_flags);
		fprintf(stderr,"From str: %d seq: %d (assoc:0x%x):",sri.sinfo_stream,sri.sinfo_ssn,(u_int) sri.sinfo_assoc_id);
		fprintf(stderr,"%.*s",rd_sz,recvline);
	}
}

int main(int argc,char *argv[])
{
	int sockfd;
	struct sockaddr_in servaddr;
	struct sctp_event_subscribe events;
	int echo_to_all = 0;
	if (argc<2){
		fprintf(stderr,"Usage: %s host [echo]\n",argv[0]);
		return 1;
	}
	if (argc>2){
		fprintf(stderr,"Echoing message to all streams\n");
		echo_to_all = 1;
	}

	sockfd = socket(AF_INET,SOCK_SEQPACKET,IPPROTO_SCTP);
	if (sockfd == -1){
		perror("socket");
		return 1;
	}

	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (inet_pton(AF_INET,argv[1],&servaddr.sin_addr) == -1){
		perror("inet_pton");
		return 1;
	}

	memset(&events,0,sizeof(events));
	events.sctp_data_io_event = 1;
	if (setsockopt(sockfd,IPPROTO_SCTP,SCTP_EVENTS,&events,sizeof(events)) == -1){
		perror("setsockopt");
		return 1;
	}

	if (echo_to_all == 0){
		sctpstr_cli(stdin,sockfd,(struct sockaddr *) &servaddr,sizeof(servaddr));
	}else{
	}

	if (close(sockfd) == -1){
		perror("close");
		return 1;
	}

	exit(0);
}
