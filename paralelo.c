#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "paralelo.h"
#include "entradaSaida.h"

void BMHSParalelo(char *texto, int tamanhoTexto, char *padrao, int tamanhoPadrao, FILE* arq){

    int i;
	pthread_t THREADS[NTHREADS];
	thread_arg argument[NTHREADS];

	for(i = 0; i < NTHREADS; i++){
    if(i != 0)
		  argument[i].inicio = argument[i-1].fim+1;
    else
      argument[i].inicio = tamanhoPadrao;
		argument[i].fim = ((i+1)*tamanhoTexto)/NTHREADS;
    if(argument[0].d[argument[i].fim] < tamanhoPadrao+1)
      argument[i].fim += argument[0].d[argument[i].fim];
    argument[i].arq = arq;
    argument[i].tamanhoTexto = tamanhoTexto;
    argument[i].tamanhoPadrao = tamanhoPadrao;
    argument[i].texto = AlocaString(tamanhoTexto);
    argument[i].padrao = AlocaString(tamanhoPadrao);
    strcpy(argument[i].texto,texto);
    strcpy(argument[i].padrao,padrao);
		pthread_create(&(THREADS[i]), NULL,func, &(argument[i]));
	}

	for(i = 0; i < NTHREADS; i++){
		pthread_join(THREADS[i],NULL);
	}
  
  for(i = 0; i < NTHREADS; i++){
    liberaStrings(argument[i].padrao,argument[i].texto);
	}

}
void *func(void *arg){
	ptr_thread_arg argument = (ptr_thread_arg) arg;
  int flag = 0, i, k, j = argument->inicio;

  for (i = 0; i <= MAXCHAR; i++)
    argument->d[i] = argument->tamanhoPadrao + 1;
  for (i = 1; i <= argument->tamanhoPadrao; i++)
    argument->d[argument->padrao[i-1]] = argument->tamanhoPadrao - i + 1;

  while (j <= argument->fim) {
    // printf("\n %s ",argument->texto);
    k = j;
    i = argument->tamanhoPadrao;
    while (argument->texto[k-1] == argument->padrao[i-1] && i > 0) {
      k--;
      i--;
    }
    if (i == 0){
      flag = 1;
      fprintf(argument->arq, "Casamento na posicao: %d\n", k+1);
    }
    j += argument->d[argument->texto[j]];
  }
  if (flag == 0)
    fprintf(argument->arq, "Não há casamento entre o caracter %d e %d.\n",argument->inicio,argument->fim);
}
