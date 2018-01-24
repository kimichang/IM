#ifndef _AES_H_
#define _AES_H_
int encrypt(char *input,char **encrypt_string);
void decrypt(char *encrypt_string,char **decrypt_string,int len);
#endif
