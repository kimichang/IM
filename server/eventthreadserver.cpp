#include <iostream>
#include <sys/select.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <event.h>
#include <stdlib.h>
using namespace std;

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8008
#define BUF_SIZE 8192

struct sock_ev_write{
    struct event* write_ev;
    char* buffer;
};

struct sock_ev{
    struct event_base* base;
    struct event* read_ev;
};

void destroy_sock_ev_write(struct sock_ev_write* sock_ev_write_struct)
{
    if(NULL != sock_ev_write_struct){
        if(NULL != sock_ev_write_struct->write_ev){
            free(sock_ev_write_struct->write_ev);
        }
        if(NULL != sock_ev_write_struct->buffer)
        {
            delete[] sock_ev_write_struct->buffer;
        }
        free(sock_ev_write_struct);
    }
}

void destroy_sock_ev(struct sock_ev* sock_ev_struct)
{
    if(NULL == sock_ev_struct)
    {
        return;
    }
    event_del(sock_ev_struct->read_ev);
    event_base_loopexit(sock_ev_struct->base,NULL);
    if(NULL != sock_ev_struct->read_ev)
    {
        free(sock_ev_struct->read_ev);
    }
    event_base_free(sock_ev_struct->base);
    free(sock_ev_struct);
}

int getsocket()
{
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == fd)
        cout<<"Error fd"<<endl;
    
    return fd;
    
}
void on_write(int sock,short event,void* arg)
{
    cout <<"On_write,sock = :"<<sock<<endl;
    if(NULL == arg)
    {
        cout<< "error void* arg is null"<<endl;
        return;
    }
    struct sock_ev_write* sock_ev_write_struct = (struct sock_ev_write*)arg;
    char buffer[BUF_SIZE];
    sprintf(buffer,"fd=%d,received[%s]",sock,sock_ev_write_struct->buffer);

    int write_num = write(sock,buffer,strlen(buffer));
    destroy_sock_ev_write(sock_ev_write_struct);
    cout<<"On_write out"<<endl;
}

void on_read(int sock,short event,void* arg)
{
    cout<<"on_read called,sock = :"<<sock<<endl;
    if(NULL == arg)
    {
        return ;
    }
    struct sock_ev* event_struct = (struct sock_ev*)arg;
    char* buffer = new char[BUF_SIZE];
    memset(buffer,0,sizeof(char)*BUF_SIZE);
    int size = read(sock,buffer,BUF_SIZE);
    if(0 == size)
    {
        cout<<"read size is 0 in socket"<<sock<<endl;
        destroy_sock_ev(event_struct);
        close(sock);
        return;
    }
    struct sock_ev_write* sock_ev_write_struct = (struct sock_ev_write*)malloc(sizeof(struct sock_ev_write));
    sock_ev_write_struct->buffer = buffer;

    struct event* write_ev = (struct event*)malloc(sizeof(struct event));
    sock_ev_write_struct->write_ev = write_ev;
    event_set(write_ev,sock,EV_WRITE,on_write,sock_ev_write_struct);
    event_base_set(event_struct->base,write_ev);
    event_add(write_ev,NULL);
    cout<<"on_read out on sock " <<sock<<endl;
}

void* process_in_new_thread_when_accepted(void* arg)
{
    long long_fd = (long)arg;
    int fd = (int)long_fd;
    if(fd < 0)
    {
        cout << "Process_in_new_threa_when_accepted() quit"<<endl;
        return 0;
    }
    struct event_base* base = event_base_new();
    struct event* read_ev = (struct event*)malloc(sizeof(struct event));

    struct sock_ev* event_struct = (struct sock_ev*)malloc(sizeof(struct sock_ev));
    event_struct->base = base;
    event_struct->read_ev = read_ev;
    event_set(read_ev,fd,EV_READ | EV_PERSIST,on_read,event_struct);
    event_base_set(base,read_ev);
    event_add(read_ev,NULL);
    event_base_dispatch(base);
    cout<<"event_base_dispatch() stop on fd " << fd<<endl;
    return 0;
}

void accept_new_thread(int sock)
{
    pthread_t thread;
    pthread_create(&thread,NULL,process_in_new_thread_when_accepted,(void*)sock);
    pthread_detach(thread);
}

void on_accept(int sock,short event,void* arg)
{
    struct sockaddr_in remote_addr;
    int sin_size = sizeof(struct sockaddr_in);
    int new_fd = accept(sock,(struct sockaddr*)&remote_addr,(socklen_t*)&sin_size);

    if(new_fd < 0)
    {
        cout <<"accept error on sock " << sock << endl;
        return ;
    }
    cout << "new fd accepted is " << new_fd<< endl;
    accept_new_thread(new_fd);
    cout<<"on_accept() out on fd " << new_fd << endl;

}

int main()
{
    int fd  = getsocket();
    if(fd < 0)
    {
        cout << "error in main "<<endl;
        exit(1);
    }
    cout << "main in fd = " << fd << endl;
    struct sockaddr_in local_addr;
    memset(&local_addr,0,sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(SERVER_PORT);
    local_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    int bind_result = bind(fd,(struct sockaddr*)&local_addr,sizeof(struct sockaddr));
    if(bind_result < 0 )
    {
        cout <<" bind error"<< endl;
        return -1;
    }
    cout << "bind result " << bind_result<<endl;

    listen(fd,100);
    struct event_base* base = event_base_new();
    struct event listen_ev;
    event_set(&listen_ev,fd,EV_READ | EV_PERSIST,on_accept,NULL);
    event_base_set(base,&listen_ev);
    event_add(&listen_ev,NULL);
    event_base_dispatch(base);
    cout<<"event base dispatch out" << endl;

    event_del(&listen_ev);
    event_base_free(base);
    cout << " main out" << endl;
	cout << "server out" <<endl;
}


