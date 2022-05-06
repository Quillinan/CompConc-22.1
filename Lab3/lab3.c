#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

long long N;
int nthreads;
float *array;

typedef struct{
    float maxVal;
    float minVal;
} arrayValue;

//fluxo das threads
void * tarefa(void * arg){
    long long id = (long int) arg;
    long long int tamBloco = N/nthreads;
    long long int ini = id * tamBloco;
    long long int fim;

    arrayValue * values;
    values = (arrayValue *)malloc(sizeof(arrayValue));

    if(values == NULL){
        fprintf(stderr, "ERRO--malloc\n");
        exit(1);
    }

    if(id == nthreads -1) fim = N;
    else fim = ini+tamBloco;

    values->maxVal = array[ini];
    values->minVal = array[ini];

    for(long long int i=ini; i<fim; i++){
        if(array[i] > values->maxVal){
            values->maxVal = array[i];
        }
        else if(array[i] < values->minVal){
            values->minVal = array[i];
        }
    }

    //retorna o resultado da soma local
    pthread_exit((void*) values);
}

void tarefaSequencial(float **inputArray, arrayValue *seqValue, int Num) {
  for (long long int i = 1; i < Num; i++) {
    if ((*inputArray)[i] > seqValue->maxVal) {
      seqValue->maxVal = (*inputArray)[i];
    }
    if ((*inputArray)[i] < seqValue->minVal) {
      seqValue->minVal = (*inputArray)[i];
    }
  }
}

// aloca memoria para as estruturas de dados
void initThreads(pthread_t **tid, arrayValue *concValue, int numThreads){
    (*tid) = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if (*tid == NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        exit(2);
    }

    // cria as threads
    for(long int i = 0; i < nthreads; i++){
        if(pthread_create((*tid) + i, NULL, tarefa, (void *) i)){
            fprintf(stderr, "ERRO--create\n");
            exit(3);
        }
    }

    arrayValue *threadsValue;

    for(long int i = 0; i < nthreads; i++){
        if(pthread_join(*((*tid) + i), (void **) &threadsValue)){
            fprintf(stderr, "ERRO--join\n");
            exit(4);
        }

        if(threadsValue->maxVal > concValue->maxVal) {
            concValue->maxVal = threadsValue->maxVal;
        }

        if(threadsValue->minVal < concValue->minVal) {
            concValue->minVal = threadsValue->minVal;
        }
    }
}

void checaIgualdade(arrayValue *seqValue, arrayValue *concValue){
    if(seqValue->maxVal == concValue->maxVal && seqValue->minVal == concValue->minVal){
        printf("OK\n");
    }else{
        printf("ERRO-igualdade\n");
    }
}

//fluxo princial
int main(int argc, char *argv[]){
    arrayValue seqValue, concValue;
    double ini, fim, tempSeq, tempConc;
    pthread_t *tid;
    if(argc < 3){
        fprintf(stderr, "Digite: %s <dimensao do vetor> <numero de threads>\n", argv[0]);
        return 1;
    }

    //converte para inteiro
    N = atoi(argv[1]);
    nthreads = atoi(argv[2]);

    array = (float*) malloc(sizeof(float)*N);
    if(array == NULL){
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }

    //preenche o vetor  de entrada
    srand(time(NULL));
    for(long int i=0;i<N;i++)
        array[i] = rand() % N;

    //atribuindo valor das estruturas ao primeiro item do array
    concValue.maxVal = seqValue.maxVal = array[0];
    concValue.minVal = seqValue.minVal = array[0];

    //checagem sequencial dos elementos
    GET_TIME(ini);
    tarefaSequencial(&array, &seqValue, N);
    GET_TIME(fim);
    tempSeq = fim-ini;
    printf("Tempo sequencial: %lf\n", tempSeq);

     //criar as threads
    GET_TIME(ini);
    initThreads(&tid, &concValue, nthreads);
    GET_TIME(fim);
    tempConc = fim-ini;
    printf("Tempo concorrente: %lf\n", tempConc);
    
    //tempo aceleração
    printf("Aceleração: %lf\n", tempSeq/tempConc);

    //prova real da função
    checaIgualdade(&seqValue, &concValue);

    //libere memoria alocada
    free(array);
    free(tid);
    return 0;
}
