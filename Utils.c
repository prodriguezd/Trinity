#include "Utils.h"
#include <stdio.h>
#include <stdint.h>

#define S "\ntrama: %s"
#define aux2 "\nnumoero length = %d\n"
#define T "trama.data = %s\n"
#define C "\nCHARlimpio = %c\n"



/*char* UTILS_getMD5SUM(dataFile, int length){
	unsigned char* result = (char*)malloc(MD5_DIGEST_LENGTH);
 
  	MD5(string, strlen(string), result);

    return result;
}*/



int UTILS_getArguments(char* command){
	int i = 0;
	int espacios = 0;

	while (command[i] != '\0') {
		if (command[i] == ' ') {
			espacios++;
			i++;
			while (command[i] == ' ') i++;
		}
		if (command[i] == '"') break;
		else i++;	
	}
	return espacios;

}

char* UTILS_limpiaChar(char* name) {
	char* str = (char*) malloc (sizeof(char));
	int i = 0;
	char c = name[i];

	while(c != '\0' && c != ' '){
		str[i] = c;
		str = (char*) realloc (str, sizeof(char) * (i + 2));
		i++;
		c = name[i];
	}

	return str;
}

char* UTILS_getAudioName(char* command) {
	int pos = 0, j = 0, espacio = 0;
	char* audio = (char*)malloc(sizeof(char));

	while(command[pos] != '\0') {
		if (command[pos] == ' ') espacio++;
		pos++;
		if (espacio == 2) {
			break;
		}
	}
	if (espacio == 2) {
		while (command[pos] != '\0') {
			audio[j++] = command[pos++];
			audio = (char*)realloc(audio, sizeof(char) * (j + 1));
		}
	}
	audio[j] = '\0';
	return audio;
}


void UTILS_eliminaConexion(User *user, int pos){

	int i, a;
	for (i = pos; i < user->numConnections; i++){
		for(a = i+1; a < user->numConnections; a++){
			user->connections[i] = user->connections[a];
		}
	}
	//free(user->connections[user->numConnections -1]);
	user->numConnections--;
}

char* UTILS_getBroadcastMSG(char* command){
	int ok = 0;
    int a = 0;
    char c;
    int i = 0;
	char* name = (char*)malloc(sizeof(char));
	
	c = command[i];

    while(c != '\0'){
        if(ok >= 1){
            name[a] = c;
            name = (char*)realloc(name, sizeof(char) * (a + 2));
            a++;
        }

		if(c == ' ') ok++;
		
		i++;
		c = command[i];
    }
    name[a] = '\0';
    return name;
}


char* UTILS_getName(char* command){
	char* name = (char*)malloc(sizeof(char));
	int i = 0, j = 0;
	
	while (command[i] != '\0') {
		if (command[i] == ' ') {
			while (command[i] == ' ') i++;

			while (command[i] != ' ') {
				name[j++] = command[i++];
				name = (char*)realloc(name, sizeof(char) * (j + 1));
			}
			break;
		}
		i++;
	}
	name[j] = '\0';
	return name;
}


char* UTILS_getMSG(char* command){

	int pos = 0, j = 0;
	char* msg = (char*)malloc(sizeof(char));

	while(command[pos] != '\0') {
		if (command[pos] == '"') {
			pos++;
			while(command[pos] != '"') {
				msg[j++] = command[pos++];
				msg = (char*)realloc(msg, sizeof(char) * (j + 1));
			}
			break;
		}
		pos++;
	}
	msg[j] = '\0';
	return msg;
}




