#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX 256

int main() {
  char comando[MAX], *token;
  int pid, i;

  while (1) {
    printf("> ");
    fgets(comando, MAX, stdin);

    comando[strlen(comando) - 1] = '\0';

    char** argumentos = calloc(MAX + 1, sizeof(char *));
    
    token = strtok(comando, " ");
    for(i = 0; token != NULL; i++){
        argumentos[i] = token;
        token = strtok(NULL, " ");
    }
    argumentos[i] = NULL;
    
    if (!strcmp(comando, "exit")) {
      exit(EXIT_SUCCESS);
    }

    if(i <= 0) continue;

    int ampersand = !strcmp(argumentos[i - 1], "&");

    if(ampersand) 
      argumentos[--i] = NULL;

    int changeOut = 0;
    if(i >= 2)
      changeOut = !strcmp(argumentos[i - 2], ">");

    int changeIn = 0;
    if(i >= 2)
      changeIn = !strcmp(argumentos[i - 2], "<");

    pid = fork();
    if(pid && ampersand) continue;

    if (pid) {
      waitpid(pid, NULL, 0);
    } else {
      if(changeOut){
        freopen(argumentos[i - 1], "w", stdout);
        argumentos[i - 2] = NULL;
      }
      else if(changeIn){
        freopen(argumentos[i - 1], "r", stdin);
        argumentos[i - 2] = NULL;
      }

      execvp(argumentos[0], argumentos);
      printf("Erro ao executar comando!\n");
      exit(EXIT_FAILURE);
    }
  }
}
