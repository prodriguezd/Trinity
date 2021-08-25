#ifndef _SERVER_H_
#define _SERVER_H_



#define LISTEN_BACKLOG		64
#define ERR_SOCK        "\nError creating socket!"
#define ERR_BIND        "\nError during bind!"
#define ERR_THREAD      "\nUnable to create client thread!"
#define ERR_ACC         "\nError on accept!"
#define USR 			"\n$%s: "

#include "Structure.h"
#include "Utils.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
//#include <openssl/md5.h>
//int MD5_Init(MD5_CTX *c);
//int MD5_Update(MD5_CTX *c, const void *data, unsigned long len);
//int MD5_Final(unsigned char *md, MD5_CTX *c);




void SERVER_downloadAudios(Client client, Trama trama);
void* SERVER_launchServer(void *arg);
void* threadConnection(void *arg);

#endif

