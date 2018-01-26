#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509.h>




#define MAXBUF 8192

void ShowCerts(SSL * ssl)
{
    X509 *cert;
    char *line;
    cert = SSL_get_peer_certificate(ssl);
    if(cert != NULL){
        printf("Digital certificate information:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert),0,0);
        printf("Certificate:%s\n",line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert),0,0);
        printf("Issuer:%s\n",line);
        free(line);
        X509_free(cert);
    }
    else
    {
        printf("No certificate information!\n");
    }
}
int main(int argc,char** argv)
{
    int i,j,sockfd,len,fd,size;
    char filename[50],sendfn[20];
    struct sockaddr_in dest;
    char buffer[MAXBUF + 1];
    SSL_CTX *ctx;
    SSL *ssl;

    if(argc != 3)
        {
            printf("Patameter format error!Correct usage is as follows:\n\t\t$s IP Port \n\t Such as :\t%s 127.0.0.1\n",argv[0],argv[0]);
            exit(0);
        }

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(SSLv23_client_method());

    if(ctx == NULL)
    {
        ERR_print_errors_fp(stdout);
        exit(1);
    }


    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        perror("Socket create error\n");
        exit(errno);
    }

    printf("socket created\n");

    bzero(&dest,sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(atoi(argv[2]));
    if(inet_aton(argv[1],(struct in_addr *)&dest.sin_addr.s_addr) == 0)
    {
        perror(argv[1]);
        exit(errno);
    }

    printf("address created\n");

    if(connect(sockfd,(struct sockaddr*)&dest,sizeof(dest))!= 0 )
    {
        perror("Connect\n");
        exit(errno);
    }


    printf("server connected !\n");

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl,sockfd);

    if(SSL_connect(ssl) == -1)
        ERR_print_errors_fp(stderr);
    else
    {
        printf("connected with %s encryption\n",SSL_get_cipher(ssl));
        ShowCerts(ssl);
    }

    printf("\nplease input the filename to load:\n");
    scanf("%s",filename);
    if((fd = open(filename,O_RDONLY,0666)) < 0)
    {
        perror("open");
        exit(1);
    }

    for(i = 0; i< strlen(filename);i++)
    {
        if(filename[i] == '/')
        {
            j = 0;
            continue;
        }
        else
            {
                sendfn[j] = filename[i] ;
                ++j;
            }
    }

    len = SSL_write(ssl,sendfn,strlen(sendfn));

    if(len < 0)
        printf("%s message send failed ! error code is %d,Error message are %s",buffer,errno,strerror(errno));


    bzero(buffer,MAXBUF +1 );
    while((size = read(fd,buffer,8192)))
    {
        if(size < 0 )
        {
            perror("read");
            exit(1);
        }
        else
        {
            len = SSL_write(ssl,buffer,size);
            if(len< 0)
                printf("%s message send failed! error code is %d,Error message are %s",buffer,errno,strerror(errno));
        }
        bzero(buffer,MAXBUF+1);
    }
    printf("send complete!\n");

    close(fd);
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sockfd);
    SSL_CTX_free(ctx);
    return 0;
}

