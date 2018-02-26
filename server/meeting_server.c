#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

int sockfd;
int fds[100];
int size = 100;
char* ip = "192.168.10.190";
short PORT = 8888;
typedef struct sockaddr SA;

void init(){
		sockfd = socket(AF_INET,SOCK_STREAM,0);
		if(sockfd == -1)
		{
				perror("shibai");
				exit(-1);
		}

		struct sockaddr_in addr;
		addr.sin_addr.s_addr = inet_addr(ip);
		addr.sin_port = htons(PORT);
		addr.sin_family = AF_INET;
		if(bind(sockfd,(SA*)&addr,sizeof(addr)) == -1)
		{	
				perror("bind error");
				exit(-1);
		}

		if(listen(sockfd,100) == -1)
		{
				perror("shezhi shibai");
				exit(-1);
		}

	}
void SendMsgToAll(char* msg)
{
		int i;
		for(i = 0; i < size; i++)
		{
				if(fds[i] != 0){
						printf("sendto %d\n",fds[i]);
						send(fds[i],msg,strlen(msg),0);
				}
		}
}

void * service_thread(void* p )
{
		int fd = *(int*)p;
		printf("pthread = %d\n",fd);
		while(1){
		char buf[100] = {};
		if(recv(fd,buf,sizeof(buf),0) <= 0)
		{
				int i;
				for(i = 0; i < size; i++)
				{
						if(fd == fds[i])
						{
								fds[i] = 0;
								break;
						}
				}
				printf("tuichu fd= %d quit\n",fd);
				pthread_exit((void*)i);
		}
		SendMsgToAll(buf);
		}
}

void service()
{
		printf("server starting");
		while(1)
		{
				struct sockaddr_in fromaddr;
				socklen_t len = sizeof(fromaddr);
				int fd = accept(sockfd,(SA*)&fromaddr,&len);
				if(fd == -1)
				{
						printf("client connecting error");
						continue;
				}

				int i = 0;
				for(i = 0; i < size; i++)
				{
						if(fds[i] == 0 )
						{
								fds[i] = fd;
								printf("fd = %d\n",fd);
								pthread_t tid;
								pthread_create(&tid,0,service_thread,&fd);
								break;
						}
						if(size == i)
						{
								char* str = " server overloaded";
								send(fd,str,strlen(str),0);
								close(fd);
						}
				}
		}
}
int main()
{
		init();
		service();
}

		
					
