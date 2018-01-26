#include <openssl/ssl.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/pkcs12.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IP "127.0.0.1"
#define PORT 8008
#define CERT_PATH "./sslclientcert.pem"
#define KEY_PATH "./sslclientkey.pem"
#define CAFILE "./demoCA/cacert.pem"
static SSL_CTX *g_sslctx = NULL;

int connect_to_server(int fd,char* ip,int port)
{
    struct sockaddr_in svr;
    memset(&svr,sizeof(svr));
    svr.sin_family = AF_INET;
    svr.sin_port = htons(PORT);
    if(inet_pton(AF_INET,ip,&svr.sin_addr) <=0)
    {
        printf("invalid ip address\n");
        exit(1);
    }

    if(connect(fd,(struct sockaddr*)&svr,sizeof(svr)))
    {
        printf("connect error\n");
        exit(1);
    }

    return 0;
}

void print_client_cert(char* path)
{
    X509 *cert = NULL;
    FILE *fp = NULL;
    fp = fopen(path,"rb");
    cert = PEM_read_X509(fp,NULL,NULL,"1111");
    X509_NAME *name = NULL;
    char buf[8192] = {0};
    BIO* bio_cert = NULL;
    name = X509_get_subject_name(cert);
    X509_NAME_online(name,buf,8191);
    printf("client subject name: %s\n",buf);
    memset(buf,0,sizeof(buf));
    bio_cert = BIO_new(BIO_s_mem());
    PEM_write_bio_X509(bio_cert,cert);
    BIO_read(bio_cert,buf,8191);
    printf("client cert : %s\n",buf);
    if(bio_cert)
        BIO_free(bio_cert);
    fclose(fp);
    if(cert)
        X509_free(cert);
}

static int verify_cb(int res,X509_STORE_CTX *xs)
{
    printf("SSL verify result:%d\n",res);
    switch(xs->error)
    {
        case X509_V_ERR_UNABLE_TO_GET_CRL:
            printf("NOT GET CRL\n");
            return 1;
        default:
            break;
    }
    return res;
}


int sslctx_init()
{
#if 0
    BIO *bio = NULL;
    X509 *cert = NULL;
    STACK_OF(X509) *ca = NULL;
    EVP_PKEY *pkey = NULL;
    PKCS12* p12 = NULL;
    X509_STORE *store = NULL;
    int error_code = 0;
#endif

    int ret = 0;
    print_client_cert(CERT_PATH);
    SSL_load_error_strings();
    SSL_library_init();
    g_sslctx = SSL_CTX_new(SSLv23_client_method());
    if(g_sslctx == NULL)
    {
        ret = -1;
        goto end;
    }

    SSL_CTX_set_default_passwd_cb_user_data(g_sslctx,"1111");

    SSL_CTX_set_cipher_list(g_sslctx,"HIGH:MEDIA:LOW:!DH");

    SSL_CTX_set_verify(g_sslctx,SSL_VERIFY_PEER,verify_cb);

    SSL_CTX_set_verify_depth(g_sslctx,10);

    SSL_CTX_load_verify_locations(g_sslctx,CAFILE,NULL);

    if(SSL_CTX_use_certificate_file(g_sslctx,CERT_PATH,SSL_FILETYPE_PEM) <= 0)
    {
        printf("certificate file error\n");
        ret = -1;
        goto end;
    }

    if(SSL_CTX_user_PrivateKey_file(g_sslctx,KEY_PATH,SSL_FILETYPE_PEM) <= 0)
    {
        printf("private key file error\n");
        ret = -1;
        goto end;
    }

    if(!SSL_CTX_check_private_key(g_sslctx)){
        printf("Check private key failed\n");
        ret = -1;
        goto end;
    }
end:
    return ret;
}

void sslctx_release()
{
    EVP_cleanup();
    if(g_sslctx){
        SSL_CTX_free(g_sslctx);
    }
    g_sslctx = NULL;
}

void print_peer_certificate(SSL *ssl)
{
    X509 *cert = NULL;
    X509_NAME *name = NULL;
    char buf[8192] = {0};
    BIO *bio_cert = NULL;
    cert = SSL_get_peer_certificate(ssl);

    name = X509_get_subject_name(cert);

    X509_NAME_online(name,buf,8191);
    printf("Serversubject name :%s\n",buf);
    memset(buf,0,sizeof(buf));
    bio_cert = BIO_new(BIO_s_mem());
    PEM_write_bio_X509(bio_cert,cert);
    BIO_read(bio_cert,buf,8191);
    printf("server cert :\n %s\n",buf);
    if(bio_cert)
        BIO_free(bio_cert);
    if(cert)
        X509_free(cert);
}


int main(int argc,char** argv){
    int fd = -1,ret = 0;
    SSL *ssl = NULL;
    char buf[1024] = {0};
    if(sslctx_init()){
        printf("sslctx init failed\n");
        goto out;
    }
    fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd <0)
    {
        printf("socket error\n");
        goto out;
    }

    if(connect_to_server(fd,IP,PORT)){
        printf("can not connect to server\n");
        goto out;
    }
    ssl =SSL_new(g_sslctx);
    if(!ssl){
        printf("can not get ssl from ctx\n");
        goto out;
    }
    SSL_set_fd(ssl,fd);

    ret = SSL_connect(ssl);
    if(ret != 1){
        int err = ERR_get_error();
        printf("connect error code %d\n",err);
        goto out;
    }

    print_peer_certificate(ssl);
    SSL_shutdown(ssl);
out:
    if(fd>0)
        close(fd);
    if(ssl != NULL)
    {
        SSL_free(ssl);
        ssl = NULL;
    }
    if(g_sslctx != NULL)
        sslctx_release();
    return 0;
}



