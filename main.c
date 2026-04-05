#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h> // (strtok)
#include <stdbool.h> // booleen
#define MAX_INPUT_LENGTH 1000
#define MAX_ARGUMENTS_NUMBER 100
char** parseArgument(char* ch){

	ch[strlen(ch)-1] = 0;

	if(strcmp(ch,"")==0){
		return NULL;
	}
	char *token;
	char **list = malloc(MAX_ARGUMENTS_NUMBER * sizeof(char*));
	// CHANGE TO REALLOC AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	if( list == NULL ){
		printf("malloc failed");
		return NULL;
	}
	int i = 0 ;
	token = strtok(ch," ");
	while(token != NULL && i < MAX_ARGUMENTS_NUMBER -1){
		list[i++] = token;
		token = strtok(NULL," ");
	}
	list[i] = NULL;
	if( token != NULL ){ // MAX ARGUMENTS reached
		printf("Too many arguments\n");
		free(list);
		return NULL;
	}
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
		printf("These shell commands are defined internally. Type '= 'help' to see this list\n");
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
	while(1){
		printf("fstShell> ");
		if( fgets(ch,sizeof(ch),stdin) == NULL ){
			printf("\nExiing fstShell...\n");
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
