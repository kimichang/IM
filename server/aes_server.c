#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "aes_options.h"



int main(){
	int server_fd;
	int client_fd;
	int len;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int sin_size;
	char buffer[BUFSIZ];
	memset(&server_addr,0,sizeof(server_addr));
	memset(&client_addr,0,sizeof(client_addr));
	

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(8008);
	
	if((server_fd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		perror("socket create failed\n");
		return -1;
	}
	
	if(bind(server_fd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr)) < 0)
	{
		perror("bind failed\n");
		return -1;
	}
	
	listen(server_fd,100);

	sin_size = sizeof(struct sockaddr_in);

	if((client_fd = accept(server_fd,(struct sockaddr *)&client_addr,&sin_size)) < 0)
	{
		perror("accept failed\n");
		return -1;
	}

	printf("accept client \n",inet_ntoa(client_addr.sin_addr));

//	len = send(client_fd,"Welcome to my server\n",21,0);

	while((len =recv(client_fd,buffer,BUFSIZ,0)) > 0)
	{
		char *decryto_string = NULL;
		decrypt(buffer,&decryto_string,len);
		printf("origin string:\n");
		printf("%s\n",decryto_string);
		if(send(client_fd,decryto_string,len,0) < 0) 
			{
				perror("send failed \n");
				return 1;
			}
	}
	close(client_fd);
	close(server_fd);
	return 0;
}