char* UTILS_readUntil(int fd, char cFi) {
    int i = 0;
    char c = '0';
    char *buffer = (char*)malloc(sizeof(char));

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


char* UTILS_splitString(char* string, char cFi) {
    char* str1;
    char c;
    int i = 0;
    str1 = (char*)malloc(sizeof(char));

	c = string[i];
    while (c != cFi) {
		if (c == '\0') break;
        str1[i] = c;
        str1 = (char*)realloc(str1, sizeof(char) * (i + 2));
        i++;
		c = string[i];
    }
		
    str1[i] = '\0';
	return str1;
}


int UTILS_getOption(char* command) {
    

    if (strcmp(command, "SHOW CONNECTIONS") == 0 || strcasecmp(command, "SHOW CONNECTIONS") == 0){
		return 1;
	}

    if (strcmp(command, "EXIT") == 0 || strcasecmp(command, "EXIT") == 0){
		return 7;
	}else {
		char* string1 = UTILS_splitString(command, ' ');

        if (strcmp(string1, "CONNECT") == 0 || strcasecmp(string1, "CONNECT") == 0){
			free(string1);
			return 2;
		}

        if (strcmp(string1, "SAY") == 0 || strcasecmp(string1, "SAY") == 0){
			free(string1);
			return 3;
		}

        if (strcmp(string1, "SHOW") == 0 || strcasecmp(string1, "SHOW") == 0){
			free(string1);
			return 5;
		}

        if (strcmp(string1, "DOWNLOAD") == 0 || strcasecmp(string1, "DOWNLOAD") == 0){
			free(string1);
			return 6;
		}

		if (strcmp(string1, "BROADCAST") == 0 || strcasecmp(string1, "BROADCAST") == 0){
			free(string1);
			return 4;
		}

		free(string1);
    }

    return -1;

}


char* my_strcat2(char* dest, char* str2) {
	
	char* ret = (char*)malloc(strlen(dest) + strlen(str2) + 1);
	int i = 0, j = 0;
	while (dest[i] != '\0') {
		ret[i] = dest[i]; 
		i++;
	}

	while (str2[j] != '\0') ret[i++] = str2[j++];

	ret[i] = '\0';
	return ret;
}



char* my_strcat(char* dest, char* str2) {
	
	char* ret = (char*)malloc(strlen(dest) + strlen(str2) + 1);
	int i = 0, j = 0;
	while (dest[i] != '\0') {
		ret[i] = dest[i]; 
		i++;
	}

	free(dest);
	while (str2[j] != '\0') ret[i++] = str2[j++];

	ret[i] = '\0';
	return ret;
}
char* my_strcatAudio(char* dest, char* str2) {
	
	char* ret = (char*)malloc(sizeof(dest) + sizeof(str2) + 1);
	int i = 0, j = 0;
	while (dest[i] != '\0') {
		ret[i] = dest[i]; 
		i++;
	}

	free(dest);
	while (str2[j] != '\0') ret[i++] = str2[j++];

	ret[i] = '\0';
	return ret;
}



void  UTILS_mostrarConexiones(int puertos, User user){
	char* final = (char*) malloc(sizeof(char));
	final[0] = '\0';
	char* buffer = (char*) malloc(sizeof(char));

	char aux[2];
	aux[0] = '\n';
	aux[1] = '\0';

	char temp[2];
	temp[0] = '\t';
	temp[1] = '\0';
	
	int cont = 0;
	int num = 0;
	int finalIndex = 0;
 	int i = 0;
	char c;
	int espacio = 0;

    while (read(puertos, &c, 1) > 0) {
		
 		if (c == ' ') {
			if (espacio == 1){
				for(int z = 0; z < 4; z++){
               		num += (buffer[z] - '0') * pow(10,(3-z));
                }

		   		if(num != user.port){
					
					final = my_strcat(final, buffer);
					//my_strcat2(&final, buffer);
					
					for (int j = 0; j < user.numConnections; j++) {
						if (user.connections[j].port == num) {
							
							final = my_strcat(final, temp);
							//my_strcat2(&final, temp);
							final = my_strcat(final, user.connections[j].name);
							//my_strcat2(&final, user.connections[j].name);
							finalIndex = strlen(final);
						}
					}

					final = my_strcat(final, aux);
					//my_strcat2(&final, aux);
					finalIndex = strlen(final);
 				}
				num = 0;
			}
			i = 0;
			free(buffer);
        	buffer = (char*)malloc(sizeof(char));
			espacio++;
		}else{
			if(c == '\n'){
				cont++;
				espacio = 0;
			}else{
				buffer[i] = c;
				i++;
				buffer = (char*)realloc(buffer, sizeof(char) * (i + 1));
				buffer[i] = '\0';
			}
		}
	}
    
    final[finalIndex] = '\0';
	
	free(buffer);

	int bytes = asprintf(&buffer, MSG_CONNECTIONS_AVAILABLE, cont-1);
    write(1, buffer, bytes);
	
	if (finalIndex > 0) write(1, final, strlen(final));
	
	free(final);
	free(buffer);
	
}



int UTILS_getPort(char *str) {
	const char delim[2] = " ";
	char *token = strtok(str, delim);

	token = strtok(NULL, delim);
	if (token != NULL) return atoi(token);
	return -1;

	
}



void swap(char *x, char *y) {
	char t = *x;
	*x = *y;
	*y = t;
}

char* reverse(char* str, int i, int j) {
    while (i < j) {
        swap(&str[i++], &str[j--]);
    }
    return str;

}

char* itoa(int num, int base) {
    int i=0;
	char* str = (char*)malloc(sizeof(char));
	if (num <= 9 && base == 10) {
		str=(char*)realloc(str, sizeof(char) * (i + 3));
		int x = num % base;
		str[i++] = '0';
		str[i++] = 48 + x;
		str[i] = '\0';
		return str;

	}else{
		while (num) {
			str=(char*)realloc(str, sizeof(char) * (i + 2));
			int r = num % base;
			if(r >= 10)	str[i++] = 65 + (r -10);
			else str[i++] = 48 + r;

			num = num / base;
		}

		if(i == 0) str[i++] = '0';

		str[i]='\0';
		return reverse(str, 0, i-1);
	}
}

int UTILS_sendTramaAudios(int fd, char *data, int size) {
	int a;
	char* buffer;
	short finalSize = size;

	int bytes = asprintf(&buffer, TRAMA_AUDIO, itoa(5, 16), "AUDIO_RSPNS");
	//a = write(1, buffer, bytes);

	a = write(fd, buffer, bytes);
	sleep(0.01);
	write(fd, &finalSize, sizeof(short));
	//printf("size enviado: %s\n", buffer);
	
	write(fd, data, finalSize);
	//write(1, "data enviado\n", strlen("data enviado\n"));


	free(buffer);

	if (a == bytes) {
		return 1;
	}else if(a < 0){
		return 0;
	}

	return 1;
}

int UTILS_sendTrama(int fd, int t, char *header, char *data) {
	int a;
	char* type;
	char* s;
	char* buffer;

	switch (t) {
		case 1:
			type = itoa(1, 16);
			break;
		case 2:
			type = itoa(2, 16);
			break;
		case 3: 
			type = itoa(3, 16);
			break;
		case 4:
			type = itoa(4, 16);
			break;
		case 5:
			type = itoa(5, 16);
			
			break;
		case 6:
			type = itoa(6, 16);
			break;
	}
	
	int i;
	
	if (t == 5 && strcmp(header, "AUDIO_RSPNS") ==0) {
		i = sizeof(data);
		s = itoa(i, 10);
	}
	else {
		i = strlen(data);
		s = itoa(i, 10);
	}

	int bytes = asprintf(&buffer, TRAMA, type, header, s, data);
	//a = write(1, buffer, bytes);

	a = write(fd, buffer, bytes);
	free(buffer);
	free(type);
	free(s);

	if (a == bytes) {
		return 1;
	}else if(a < 0){
		return 0;
	}

	return 1;
}

Trama UTILS_readTrama(int fd) {
	Trama trama;
	char* r = (char*)malloc(2);
	int index = 0;

	if(read(fd, &r[index], 1) <= 0) { //show connections
		trama.type = -1;
		free(r);
		return trama;
	}

	r[++index] = '\0';
	trama.type = atoi(r);

	read(fd, &r[index], 1); //'['
	trama.header = UTILS_readUntil(fd, ']'); 


	if (trama.type == 5 && strcmp(trama.header,  "AUDIO_RSPNS") == 0) {
		//trama.data = malloc(trama.length);
		//read(fd, trama.data, trama.length);
	}
	else {
		r = (char*)realloc(r, 3);
		read(fd, r, 2);
		index = 2;
		r[index] = '\0';

		trama.length = atoi(r);

		trama.data = (char*)malloc(sizeof(char) * (trama.length + 1));
		read(fd, trama.data, trama.length);
		trama.data[trama.length] = '\0';
	}

	free(r);

	return trama;
	
}

int UTILS_checkDirectory(char* directoryName, char* userName) {
	DIR* dir = opendir(userName);
	char* directory = (char*)malloc(sizeof(char));
	directory[0] = '\0';

	char *aux = (char*)malloc(sizeof(char) * 2);
	aux[0] = '/';
	aux[1] = '\0';

	int result;
	if (dir == NULL) {
		result = mkdir(userName, 0777);

		if (result < 0){
			closedir(dir);
			free(directory);
			free(aux);
			return 0;
		}

		directory = my_strcat(directory, userName);
		//my_strcat2(&directory, userName);
		directory = my_strcat(directory, aux);
		//my_strcat2(&directory, aux);
		directory = my_strcat(directory, directoryName);
		//my_strcat2(&directory, directoryName);

		result = mkdir(directory, 0777);
		
		if(result < 0){
			closedir(dir);
			free(aux);
			free(directory);
			return 0;
		}
		
	}
	closedir(dir);
	free(directory);
	free(aux);


	return 1;
}


char* my_strcpy(char* str) {
	char* ret = (char*)malloc(sizeof(char) * strlen(str) + 1);
	int i = 0;

	while (str[i] != '\0') {
		ret[i] = str[i];
		i++;
	}
	ret[i] = '\0';
	return ret;
}



char* UTILS_getMD5SUMbyData(char* data){
	int fd[2];
    pipe(fd);
	pid_t pid = fork();
 
	int a = 0;
	int z;
	char* buffer;
 	char* arg[4];
	char* md5sum;
 
	switch (pid) {
 		case -1: //error
 			close(fd[0]);
 			close(fd[1]);
 			write(1, ERR_FORK, strlen(ERR_FORK));
			exit(-1);
        break;
 
		case 0: //hijo
			arg[0] = "md5sum";
			arg[1] = "<<<";
			buffer = my_strcat2("\"", data);
			buffer = my_strcat(buffer, "\"");
			//my_strcat2(&buffer, "\"");
			arg[2] = buffer;
			//arg[2] = buffer;
			arg[3] = NULL;
			close(fd[0]);

			a = dup2(fd[1], 1);
			if (a != -1){
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
			
			md5sum = UTILS_readUntil(fd[0], ' ');
			close(fd[0]);
		break;

		free(buffer);
		for (z = 0; z < 4; z++){
			free(arg[z]);
		}
	}

	return md5sum;
}




char* UTILS_getMD5SUMbyFile(char* data){
	int fd[2];
    pipe(fd);
	pid_t pid = fork();
 
	int a = 0;
	//int z;
 	char* arg[3];
	char* md5sum;
 
	switch (pid) {
 		case -1: //error
 			close(fd[0]);
 			close(fd[1]);
 			write(1, ERR_FORK, strlen(ERR_FORK));
			exit(-1);
        break;
 
		case 0: //hijo
			arg[0] = "md5sum";
			arg[1] = data;
			arg[2] = NULL;
			close(fd[0]);
			

			a = dup2(fd[1], 1);
			if (a != -1){
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
			
			md5sum = UTILS_readUntil(fd[0], ' ');
			close(fd[0]);
		break;

		/*for (z = 0; z < 3; z++){
			free(arg[z]);
		}*/
	}

	return md5sum;
}






char* UTILS_getAudios(char* directory, char* userName) {

	char *directoryName = (char*)malloc(sizeof(char));
	directoryName[0] = '\0';
	

	char aux[2];
	aux[0] = '/';
	aux[1] = '\0';

	directoryName = my_strcat(directoryName, userName);
	//my_strcat2(&directoryName, userName);
	directoryName = my_strcat(directoryName, aux);
	//my_strcat2(&directoryName, aux);
	directoryName = my_strcat(directoryName, directory);
	//my_strcat2(&directoryName, directory);

	DIR *dir;
	struct dirent *ent;
	char *audios = (char*)malloc(sizeof(char));
	audios[0] = '\0';

	if ((dir = opendir(directoryName)) != NULL) {
		
		ent = readdir(dir);
		ent = readdir(dir);
		aux[0] = '\n';
		while ((ent = readdir(dir)) != NULL) {
			audios = my_strcat(audios, ent->d_name);
			//my_strcat2(&audios, ent->d_name);
			audios = my_strcat(audios, aux);
			//my_strcat2(&audios, aux);
		}
		closedir(dir);
	}
	free(ent);
	free(directoryName);
	return audios;
	
	/*char* audios = (char*)malloc(sizeof(char));
	write(1, "HOLA\n", strlen("HOLA\n"));
	char* directoryName = (char*) malloc (sizeof(char));
	directoryName[0] = '\0';

	write(1, "HOLA2\n", strlen("HOLA2\n"));
	char buffer[150];
	sprintf(buffer, "sizeof de userName+2 %ld\n sizeof del directoryName = %ld\n", strlen(userName) + 2, strlen(directoryName));
	write(1, buffer, strlen(buffer));
	directoryName = (char*)realloc(directoryName, (strlen(userName) + 4));
	strcat(directoryName, userName);
	strcat(directoryName, "/");

	//directoryName = UTILS_concat(userName, "/");
	write(1, "HOLA3\n", strlen("HOLA3\n"));


	directoryName = (char*)realloc(directoryName, sizeof(char) * (sizeof(directoryName) + sizeof(directory)));
	strcat(directoryName, directory);
	//UTILS_concatV2(&directoryName, directory);
	write(1, "HOLA4\n", strlen("HOLA4\n"));

	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir(directoryName)) != NULL) {
		
		ent = readdir(dir);
		ent = readdir(dir);
		while ((ent = readdir(dir)) != NULL) {
			UTILS_concatV2(&audios, ent->d_name);
			UTILS_concatV2(&audios, "\n");
		}
		closedir(dir);
	}
	free(ent);
	free(directoryName);
	write(1, audios, strlen(audios));
	return audios;*/
}


int UTILS_commandAudios(char* str) {
	int i = 0, j = 0;
	char *aux = (char*)malloc(sizeof(char));
	int ok = -1;

	while (str[i] != '\0') {
		if (str[i] == ' ') {
			while (str[i] == ' ' && str[i] != '\0') i++;

			while (str[i] != ' ' && str[i] != '\0') {
				aux[j++] = str[i++];
				aux = (char*)realloc(aux, sizeof(char) * (j + 1));
			}
			aux[j] = '\0';

			if (strcmp(aux, "AUDIOS") == 0 || strcasecmp(aux, "AUDIOS") == 0) {
				ok = 1;
			}
			break;
		}
		i++;
	}
	free(aux);
	return ok;
}

char* UTILS_getInfoShowAudios(char *str) {
	char* who = (char*)malloc(sizeof(char));
	int i = 0, j = 0, ready = 0;
	while(str[i] != '\0') {
		if (str[i] == ' ') {
			while (str[i] == ' ') i++;

			while (str[i] != ' ') i++;

			while (str[i] == ' ') i++;

			ready = 1;
			break;
		}
		i++;
	}

	while(str[i] != '\0' && ready == 1) {
		who[j++] = str[i++];
		who = (char*)realloc(who, sizeof(char) * (j + 1));
	}
	who[j] = '\0';
	return who;
}
