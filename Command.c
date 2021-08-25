#include "Command.h"
#define VECES 				                "\nveces: %d\n"
#define S 					                "\ndataFile = %s\n"

void COMMAND_download(char* command, User user){
	
	int espacios = UTILS_getArguments(command);

	if (espacios != 2) {
		write(1, COMM_ERROR, strlen(COMM_ERROR));
	}
	else {

		char* userName = UTILS_getName(command);
		char* file =  UTILS_getAudioName(command);
		char *buffer;
		int fd = -1, i, bytes;

		for (i = 0; i < user.numConnections; i++) {
			if(strcmp(user.connections[i].name, userName) == 0) {
				fd = user.connections[i].fd;
				break;
			}
		}

		if (fd < 0) {
			bytes = asprintf(&buffer, CONN_NO_EXISTS, userName);
			write(1, buffer, bytes);
			free(buffer);
		}
		else {
			Trama trama;

			if (UTILS_sendTrama(fd, 5, "AUDIO_RQST", file)  > 0) {
			

				DIR* dir;
				char* myDir;
				char* dataFile = (char*)malloc(sizeof(char));
				dataFile[0] = '\0';

				myDir = my_strcat2(user.name, "/");
				myDir = my_strcat(myDir, user.directory);

				char* md5sum;
		
				if ((dir = opendir(myDir)) == NULL) {
					write(1, "\nERROR, no existe el fichero\n", strlen("ERROR, no existe el fichero\n"));
				}else{

					trama = UTILS_readTrama(fd);

					if (trama.type == 5) {

						myDir = my_strcat(myDir, "/");
						myDir = my_strcat(myDir, file);
						int filedescriptor = open(myDir, O_RDWR | O_APPEND | O_CREAT, 0777);
					

						read(fd, &trama.length, sizeof(short));
						trama.data = (char*)malloc(sizeof(char) * (trama.length + 1));
						read(fd, trama.data, trama.length);


						while(strcmp(trama.header, "EOF") != 0) {
							/*read(fd, &trama.length, sizeof(short));
							trama.data = (char*)malloc(sizeof(char) * (trama.length +1));
							read(fd, trama.data, trama.length);*/

							write(filedescriptor, trama.data, trama.length);
						
							free(trama.header);
							free(trama.data);
							trama = UTILS_readTrama(fd);
							if(strcmp(trama.header, "EOF") == 0){
								break;
							}
						
							read(fd, &trama.length, sizeof(short));
							trama.data = (char*)malloc(sizeof(char) * (trama.length +1));
							read(fd, trama.data, trama.length);
						}
						close(filedescriptor);

						write(1, "yo\n", 3);

						
						write(1, "\n", 1);
						write(1, trama.data, strlen(trama.data));
					
						md5sum = UTILS_getMD5SUMbyFile(myDir);
						write(1, "\n", 1);
						write(1, md5sum, strlen(md5sum));

						if (strcmp(trama.data, md5sum) == 0) {
							UTILS_sendTrama(fd, 5, "MD5OK", "");

						}else {
							UTILS_sendTrama(fd, 5, "MD5KO", "");
							//eliminamos fichero
						}
						free(dir);
						free(myDir);
						free(dataFile);
						free(md5sum);
						free(trama.header);
						free(trama.data);
					}
					else {
						UTILS_eliminaConexion(&user, i);
						//write(1, USR_NOT_CONN, strlen(USR_NOT_CONN));
					}
				}
				
			}
			//write(1, ERR_REQ_AUDIOS, strlen(ERR_REQ_AUDIOS));
		}
		free(userName);
		free(file);
	}
}


void COMMAND_EXIT(User *user){
	Trama trama;
	int i;

	for (i = (user->numConnections - 1); i == 0; i--) {

		if ((UTILS_sendTrama(user->connections[i].fd, 6, " ", user->name)) > 0) {
			trama = UTILS_readTrama(user->connections[i].fd);

			if (strcmp(trama.header, "CONOK") == 0) {
				close(user->connections[i].fd);
				free(user->connections[i].name);
			}
			free(trama.header);
			free(trama.data);
		}
	}

	free(user->connections);
	free(user->name);
	free(user->connectionIP);
	free(user->directory);
} 


