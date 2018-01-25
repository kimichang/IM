#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define SERVER_PORT 8008
#define BUFFER_SIZE 8192
#define FILE_NAME_MAX_SIZE 512


int main()
{
	struct sockaddr_in client_addr;
	bzero(&client_addr,sizeof(client_addr));
	client_addr.sin_family = AF_INET;
//	client_addr.sin_addr.s_addr = inet_addr("192.168.10.190");
	client_addr.sin_addr.s_addr = htons(INADDR_ANY);
	client_addr.sin_port = htons(0);
//	client_addr.sin_port = htons(SERVER_PORT);

	int client_socket_fd = socket(AF_INET,SOCK_STREAM,0);

	if(client_socket_fd < 0)
	{
		perror("Create socket failed\n");
		exit(1);
	}

	if(-1 == (bind(client_socket_fd,(struct sockaddr*)&client_addr,sizeof(client_addr))))
	{
		perror("client bind failed\n");
		exit(1);
	}

	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));

	server_addr.sin_family = AF_INET;

	if(inet_pton(AF_INET,"192.168.10.190",&server_addr.sin_addr) == 0)
	{
		perror("Server ip Address Error\n");
		exit(1);
	}
	server_addr.sin_port = htons(SERVER_PORT);
	socklen_t server_addr_length = sizeof(server_addr);
	if(connect(client_socket_fd,(struct sockaddr*)&server_addr,server_addr_length) < 0)
//	if(connect(client_socket_fd,(struct sockaddr*)&client_addr,sizeof(client_addr)) < 0)
	{
		perror("can not connect to server ip\n");
		exit(1);
	}

	char file_name[FILE_NAME_MAX_SIZE +1 ];
	bzero(file_name,FILE_NAME_MAX_SIZE + 1);
	int len = recv(client_socket_fd,file_name,FILE_NAME_MAX_SIZE,0);
	FILE *fp = NULL;
	if(len > 0 )
	{
		fp = fopen(file_name,"w");
	}else
	{
		printf("can not receive file name\n");
		exit(1);
	}

	if(NULL == fp)
	{
		printf("File:%s can not open to write\n",file_name);
		exit(1);
	}
	char buffer[BUFFER_SIZE];
	bzero(buffer,BUFFER_SIZE);
	int length = 0;
	while((length = recv(client_socket_fd,buffer,BUFFER_SIZE,0)) > 0)
	{
		if(fwrite(buffer,sizeof(char),length,fp) < length)
		{
			printf("file : %s write failed\n",file_name);
			break;
		}
		bzero(buffer,BUFFER_SIZE);
	}

	printf("Received file :%s From server Successfully\n",file_name);
	close(fp);
	close(client_socket_fd);
	return 0;
}

