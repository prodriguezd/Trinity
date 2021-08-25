#include "File.h"


char* readUntil(int fd, char cFi) {
    int i = 0;
    char c = '0';
    char* buffer = (char*)malloc(sizeof(char));

    while (c != cFi) {

        read(fd, &c, 1);

        if (c != cFi) {
            buffer[i] = c;
            buffer = (char*)realloc(buffer, i + 2);
        }
        i++;
    }
    buffer[i - 1] = '\0';
    return buffer;
}

void FILE_readFile(int fd, User *user) {
    user->name = readUntil(fd, '\n');
    user->directory = readUntil(fd, '\n');
    user->ip = readUntil(fd, '\n');

    char* aux1 = readUntil(fd, '\n');
    user->port = atoi(aux1);
    free(aux1);

    user->connectionIP = readUntil(fd, '\n');

    aux1 = readUntil(fd, '\n');
    user->initPort = atoi(aux1);
    free(aux1);

    aux1 =  readUntil(fd, '\n');
    user->fiPort = atoi(aux1);
    free(aux1);

    user->numConnections = 0;

}
