#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

long long int dim, global = 0;
int nthreads, *vetor;
float *vetorSeq, *vetorConc;
pthread_mutex_t lock;

int ehPrimo(long long int n) {
  if (n<=1) return 0;
  if (n==2) return 1;
  if (n%2==0) return 0;
  for (int i=3; i<sqrt(n)+1; i+=2)
    if(n%i==0) return 0;
  return 1;
}

void PrimosSeq(int *vetor, long long int dim) {
  for(long long int i=0; i<dim; i++) {
    if (ehPrimo(vetor[i]))
      vetorSeq[i] = sqrt(vetor[i]);
    else
      vetorSeq[i] = vetor[i];
  };
}

void* PrimosConc() {
    pthread_mutex_lock(&lock);
    int g = global; 
    global++;
    pthread_mutex_unlock(&lock);

    while (g<dim) {
        if (ehPrimo(vetor[g]))
            vetorConc[g] = sqrt(vetor[g]);
        else
            vetorConc[g] = vetor[g];
        
        pthread_mutex_lock(&lock);
        g = global;
        global++;
        pthread_mutex_unlock(&lock);
    }
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t *tid;
    long int t;
    double ini, fim;

    if(argc<3) {
        fprintf(stderr, "Digite: %s <numero de elementos> <numero de threads>\n", argv[0]);
        return 3;
    }

    dim = atoll(argv[1]);
    nthreads = atoi(argv[2]);

    vetor = (int*) malloc(sizeof(int)*dim);
    if(vetor == NULL) { fprintf(stderr, "ERRO--malloc--vetor\n"); 
        return 4;
    }

    vetorSeq = (float*) malloc(sizeof(float)*dim);
    if(vetorSeq == NULL) { fprintf(stderr, "ERRO--malloc--vetorSeq\n"); 
        return 5; 
    }
    
    vetorConc = (float*) malloc(sizeof(float)*dim);
    if(vetorConc == NULL) { fprintf(stderr, "ERRO--malloc--vetorConc\n"); 
        return 6; 
    }

    srand(time(NULL));

    for(long int i = 0; i < dim; i++) {
        vetor[i] = rand() % dim;
    };

    
    GET_TIME(ini);
    PrimosSeq(vetor, dim);
    GET_TIME(fim);
    printf("Tempo sequencial: %lf\n", fim - ini);

    GET_TIME(ini);
    pthread_mutex_init(&lock, NULL);

    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if( tid == NULL ) { fprintf(stderr, "ERRO--malloc--tid\n"); 
        return 7;
    }

    for(t=0; t<nthreads; t++) {
        if (pthread_create(tid+t, NULL, PrimosConc, NULL)) {
        printf("--ERRO: pthread_create()\n"); 
            return 8;
        };
    };

    for (t=0; t<nthreads; t++) {
        if (pthread_join(*(tid+t), NULL)) {
            printf("--ERRO: pthread_join() \n"); 
                return 9; 
        } 
    } 
    
    pthread_mutex_destroy(&lock);

    GET_TIME(fim);
    printf("Tempo concorrente: %lf\n", fim - ini);
    
    free(vetor);
    free(tid);
    
    return 0;
}

