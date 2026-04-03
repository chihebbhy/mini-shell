#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h> // (strtok)
#include <stdbool.h> // booleen
#define MAX_INPUT_LENGTH 1000

char** parseArgument(char* ch){

    if(strcmp(ch,"")==0){
        return null;
    }
    strtok(ch," ");
    while(strtok())

    return null;
}


int main(){
    char ch[MAX_INPUT_LENGTH];
    while(1){
            printf("minishell> ");
            scanf("%s",ch);
            printf("%d\n",strcmp(ch,""));
        }
    return 0;
    }