void COMMAND_BROADCAST(char* command, User user){
	char* msg = UTILS_getMSG(command);
	char* buffer;
	
	int espacios = UTILS_getArguments(command);

	if (espacios < 1){
		write(1, COMM_ERROR, strlen(COMM_ERROR));
	}else{
		for(int i = 0; i < user.numConnections; i++){
			int a = UTILS_sendTrama(user.connections[i].fd, 3, "BROADCAST", msg);

			if (a <= 0){
				UTILS_eliminaConexion(&user, i);
			}else{
				Trama trama = UTILS_readTrama(user.connections[i].fd);
				if(trama.type == 3){
					asprintf(&buffer, BROADCAST_ANSWER, user.connections[i].name);
					write(1, buffer, strlen(buffer));
					free(trama.header);
					free(trama.data);
					free(buffer);
				}else if(trama.type == -1){
					UTILS_eliminaConexion(&user, i);

				}
			}
		}	
	}

	free(msg);
	
}


void COMMAND_SAY(char* command, User user){
	
	int espacios = UTILS_getArguments(command);

	if (espacios != 2) {
		write(1, COMM_ERROR, strlen(COMM_ERROR));
	}
	else {
		char* msg = UTILS_getMSG(command);
		char* name = UTILS_getName(command);

		if (strlen(msg) < 1 || name == NULL) {
			write(1, COMM_ERROR, strlen(COMM_ERROR));
		}
		else {
			int i, fd = -1, pos;
			for (i = 0; i < user.numConnections; i++) {
				if (strcmp(user.connections[i].name, name) == 0) {
					fd = user.connections[i].fd;
					pos = i;
					break;
				}
			}
			if (fd == -1) {
				char *buffer = (char*)malloc(sizeof(char));

				asprintf(&buffer, CONN_NO_EXISTS, name);
				write(1, buffer, strlen(buffer));
				free(buffer);
			}
			else {
				int a = UTILS_sendTrama(fd, 2, "MSG", msg);

				if (a == 0) {
					UTILS_eliminaConexion(&user, pos);

					char *buffer = (char*)malloc(sizeof(char));
					asprintf(&buffer, CONN_NO_EXISTS, name);
					write(1, buffer, strlen(buffer));
					free(buffer);
				}
				else {
					Trama trama = UTILS_readTrama(fd);

					if (trama.type == -1) {
						UTILS_eliminaConexion(&user, pos);
						write(1, ERR_MSG, strlen(ERR_MSG));
					}
					else if (strcmp(trama.header, "MSGKO") == 0) {
						write(1, ERR_MSG, strlen(ERR_MSG));
						free(trama.header);
						free(trama.data);
					}
				}
			}
		}
		free(name);
		free(msg);	
	}
}



void COMMAND_showConnections(User user){
	int fd[2];
    pipe(fd);
	pid_t pid = fork();
 
	//int a = 0;
	int z;
	char* buffer;
 	char* buffer2;
 	char* arg[4];
 
	switch (pid) {
 		case -1: //error
 			close(fd[0]);
 			close(fd[1]);
 			write(1, ERR_FORK, strlen(ERR_FORK));
			exit(-1);
        break;
 
		case 0: //hijo
			arg[0] = "./show_connections.sh";
			asprintf(&buffer, "%d", user.initPort);

			arg[1] = buffer;
			asprintf(&buffer2, "%d", user.fiPort);				

			arg[2] = buffer2;
			arg[3] = NULL;
			close(fd[0]);
			write (1, MSG_TESTING, strlen(MSG_TESTING));

			//execvp(arg[0], arg);
			if (dup2(fd[1], 1) != -1){
				if (execvp(arg[0], arg)<0){
					write(1, ERR_EXEC, strlen(ERR_EXEC));
				}
			}

			close(fd[1]);
			exit(1);
		break;

		default: //padre
			wait(NULL);
			close(fd[1]);
			UTILS_mostrarConexiones(fd[0], user);
			close(fd[0]);
		break;

		free(buffer);
		free(buffer2);
		for (z = 0; z < 4; z++){
			free(arg[z]);
		}
	}
}


