#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h> // (strtok)
#include <stdbool.h> // booleen
#define MAX_INPUT_LENGTH 1000

char** parseArgument(char* ch){

    int len = strlen(ch);
    if (len > 0 && ch[len - 1] == '\n') {
        ch[len - 1] = '\0';
    }
	if(strcmp(ch,"")==0){
		return NULL;
	}
	char *token;
	int local_max_arguments = 10;
	char **list = malloc(local_max_arguments * sizeof(char*));
	if( list == NULL ){
		printf("malloc failed\n");
		return NULL;
	}
	int i = 0 ;
	token = strtok(ch," ");
	while(token != NULL ){
		list[i++] = token;
		if(i >= local_max_arguments - 1){
			local_max_arguments += 5; // increment memory size by 5 * sizeof(char*) = 40 bytes
			char **temp  = realloc(list,local_max_arguments * sizeof(char*));
			if( temp == NULL ){
				printf("Ran out of memory for arguments\n");
				free(list);
				return NULL;
			}
			list = temp;
		}
		token = strtok(NULL," ");
	}
	list[i] = NULL;
	return list;
}

int internalCommand(char** args){
	if( strcmp(args[0],"exit") == 0 ){
		printf("Exiting fstShell...\n");
		exit(0);
	}
	else if( strcmp(args[0],"cd") == 0 ){
		if(args[1] != NULL && args[2] != NULL ){ // cd can't have more than one argument
			printf("Too many arguments!\n");
			return -1;
		}

		char* dest = args[1];
		if(dest == NULL){ // If no destination in arguments cd back to home repository
			dest = getenv("HOME");
		}

		if(chdir(dest) == -1){
			printf("Something went wrong when changing repositories\n");
			return -1;
		}
	}else if( strcmp(args[0],"help") == 0 ){
		printf("fstShell, version 1.0 (x86_64-pc-linux-fstShell)\n");
		printf("These shell commands are defined internally. Type 'help' to see this list\n");
		printf("help\t\t\t");
		printf("exit\n");
		printf("cd [DEST]\t\t\t");
		printf("\n");
	}
	return 0;
}
int externalCommand(char** args){
	pid_t pid = fork();
	if( pid < 0 ){
		perror("fork failed");
		exit(1);
	}
	if( pid == 0 ){
		// --- CHILD PROCESS --- //
		if(execvp(args[0],args) == -1){
			perror("Command not found");
			exit(1);
		}
	}else{
		// --- PARENT PROCESS --- //
		int status;
		waitpid(pid,&status,0);
	}
	return 0 ;
}
int isInternal(char* arg){
	// The only internal commands are cd, exit or help
	return (!strcmp(arg,"cd") || !strcmp(arg,"exit") || !strcmp(arg,"help")) ;
}

int main(){
	char ch[MAX_INPUT_LENGTH];
	char **args;
	char *pwd;
	while(1){
		pwd = getcwd(NULL, 0);
		if (!pwd)
			printf("Can't find current pwd (getcwd() error)\n");

		free(pwd);
		printf("fstShell:~%s> ",pwd ? pwd : "");
		if( fgets(ch,sizeof(ch),stdin) == NULL ){
			printf("\nExiting fstShell...\n");
			break;
		}
		args = parseArgument(ch);
		if(args != NULL){
			if( isInternal(args[0]) )
				internalCommand(args);
			else
				externalCommand(args);
			free(args);
		}
	}
	return 0;
}
