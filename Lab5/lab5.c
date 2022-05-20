#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

// uma thread para cada mensagem
#define NTHREADS 5

// variaveis globais
int x = 0;
pthread_mutex_t x_mutex; 
pthread_cond_t x_cond, y_cond;

void *VolteSempre (void *t) {
  pthread_mutex_lock(&x_mutex);
  while (x < 4) {
    pthread_cond_wait(&y_cond, &x_mutex);
  }
  pthread_mutex_unlock(&x_mutex); 

  printf("Volte sempre!\n");
  
  pthread_exit(NULL);
}

void *AVontade (void *t) {
  pthread_mutex_lock(&x_mutex);
  if (x == 0) {
    pthread_cond_wait(&x_cond, &x_mutex);
  }
  pthread_mutex_unlock(&x_mutex); 

  printf("Fique a vontade.\n");

  pthread_mutex_lock(&x_mutex);
  x++;
  pthread_cond_signal(&y_cond);
  pthread_mutex_unlock(&x_mutex); 

  pthread_exit(NULL);
}

void *SenteSe (void *t) {
  pthread_mutex_lock(&x_mutex);
  if (x == 0) {
    pthread_cond_wait(&x_cond, &x_mutex);
  }
  pthread_mutex_unlock(&x_mutex); 

  printf("Sente-se por favor.\n");

  pthread_mutex_lock(&x_mutex);
  x++; 
  pthread_cond_signal(&y_cond);
  pthread_mutex_unlock(&x_mutex); 

  pthread_exit(NULL);
}

void *CopoAgua (void *t) {
  pthread_mutex_lock(&x_mutex);
  if (x == 0) {
    pthread_cond_wait(&x_cond, &x_mutex);
  }
  pthread_mutex_unlock(&x_mutex); 

  printf("Aceita um copo d'agua?\n");

  pthread_mutex_lock(&x_mutex);
  x++; // atualiza o total de mensagens
  pthread_cond_signal(&y_cond); // sinaliza caso a thread 1 esteja esperando
  pthread_mutex_unlock(&x_mutex); 

  pthread_exit(NULL);
}

void *BemVindo (void *t) {
  printf("Seja bem-vindo! \n");

  pthread_mutex_lock(&x_mutex);
  x++; // atualiza o total de mensagens
  pthread_cond_broadcast(&x_cond); // sinaliza as outras threads que estejam esperando para que possam imprimir
  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}

// criar as threads
void createThreads(pthread_t *tids, int nthreads) {
  int error;

  error = pthread_create(&tids[0], NULL, AVontade, NULL);
  error = pthread_create(&tids[1], NULL, BemVindo, NULL);
  error = pthread_create(&tids[2], NULL, SenteSe, NULL);
  error = pthread_create(&tids[3], NULL, CopoAgua, NULL);
  error = pthread_create(&tids[4], NULL, VolteSempre, NULL);

  if (error) {
    fprintf(stderr, "ERRO--pthread_create()\n");
    exit(1);
  }
}

// aguarda o termino das threads
void joinThreads(pthread_t *tids, int nthreads) {
  for (int i = 0; i < nthreads; i++) {
    if (pthread_join(tids[i], NULL)) {
      fprintf(stderr, "ERRO--pthread_join()\n");
      exit(2);
    }
  }
}

pthread_mutex_t x_mutex; 
pthread_cond_t x_cond, y_cond;

// inicializa as variaveis de exclusao mutua e condicao
void init(pthread_mutex_t *x_mutex, pthread_cond_t *x_cond, pthread_cond_t *y_cond) {
  pthread_mutex_init(x_mutex, NULL);
  pthread_cond_init (x_cond, NULL);
  pthread_cond_init (y_cond, NULL);
}

// desaloca as variaveis criadas
void destroy(pthread_mutex_t *x_mutex, pthread_cond_t *x_cond, pthread_cond_t *y_cond) {
  pthread_mutex_destroy(x_mutex);
  pthread_cond_destroy(x_cond);
  pthread_cond_destroy(y_cond);
}

int main(int argc, char *argv[]) {
  pthread_t tids[NTHREADS];
  
  init(&x_mutex, &x_cond, &y_cond);
  createThreads(tids, NTHREADS);
  joinThreads(tids, NTHREADS);
  destroy(&x_mutex, &x_cond, &y_cond);

  return 0;
}