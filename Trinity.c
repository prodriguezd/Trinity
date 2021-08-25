#define  _GNU_SOURCE 1
#include <stdio.h>

#define START           "\n\nStarting Trinity..."
#define FILE_MISS       "\nConfiguration file missing!"
#define FILE_ERR        "\nError opening file!"
#define COMM_ERR        "\nCommand not recognised!"
#define ERR_THRD        "\nError creating server"
#define TEST            "\nTesting..."
#define USR 			"\n$%s: "
#define ERR_DIR         "\nUnable to create/find directory %s!"


#include "Structure.h"
#include "File.h"
#include "Server.h"
#include "Utils.h"
#include "Command.h"

#include <unistd.h> //read write
#include <signal.h>
#include <sys/types.h> //open
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h> //strlen


int out = 0;

User user;

void ctrlC (){
    out = 1;
    COMMAND_EXIT(&user);
    exit(1);
}


int option() {
	char* buffer;
	int bytes = asprintf(&buffer, USR, user.name);
	write(1, buffer, bytes);
    

    char* command;
    int opt = -1;

    signal(SIGINT, ctrlC);
    command = UTILS_readUntil(0, '\n');
   
    opt = UTILS_getOption(command);
    
	
    switch (opt) {
        case 1:   		//SHOW CONNECTIONS
			COMMAND_showConnections(user);           
        break;

        case 2:  		//CONNECT
			COMMAND_connect(command, &user);           
		break;

        case 3:			//SAY
            COMMAND_SAY(command, user);
        break;

        case 4:			//BROADCAST
			COMMAND_BROADCAST(command, user);
        break;

        case 5:			//SHOW AUDIOS
			COMMAND_showAudios(command, user);
        break;

        case 6:			//DOWNLOAD
            COMMAND_download(command, user);
        break;

        case 7:			//EXIT
            COMMAND_EXIT(&user);
        break;

        default:
            write(1, COMM_ERR, strlen(COMM_ERR));
        break;

    }
	free(command);
	free(buffer);
	return opt;
}

int main(int argc, char** argv) {
	

	if (argc != 2) {
        write(1, FILE_MISS, strlen(FILE_MISS));
        exit(-1);
    }

	int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        write(1, FILE_ERR, strlen(FILE_ERR));
        exit(-1);
    }

	FILE_readFile(fd, &user);
    
	if (UTILS_checkDirectory(user.directory, user.name) == 0) {
    	char *buffer = (char*)malloc(sizeof(char)); 
	    int bytes = sprintf(buffer, ERR_DIR, user.directory);
        write(1, buffer, bytes);
		free(buffer);
        exit(-1);
    }


	user.connections = (Connection*)malloc(sizeof(Connection));
	close(fd);

	//server
    pthread_t thread_s;
    if (pthread_create(&thread_s, NULL, SERVER_launchServer, &user) != 0) {
        write(1, ERR_THRD, strlen(ERR_THRD));
        exit(-1);
    }

	//client
	write(1, START, strlen(START));
	
	while (1) {

		int opt = option();

		if (opt == 7) break;
	}

	return 1;
}
