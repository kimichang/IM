#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_PORT 8008
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 8192
#define FILE_NAME_MAX_SIZE 512

int main()
{
	int listenfd;
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(SERVER_PORT);
	
	int server_socket_fd = socket(PF_INET,SOCK_STREAM,0);
	if(server_socket_fd < 0 )
	{
		perror("Create socket failed\n");
		exit(-1);
	}
	
	int opt = 1;
	setsockopt(server_socket_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	
	if(-1 == (bind(server_socket_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))))
	{
		perror("server bind failed\n");
		exit(-1);
	}
	if(-1 == listen(server_socket_fd,LENGTH_OF_LISTEN_QUEUE))
	{
		perror("Server listen failed\n");
		exit(1);
	}

	while(1)
	{
		struct sockaddr_in client_addr;
		socklen_t client_addr_length = sizeof(client_addr);
		int new_server_socket_fd = accept(server_socket_fd,(struct sockaddr*)&client_addr,&client_addr_length);
	if(new_server_socket_fd < 0)
	{
		perror("Server Accept failed\n");
		exit(-1);
	}
	
	char buffer[BUFFER_SIZE];
	bzero(buffer,BUFFER_SIZE);
/*	if(recv(new_server_socket_fd,buffer,BUFFER_SIZE,0) < 0 )
	{
		perror("Server Receive Data failed\n");
		exit(-1);
	}
*/
	char filename[FILE_NAME_MAX_SIZE+1];
	bzero(filename,FILE_NAME_MAX_SIZE +1 );
	printf("please input filename:\n");
	scanf("%s",filename);
	strncpy(filename,buffer,strlen(buffer)>FILE_NAME_MAX_SIZE?FILE_NAME_MAX_SIZE:strlen(buffer));
	printf("%s\n",filename);

	FILE *fp = fopen(filename,"r");
	if(NULL == fp)
	{
		printf("File: %s Not Found\n",filename);
	}
	else
	{	int len = send(new_server_socket_fd,filename,strlen(filename),0);
		if(len <= 0)
			continue;
		
		bzero(buffer,BUFFER_SIZE);
		int length = 0;
		while((length = fread(buffer,sizeof(char),BUFFER_SIZE,fp))> 0)
		{
			if(send(new_server_socket_fd,buffer,length,0) < 0)
			{
				printf("Send file : %s failed\n",filename);
				break;
			}
			bzero(buffer,BUFFER_SIZE);
		}
		fclose(fp);
		printf("File: %s Transfer successful\n",filename);
	}
	close(new_server_socket_fd);
	}
	close(server_socket_fd);
	return 0;
}
	
