#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/crypto.h>

SSL_CTX *ctx = NULL;
BIO* sbio = NULL,*bbio = NULL,*acpt = NULL,*out = NULL;
SSL* ssl = NULL;

int main()
{
    char buf[8192];
    ERR_load_crypto_strings();
    ERR_load_SSL_strings();
    OpenSSL_add_ssl_algorithms();

    ctx = SSL_CTX_new(TLSv1_server_method());
    if(ctx == NULL)
    {
        ERR_print_errors_fp(stderr);
        return -1;
    }

    if(!SSL-CTX_use_certificate_file(ctx,"server.crt",SSL_FILETYPE_PEM))
    {
        return -1;
    }
    else
    {
        printf("Step 1 : Load server certificate done\n");
    }

    if(!SSL_CTX_use_PrivateKey_file(ctx,"server.key",SSL_FILETYPE_PEM))
    {
        ERR_print_errors_fp(stderr);
        return -1;
    }
    else
    {
        printf("Step 2 : Load server private key done\n");
    }

    if(!SSL_CTX_check_private_key(ctx))
    {
        ERR_print_errors_fp(stderr);
        return -1;
    }
    else
    {
        printf("Step 3 : Check server private key done\n");
    }


    sbio = BIO_new_ssl(ctx,0) // 0: as server ; non-0: as client
    BIO_get_ssl(sbio,&ssl);
    if(!ssl)
    {
        printf("SSL pointer is null\n");
        return -1;
    }
    SSL_get_mode(ssl,SSL_MODE_AUTO_RETRY);
    bbio = BIO_new(BIO_f_buffer());
    sbio = BIO_push(bbio,sbio);
    acpt = BIO_new_accept("8008");

    BIO_set_accept_bios(acpt,sbio);
    out = BIO_new_fp(stdout,BIO_NOCLOSE);

    if(BIO_do_accept(acpt) <= 0)
    {
        ERR_print_errors_fp(stderr);
        return -1;
    }

    printf("Step 4 :waiting for the incoming connection\n");

    if(BIO_do_accept(acpt) <= 0)
    {
        ERR_print_errors_fp(stderr);
        return -1;
    }
    printf("Step 5: A connection come in\n");

    sbio = BIO_pop(acpt);
    BIO_free_all(acpt);

    if(BIO_do_handshake(sbio) <= 0)
    {
        ERR_print_errors_fp(stderr);
        return -1;
    }
    printf("Step 6: handshake done\n");
    memset(buf,0,8192*sizeof(char));

    BIO_read(sbio,buf,8192);

    printf("message from client : %s\n",buf);
    BIO_free_all(sbio);
    return 0;
}


