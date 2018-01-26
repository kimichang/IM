#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>



int main()
{
    int ret = 0;
    BIO *bio = NULL;
    SSL* ssl = NULL;
    SSL_CTX *ctx = NULL;
    X509 * pX509 = NULL;

    const SSL_METHOD *sslMethod = NULL;

    char filename[512] = {0};
    X509_NAME * pX509_NAME = NULL;

    char clientmsg[] = "message from client\n";

    int len = 0;
    char buffer[8192];
    
    SSL_library_init();
    ERR_load_BIO_strings();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    do{
        sslMethod = TLSv1_client_method();
        if(NULL == sslMethod){
            printf("TLSv1_client_method error :%s\n",ERR_error_string(ERR_get_error(),NULL));
            ret = -1;
            break;
        }

        ctx = SSL_CTX_new(sslMethod);
        if(NULL == ctx )
        {
            printf("SSL_CTX_new error:%s\n",ERR_error_string(ERR_get_error(),NULL));
            ret = -1;
            break;
        }

        if(0 == SSL_CTX_load_verify_locations(ctx,"cacert.crt",NULL))
        {
            printf("SSL_CTX_load_verify_locations err:%s\n",ERR_error_string(ERR_get_error(),NULL));
            ret = -1;
            break;
        }

        if(0 == SSL_CTX_use_certificate_file(ctx,"client.crt",SSL_FILETYPE_PEM))
        {
            printf("SSL_CTX_use_certificate_file error\n");
            ret = -1;
            break;
        }
        if(0 == SSL_CTX_use_PrivateKey_file(ctx,"client.key",SSL_FILETYPE_PEM))
        {
            ret = -1;
            printf("SSL_ctx_use_PrivateKey_file error\n");
            break;
        }

        if(0 == SSL_CTX_check_private_key(ctx))
        {
            printf("SSL_CTX_check_private_key error\n");
            ret = -1;
            break;
        }

        bio = BIO_new_ssl_connect(ctx);
        if(NULL == bio)
        {
            printf("BIO_new_ssl_connect error\n");
            break;
        }

        BIO_get_ssl(bio,&ssl);
        SSL_set_mode(ssl,SSL_MODE_AUTO_RETRY);
        BIO_set_conn_hostname(bio,"127.0.0.1:8008");

        if(0 >= BIO_do_connect(bio))
        {
            printf("bio_do_connect error\n");
            break;
        }
        if(X509_V_OK != SSL_get_verify_result(ssl))
        {
            printf("SSL_get_verify_result error\n");
            break;
        }
        pX509 = SSL_get_peer_certificate(ssl);
        if(NULL == pX509)
        {
            printf("SSL_get_peer_certificate error\n");
            break;
        }
        pX509_NAME = X509_get_subject_name(pX509);
        if(NULL == pX509_NAME) 
        {
            printf("X509_get_subject_name error\n");
            break;
        }
        
        if(0 >= BIO_write(bio,clientmsg,strlen(clientmsg)))
        {
            printf("Send a string to server failed\n");
            break;
        }

        }while(0);

        if(NULL != bio)
        {
            BIO_free_all(bio);
        }
        if(NULL != ctx)
        {
            SSL_CTX_free(ctx);
        }
    return ret;
}

