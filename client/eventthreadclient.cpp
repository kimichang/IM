#include <iostream>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <event2/event.h>
#include <event.h>
using namespace std;
#define BUF_SIZE 8192

int connectserver(char* ip,int port)
{
    int fd = socket(AF_INET,SOCK_STREAM,0);
    cout<<"fd = " << fd<<endl;
    if(-1 == fd){
        cout << "Error "<<endl;
        return -1;
    }

    struct sockaddr_in remote_addr;
    memset(&remote_addr,0,sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(8008);
    remote_addr.sin_addr.s_addr = inet_addr(ip);

    int con_result = connect(fd,(struct sockaddr*)&remote_addr,sizeof(struct sockaddr));
    if(con_result < 0)
    {
        cout<<"connect error"<<endl;
        close(fd);
        return -1;
    }
    cout << "con_result = " <<con_result<<endl;
    return fd;
}

void on_read(int sock,short event,void *arg)
{
    char* buffer = new char[BUF_SIZE];
    memset(buffer,0,sizeof(char)*BUF_SIZE);

    int size = read(sock,buffer,BUF_SIZE);
    if(0 == size)
    {
        cout<<"read null in socket"<<endl;
        struct event* read_ev = (struct event*)arg;
        if(NULL != read_ev)
        {
            event_del(read_ev);
            free(read_ev);
        }
        close(sock);
        return ;
    }

    cout<<"received from server ----"<<buffer<<endl;
    delete[] buffer;
}

void* init_read_event(void* arg)
{
    long long_sock = (long)arg;
    int sock = (int)long_sock;
    struct event_base* base = event_base_new();
    struct event* read_ev = (struct event*)malloc(sizeof(struct event));
    event_set(read_ev,sock,EV_READ| EV_PERSIST,on_read,read_ev);
    event_base_set(base,read_ev);
    event_add(read_ev,NULL);
    event_base_dispatch(base);
    event_base_free(base);
}

void init_read_event_thread(int sock)
{
    pthread_t thread;
    pthread_create(&thread,NULL,init_read_event,(void*)sock);
    pthread_detach(thread);
}

int main()
{
    cout <<"Please input server ip:"<<endl;
    char ip[16] ;
    cin>>ip;
    cout <<"please input port:"<<endl;
    int port;
    cin>>port;
    int socket_fd = connectserver(ip,port);
    cout <<"socket_fd - "<< socket_fd<<endl;
    init_read_event_thread(socket_fd);

    char buffer[BUF_SIZE];
    bool isbreak = false;
    while(!isbreak)
    {
        cout << "Input data "<<endl;
        cin>>buffer;
        if(strcmp("q",buffer) == 0 || strcmp("quit",buffer ) == 0)
        {
            isbreak = true;
            close(socket_fd);
            break;
        }
        int write_num = write(socket_fd,buffer,strlen(buffer));
        cout << write_num <<"characters written"<<endl;
        sleep(2);
    }
    cout<<"main out"<<endl;
    return 0;
}


