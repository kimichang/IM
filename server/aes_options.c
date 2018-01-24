#include <stdio.h>
#include <openssl/aes.h>
#include <stdlib.h>
#include <string.h>


int encrypt(char *input,char **encrypt_string)
{
	AES_KEY aes;
	unsigned char key[AES_BLOCK_SIZE];
	unsigned char iv[AES_BLOCK_SIZE];
	unsigned int len;
	unsigned int i;
	
	len = 0;
	if((strlen(input) + 1) %AES_BLOCK_SIZE == 0)
	{
		len = strlen(input) + 1;
	}
	else 
	{
		len = ((strlen(input) + 1) / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;
	}

	for(i = 0;i<16;++i) {
	key[i] = 32 + i;
	}

	for(i = 0;i<AES_BLOCK_SIZE; ++i){
	iv[i] = 0;
	}

	if(AES_set_encrypt_key(key,128,&aes) < 0){
		fprintf(stderr,"Unable to set encryption key in AES\n");
		exit(0);
	}
	
	*encrypt_string = (unsigned char*)calloc(len,sizeof(unsigned char));
	if(*encrypt_string == NULL){
		fprintf(stderr,"Unable to allocate memory for encrypt string\n");
		exit(-1);
	}

	AES_cbc_encrypt(input,*encrypt_string,len,&aes,iv,AES_ENCRYPT);
	return len;
}

void decrypt(char *encrypt_string,char **decrypt_string,int len)
{
	unsigned char key[AES_BLOCK_SIZE];
	unsigned char iv[AES_BLOCK_SIZE];
	AES_KEY aes;
	int i;
	for(i = 0; i<16;++i) {
	key[i] = 32 + i;
	}
	*decrypt_string = (unsigned char *) calloc(len,sizeof(unsigned char ));
	if(*decrypt_string == NULL) {
	
		fprintf(stderr,"Unable to allocate memory for decrypt string\n");
		exit(-1);
	}
	for(i = 0;i<AES_BLOCK_SIZE;++i) {
	iv[i] = 0;
	}
	if(AES_set_decrypt_key(key,128,&aes) < 0) {
	fprintf(stderr,"Unable to set decryption key in aes\n");
	exit(-1);
	}
	
	AES_cbc_encrypt(encrypt_string,*decrypt_string,len,&aes,iv,AES_DECRYPT);
}


