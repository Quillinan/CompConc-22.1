#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Total de threads a serem criadas
#define NUM_THREADS 2

// tamanho do vetor
#define SIZE_VETOR 10000

// Vetor de elementos (variavel global)
int vetor[SIZE_VETOR];

/*
 * Handler das threads.
 * Funcao que eleva ao quadrado cada elemento de um vetor.
 */
void * eleva (void * arg) {
  int id_thread = * (int *) arg;

   for (int i=id_thread*(SIZE_VETOR/NUM_THREADS); i<(id_thread+1)*(SIZE_VETOR/NUM_THREADS); i++)
     vetor[i] = (i+1) * (i+1);

  pthread_exit(NULL);
}

/*
 * Funcao que verica se os valores finais no vetor estao corretos.
 *
 * @return 1 se os valores estiverem corretos, 0 c.c.
 */
int verificaResultado () {
  for (int i=0; i<SIZE_VETOR; i++)
    if (!(vetor[i] == (i+1)*(i+1)))
      return 0;

  return 1;
}

// Funcao principal
int main () {
  pthread_t tid_sistema[NUM_THREADS]; // Identificador das threads no sistema
  int tid_local[NUM_THREADS]; // Identificadores locais das threads
  int thread; // Variavel auxiliar que guarda o id das threads

  // Inicializa o vetor de elementos
  for (thread=0; thread<SIZE_VETOR; thread++)
    vetor[thread] = thread + 1; // vetor comeca do 1 

  // Cria as threads passando a funcao eleva como referencia
  for (thread=0; thread<NUM_THREADS; thread++) {
    tid_local[thread] = thread;

    if (pthread_create(&tid_sistema[thread], NULL, eleva, (void *) &tid_local[thread])) {
      printf("--ERRO: pthread_create\n"); exit(-1);
    }
  }

  printf("--Thread principal terminou\n");
  pthread_exit(NULL);

  return 0;
}
