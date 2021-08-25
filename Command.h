#ifndef _COMMAND_H_
#define _COMMAND_H_

#define  _GNU_SOURCE 1


#define MSG_TESTING                         "\nTesting..."
#define MSG_CONNECTIONS                     "\n%d connections available"
#define ERR_SHOWCONN						"\nUnable to show connections!"
#define ERR_FORK 							"\nError on Fork()"
#define TR_NEWCONN							"[%s]"
#define C_OK								"CONOK"
#define CONN_ERR							"\nUnable to connect to port %d!"
#define ERR_MSG								"\nError sending message"
#define CONN_OK								"\n%d connected: %s\n"
#define ERR_REQ_AUDIOS						"\nError sending request!"
#define CONN_NO_EXISTS						"\nYou are not connected to user %s!"
#define COMM_ERROR                          "\nCommand not recognised!"
#define ERR_EXEC                            "\nERROR execvp"
#define AUDIOS                              "\n%s"
#define AUDIO_KO                            "\nAudio %s does not exists"
#define BROADCAST_ANSWER 	                "[%s] Cool!\n"
#define DOWNLOAD 			                "[%s] %s downloaded\n"

#define CONN				                "\nConnecting..."
#define USR_NOT_CONN                        "\nUser not connected!"




#include "Structure.h"
#include "Utils.h"
#include <sys/wait.h>
#include <sys/types.h> 
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <dirent.h>
//#include <openssl/md5.h>



void COMMAND_download(char* command, User user);
void COMMAND_EXIT(User *user);
void COMMAND_BROADCAST(char* command, User user);
void COMMAND_SAY(char* command, User user);
void COMMAND_showConnections(User user);
void COMMAND_connect(char *str, User *user);
void COMMAND_showAudios(char *str, User user);
/*
void COMMAND_say();
void COMMAND_showAudios();
void COMMAND_download();
void COMMAND_exit();
*/

#endif

