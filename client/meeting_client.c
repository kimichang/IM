#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

int sockfd;
char* ip ="192.168.10.190";
short PORT = 8888;
typedef struct sockaddr SA;
char name[30];

void init()
{
		sockfd = socket(AF_INET,SOCK_STREAM,0);
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(PORT);
		addr.sin_addr.s_addr = inet_addr(ip);
		if(connect(sockfd,(SA*)&addr,sizeof(addr)) == -1)
		{
				perror("connect error");
				exit(-1);
		}

		printf("connect successed\n");
}

void start()
{
		pthread_t id;
		void* recv_thread(void*);
		pthread_create(&id,0,recv_thread,0);
		char buf2[100] = {};
		sprintf(buf2,"%s join in ",name);
		send(sockfd,buf2,strlen(buf2),0);
		while(1)
		{
				char buf[100] = {};
				scanf("%s",buf);
				char msg[131] = {};
				sprintf(msg,"%s:%s",name,buf);
				send(sockfd,msg,strlen(msg),0);
				if(strcmp(buf,"bye") == 0 )
				{
						memset(buf2,0,sizeof(buf2));
						sprintf(buf2,"%s leaving",name);
						send(sockfd,buf2,strlen(buf2),0);
						break;
				}
		}
		close(sockfd);
}

void* recv_thread(void * p)
{
		while(1)
		{
				char buf[100] = {};
				if(recv(sockfd,buf,sizeof(buf),0) <= 0)
						return;

				printf("%s\n",buf);
		}
}
int main()
{
		init();
		printf("input your name:\n");
		scanf("%s",name);
		start();
		return 0;
}

