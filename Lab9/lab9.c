#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// uma thread para cada mensagem
#define NTHREADS 5

// variaveis globais
int x = 0;
sem_t condt2, condt3; // semaforos para sincronizar a ordem de execucao das threads

// Thread 1
void *thread1(void *t)
{
  while (x < 3)
  {
    sem_wait(&condt3);
  }

  printf("Volte sempre!\n");

  pthread_exit(NULL);
}

// Thread 2
void *thread2(void *t)
{
  if (x == 0)
  {
    sem_wait(&condt2);
  }

  printf("Fique a vontade.\n");
  x++;

  sem_post(&condt3);

  pthread_exit(NULL);
}

// Thread 3
void *thread3(void *t)
{
  if (x == 0)
  {
    sem_wait(&condt2);
  }

  printf("Sente-se por favor.\n");
  x++;

  sem_post(&condt3);

  pthread_exit(NULL);
}

// Thread 4
void *thread4(void *t)
{
  if (x == 0)
  {
    sem_wait(&condt2);
  }

  printf("Aceita um copo d'agua?\n");
  x++;

  sem_post(&condt3);

  pthread_exit(NULL);
}

// Thread 5
void *thread5(void *t)
{
  printf("Seja bem-vindo! \n");

  for (int i = 0; i < 3; i++)
  {
    sem_post(&condt2);
  }

  pthread_exit(NULL);
}

// Cria as threads
void createThreads(pthread_t *tids, int nthreads)
{
  int error;

  error = pthread_create(&tids[0], NULL, thread1, NULL);
  error = pthread_create(&tids[1], NULL, thread2, NULL);
  error = pthread_create(&tids[2], NULL, thread3, NULL);
  error = pthread_create(&tids[3], NULL, thread4, NULL);
  error = pthread_create(&tids[4], NULL, thread5, NULL);

  if (error)
  {
    fprintf(stderr, "ERRO--create()\n");
    exit(1);
  }
}

// Espera todas as threads completarem
void joinThreads(pthread_t *tids, int nthreads)
{
  for (int i = 0; i < nthreads; i++)
  {
    if (pthread_join(tids[i], NULL))
    {
      fprintf(stderr, "ERRO--join()\n");
      exit(2);
    }
  }
}

int main(int argc, char *argv[])
{
  pthread_t tids[NTHREADS];

  // inicia os semaforos
  sem_init(&condt2, 0, 0);
  sem_init(&condt3, 0, 1);

  createThreads(tids, NTHREADS);
  joinThreads(tids, NTHREADS);

  return 0;
}