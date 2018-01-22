#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8008
int main(){

	int fd;
	int retcode;
	int recv_count;
	char *rcvbuf;
	struct sockaddr_in addr;
	memset(&addr,0x00,sizeof(struct sockaddr_in));
	fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr =inet_addr("127.0.0.1");
	addr.sin_port = htons( PORT);
	retcode = connect(fd,(struct sockaddr*)&addr,sizeof(addr));
	if(retcode == 0 )
		printf("connect successfully\n");
	


}
	
		