void COMMAND_connect(char* str, User *user) {
	int espacios = UTILS_getArguments(str);

	if(espacios != 1){
		write(1, COMM_ERROR, strlen(COMM_ERROR));
	}else{	
		write(1, CONN, strlen(CONN));

		int port = UTILS_getPort(str);	
		

		char *buffer;
		int bytes;
		
		struct sockaddr_in addr;
		int sockfd;
		
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(user->ip);
		addr.sin_port = htons(port);

		int len = sizeof(addr);

		if (connect(sockfd, (struct sockaddr *)&addr, len) == -1) {		//no se ha podido conectar
			bytes = asprintf(&buffer, CONN_ERR, port);
			write(1, buffer, bytes);
			
		}
		else {		
			if (UTILS_sendTrama(sockfd, 1, "TR_NAME", user->name) < 0) {
		
				write(1, ERR_MSG, strlen(ERR_MSG));
			}
			Trama trama = UTILS_readTrama(sockfd);
			
			if (strcmp(trama.header, C_OK) == 0) {
				
				user->connections[user->numConnections].name = (char*)malloc(sizeof(char));
				user->connections[user->numConnections].name[0] = '\0';

				user->connections[user->numConnections].name = my_strcat(user->connections[user->numConnections].name, trama.data);
				//my_strcat2(&user->connections[user->numConnections].name, trama.data);
				user->connections[user->numConnections].port = port;
				user->connections[user->numConnections].fd = sockfd;

				user->connections = (Connection*)realloc(user->connections, sizeof(Connection) * (user->numConnections + 2));

				bytes = asprintf(&buffer, CONN_OK, port, user->connections[user->numConnections].name);
				write(1, buffer, bytes);

				user->numConnections++;
			}
			else {
				bytes = asprintf(&buffer, CONN_ERR, port);
				write(1, buffer, bytes);
			}

			free(trama.header);
			free(trama.data);
		}
		free(buffer);
	}
}


void COMMAND_showAudios(char *str, User user) {

	int ok = UTILS_commandAudios(str);
	
	if (ok < 0) {
		write(1, COMM_ERROR, strlen(COMM_ERROR));
	}
	else {
		char* who;
		who = UTILS_getInfoShowAudios(str);
		int index, bytes;
		int fd = -1;
		char *buffer;


		for (index = 0; index < user.numConnections; index++) {
			if(strcmp(user.connections[index].name, who) == 0) {
				fd = user.connections[index].fd;
				break;
			}
		}

		if (fd < 0) {
			bytes = asprintf(&buffer, CONN_NO_EXISTS, who);
			write(1, buffer, bytes);
			free(buffer);
		}
		else {
			
			if (UTILS_sendTrama(fd, 4, "SHOW_AUDIOS", "") <= 0) {
				write(1, ERR_REQ_AUDIOS, strlen(ERR_REQ_AUDIOS));
			}
			else {
				Trama trama = UTILS_readTrama(fd);
				if (trama.type == -1){
					UTILS_eliminaConexion(&user, index);
					write(1, ERR_REQ_AUDIOS, strlen(ERR_REQ_AUDIOS));
				}else if (strcmp(trama.header, "LIST_AUDIOS") == 0){				
					bytes = asprintf(&buffer, AUDIOS, trama.data);
					write(1, buffer, bytes);
					free(buffer);
					free(trama.header); 
					free(trama.data);
				}
			}
		}
		free(who);
	}
	
}
