#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>

#define PORT 8008
#define MAXDATASIZE 2048
enum status { offline,online,busy,not_disturb,transparent};

struct clients {
	char* ip;
	char* port;
	};


void main()
{
	int connectedfd,listenfd;
	struct sockaddr_in server;
	struct sockaddr_in client;
	int sin_size;
	int client_size = 100;
	struct	clients* cli[100];//= malloc(100*sizeof(struct clients));
	int real_clients = 0;
	
	if((listenfd = socket(AF_INET,SOCK_STREAM,0)) == -1) {
	perror("Creating socket error.");
	exit(1);
	}
	
	int opt = SO_REUSEADDR;
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	
	bzero(&server,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(listenfd,(struct sockaddr *)&server,sizeof(struct sockaddr)) == -1) {
	perror("bind failed.");
	exit(1);
	}

	if(listen(listenfd,client_size) == -1){
		perror("listen() error\n");
		exit(1);
	}

	sin_size = sizeof(struct sockaddr_in);
	
	while(1){
	if((connectedfd = accept(listenfd,(struct sockaddr*)&client,(socklen_t *)&sin_size)) == -1){
	perror("accept() error\n");
	exit(1);
	}
	else{
		cli[real_clients]->port = client.sin_port;
		cli[real_clients]->ip = client.sin_addr.s_addr;
		real_clients ++;
	}

	}
	close(listenfd);
}
