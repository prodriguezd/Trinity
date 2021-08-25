#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_



typedef struct {
    char* name;
    int port;
    int fd;
}Connection;

typedef struct {
    char* name;
    char* directory;
    char* ip;
    int port;
    char* connectionIP;
    int initPort;
    int fiPort;
    int myFD;
    int numConnections;
    Connection *connections;
}User;

typedef struct {
	int type;
	char* header;
	short length;
	char* data;
}Trama;

typedef struct {
	User me;
	int fdClient;
	char* clientName;
	int userPort;
}Client;

#endif
