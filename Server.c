#include "Server.h"
#include <stdio.h>
#include <string.h>

#define MSG "\r[%s]: %s\n"
#define EXIT "%s se ha desconectado!\n"
#define VECES "\nveces read: %d - %s\n"


extern int out;


void SERVER_downloadAudios(Client client, Trama trama){

	int file = -1, i;
	char* directoryName = (char*) malloc (sizeof(char));
	char* md5sum;
	directoryName[0] = '\0';
	
	char aux[2];
	aux[0] = '/';
	aux[1] = '\0';
	
   


	directoryName = my_strcat(directoryName, client.me.name);
	//my_strcat2(&directoryName, client.me.name);

	directoryName = my_strcat(directoryName, aux);
	//my_strcat2(&directoryName, aux);

	directoryName = my_strcat(directoryName, client.me.directory);
	//my_strcat2(&directoryName, client.me.directory);

	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir(directoryName)) != NULL) {
		ent = readdir(dir);
		ent = readdir(dir);
		while ((ent = readdir(dir)) != NULL) {
			if(strcmp(ent->d_name, trama.data) == 0) {
				directoryName = my_strcat(directoryName, aux);
				//my_strcat2(&directoryName, aux);

				directoryName = my_strcat(directoryName, trama.data);
				//my_strcat2(&directoryName, trama.data);
				
				md5sum = UTILS_getMD5SUMbyFile(directoryName);
				
				file = open(directoryName, O_RDONLY);
				break;
			}
		}
		closedir(dir);

		if (file > 0) {


			char* data = (char*)malloc(99);
			
			while ((i = read(file, data, 99)) > 0) {
				//data[i] = '\0';
				UTILS_sendTramaAudios(client.fdClient, data, i);
				memset(data, '\0', i);

			}

			UTILS_sendTrama(client.fdClient, 5, "EOF", md5sum);

			
			Trama trama2;
			trama2 = UTILS_readTrama(client.fdClient);

			if (strcmp(trama2.header, "MD5KO") == 0) {
				//error sending audio
				write(1, "\nmd5sum diferente!\n", strlen("\nmd5sum diferente!\n"));
			}
			free(data);
			free(trama2.header);
			free(trama2.data);
		}
		else {
			UTILS_sendTrama(client.fdClient, 5, "AUDIO_KO", "");
		}
	}
	else {
		UTILS_sendTrama(client.fdClient, 5, "AUDIO_KO", "");
	}
	free(directoryName);
	free(md5sum);
}


void *threadConnection(void *arg) {
	
	Client client = *(Client*)arg;
	client.clientName = (char*)malloc(sizeof(char));

	free(arg);

	char* buffer;
	char* audios;
	
	Trama trama;
	
	while (!out) {		
		trama = UTILS_readTrama(client.fdClient);

        switch (trama.type) {
        	case 1:   		//connect
				client.clientName = my_strcat(client.clientName, trama.data);
				//my_strcat2(&client.clientName, trama.data);
				UTILS_sendTrama(client.fdClient, 1, "CONOK", client.me.name);
        	break;

        	case 2:  		//say
				asprintf(&buffer, MSG, client.clientName, trama.data);
				write(1, buffer, strlen(buffer));
				UTILS_sendTrama(client.fdClient, 2, "MSGOK", "");
				asprintf(&buffer, USR, client.me.name);
				write(1, buffer, strlen(buffer));   
				free(buffer);
			break;

    	    case 3:			//broadcast
				asprintf(&buffer, MSG, client.clientName, trama.data);
				write(1, buffer, strlen(buffer));
 				UTILS_sendTrama(client.fdClient, 3, "MSGOK", "");
				asprintf(&buffer, USR, client.me.name);
				write(1, buffer, strlen(buffer)); 
				free(buffer);         
       	 	break;

        	case 4:			//show audios
				audios = UTILS_getAudios(client.me.directory, client.me.name);
                UTILS_sendTrama(client.fdClient, 4, "LIST_AUDIOS", audios);
				free(audios);
        	break;

        	case 5:			//download AUDIOS
				SERVER_downloadAudios(client, trama);
				
        	break;

        	case 6:			//exit
				UTILS_sendTrama(client.fdClient, 6, "CONOK", "");
        	break;

        	default: //show connections
				//caca
        	break;
    	}

		if (trama.type == 5) {
			asprintf(&buffer, USR, client.me.name);
				write(1, buffer, strlen(buffer));   
				free(buffer);
		}
		if (trama.type == 6) break;
		//free(trama.header);
		//free(trama.data);
	}
	//write(1, "\nSALIMOS DEL WHILE 1!\n", strlen("\nSALIMOS DEL WHILE 1!\n"));
	close(client.fdClient);
	free(client.clientName);
	pthread_exit(NULL);
	return NULL;
}

void *SERVER_launchServer(void *arg) {
    User userServer = *(User*)arg;


    int server_socketfd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server_socketfd == -1) {
        write(1, ERR_SOCK, strlen(ERR_SOCK));
		exit(-1);
    }
	
    
	struct sockaddr_in s_addr;
    struct sockaddr_in c_addr;
    socklen_t c_len = sizeof (c_addr);

    bzero (&s_addr, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons (userServer.port);
    s_addr.sin_addr.s_addr = inet_addr(userServer.ip);

    if (bind (server_socketfd, (void*) &s_addr, sizeof(s_addr)) < 0) {
        write (1, ERR_BIND, strlen(ERR_BIND));
        exit(-1);
    }

    listen (server_socketfd, LISTEN_BACKLOG);

    int client_socketfd;

    while (1) {
        client_socketfd = accept(server_socketfd, (struct sockaddr *)&c_addr, (socklen_t*)&c_len);
        if (client_socketfd < 0) {
            write(1, ERR_ACC, strlen(ERR_ACC));
            //error accept
        }
		
		Client *client = (Client*)malloc(sizeof *client);
		client->fdClient = client_socketfd;
		client->me = userServer;
		
		
		pthread_t thread_c;
	
		if (pthread_create(&thread_c, NULL, threadConnection, (void *)client) != 0) {
			UTILS_sendTrama(client->fdClient, 1, "CONKO", "");
			free(client);
		}

			
    }
	return NULL;
}
