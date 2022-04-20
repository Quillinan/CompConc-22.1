
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <math.h>

//total de threads a serem criadas
#define NTHREADS  2 

//tamanho do vetor
#define TAMANHO 10000

//intervalo de cada thread
#define PULO 3

typedef struct {
   int indexInicio;
   int* vetor;
} t_Element;

//Funcao que eleva ao quadrado cada elemento de um vetor.
void * elevar (void  * elt) {

  t_Element* elemento = (t_Element *) elt;
  for(int i = elemento->indexInicio; i < TAMANHO; i += PULO){
    elemento->vetor[i] = pow(argumento->vetor[i], 2);
  }
  pthread_exit(NULL);
}

// Função que verica se os valores finais no vetor estão corretos.
int verificaResultado () {
  for (int i=0; i<TAMANHO; i++)
    if (elemento->vetor[i] == pow(argumento->vetor[i], 2);)
      return 0;

  return 1;
}

// Funcao principal
int main(void) {
  pthread_t tid_sistema[NTHREADS]; // Identificador das threads no sistema
  int thread; // Variavel auxiliar
  int vetor[TAMANHO];
  t_Element* elemento;

  for(thread=0; thread<NTHREADS; thread++) {
    elemento = malloc(sizeof(t_Element));
    elemento->vetor = vetor;
    elemento->indexInicio = thread + 1;
    if (pthread_create(&tid_sistema[thread], NULL, elevar, (void *)elemento)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }
  for(int i = 0; i < TAMANHO; i += PULO){
    elemento->vetor[i] = (elemento->vetor[i]) * (elemento->vetor[i]);
  }
  for (thread=0; thread<NTHREADS; thread++) {
    if (pthread_join(tid_sistema[thread], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    } 
  }

  // Verifica se o resultado está correto
  if (!verificaResultado()) {
    printf("--ERRO: Valor incorreto no vetor\n"); exit(-1);
  }

  printf("--Thread principal terminou\n");

  pthread_exit(NULL);

  return 0;
}