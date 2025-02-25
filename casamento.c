#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "casamento.h"

void ShiftAnd(char *texto, int tamanhoTexto, char *padrao, int tamanhoPadrao, FILE* arq){

  int M[MAXCHAR], R = 0, flag = 0;
  for (int i = 0; i < MAXCHAR; i++)
    M[i] = 0;
  for (int i = 1; i <= tamanhoPadrao; i++)
    M[padrao[i-1] + 127] |= 1 << (tamanhoPadrao - i);
  for (int i = 0; i < tamanhoTexto; i++) {
    R = (((R) >> 1) | (1 << (tamanhoPadrao - 1))) & M[texto[i] + 127];
    if ((R & 1) != 0){
      flag = 1;
      fprintf(arq, "Casamento na posicao: %d\n", i - tamanhoPadrao + 2);
    }
  }
  if (flag == 0)
    fprintf(arq, "Não há casamento.\n");
}


void ShiftAndAproximado(char *texto, int tamanhoTexto, char *padrao, int tamanhoPadrao, int k, FILE* arq){

  int M[MAXCHAR], i, j, Ri, Rant, Rnovo;
  int R[tamanhoPadrao + 1], flag = 0;

  for (i = 0; i < MAXCHAR; i++)
    M[i] = 0;
  for (i = 1; i <= tamanhoPadrao; i++)
    M[padrao[i-1] + 127] |= 1 << (tamanhoPadrao - i);


  R[0] = 0;
  Ri = 1 << (tamanhoPadrao - 1);

  for (i = 1; i <= k; i++)
    R[i] = (1 << (tamanhoPadrao - i)) | R[i-1];
  for (i = 0; i < tamanhoTexto; i++){
    Rant = R[0];
    Rnovo = (((Rant) >> 1) | Ri) & M[texto[i] + 127];
    R[0] = Rnovo;
    for (j = 1; j <= k; j++) {
      Rnovo = ((R[j] >> 1) & M[texto[i] + 127]) | Rant | (((Rant | Rnovo)) >> 1);
      Rant = R[j];
      R[j] = Rnovo | Ri;
    }

    if ((Rnovo & 1) != 0){
      flag = 1;
      fprintf(arq, "Casamento na posicao: %d\n", i+1);
    }
  }
  if (flag == 0)
    fprintf(arq, "Não há casamento.\n");
}


void BMH(char *texto, int tamanhoTexto, char *padrao, int tamanhoPadrao, FILE* arq) {

  int i, j, k, d[MAXCHAR + 1], flag = 0;
  for (i = 0; i <= MAXCHAR; i++)// seta todos valores da tabela de deslocamneto para tamanhoPadrao
    d[i] = tamanhoPadrao;
  for (i = 1; i < tamanhoPadrao; i++)
    d[padrao[i-1]] = tamanhoPadrao - i; // seta na posição relativa ao caractere do padrao o valor de tamanhoPadrao - i

  j = tamanhoPadrao;
  while (j <= tamanhoTexto) {
    k = j;
    i = tamanhoPadrao;
    while (texto[k-1] == padrao[i-1] && i > 0) {
      k--;
      i--;
    }
    if (i == 0){
      flag = 1;
      fprintf(arq, "Casamento na posicao: %d\n", k+1);
    }
    j += d[texto[j-1]];
  }
  if (flag == 0)
    fprintf(arq, "Não há casamento.\n");
}


void BMHS(char *texto, int tamanhoTexto, char *padrao, int tamanhoPadrao, FILE* arq) {

  int i, j, k, d[MAXCHAR + 1], flag = 0;
  for (i = 0; i <= MAXCHAR; i++)
    d[i] = tamanhoPadrao + 1;
  for (i = 1; i <= tamanhoPadrao; i++)
    d[padrao[i-1]] = tamanhoPadrao - i + 1;

  j = tamanhoPadrao;
  while (j <= tamanhoTexto) {
    k = j;
    i = tamanhoPadrao;
    while (texto[k-1] == padrao[i-1] && i > 0) {
      k--;
      i--;
    }
    if (i == 0) {
      flag = 1;
      fprintf(arq, "Casamento na posicao: %d\n", k+1);
    }
    j += d[texto[j]];
  }
  if (flag == 0)
    fprintf(arq, "Não há casamento.\n");
}
