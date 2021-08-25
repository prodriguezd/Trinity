#ifndef _UTILS_H_
#define _UTILS_H_

#define  _XOPEN_SOURCE_EXTENDED 1

#include "Structure.h"
#include "Command.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include <dirent.h>
#include <errno.h>


#define TRAMA 								"%s[%s]%s%s"
#define TRAMA_AUDIO                         "%s[%s]"
#define MSG_CONNECTIONS_AVAILABLE "\n%d connection(s) available\n"

char* UTILS_getMD5SUMbyFile(char* data);
char* UTILS_getMD5SUMbyData(char* data);
int UTILS_getArguments(char* command);
char* UTILS_limpiaChar(char* name);
void UTILS_eliminaConexion(User *user, int pos);
char* UTILS_getBroadcastMSG(char* command);
void  UTILS_mostrarConexiones(int puertos, User user);
char* UTILS_getName(char* command);
char* UTILS_getMSG(char* command);
char* UTILS_readUntil(int fd, char cFi);
int UTILS_getOption(char* command);
void UTILS_showConnections(int fd, User user);
int UTILS_getPort(char *str);
char* UTILS_createTrama(int type, char* header, char *data);
Trama UTILS_readTrama(int fd);
int UTILS_sendTrama(int fd, int t, char *header, char *data);
int UTILS_checkDirectory(char* directoryName, char* userName);
char* UTILS_getInfoShowAudios(char* str);
char* UTILS_getAudios(char* directory, char* userName);
int UTILS_commandAudios(char* str);
char* my_strcat2(char* dest, char* str2);
char* my_strcat(char* dest, char* str2);
char* UTILS_getAudioName(char* command);
char* my_strcatAudio(char* dest, char* str2);
int UTILS_sendTramaAudios(int fd, char *data, int size);

#endif
