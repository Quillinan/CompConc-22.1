#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

long long N;
int nthreads;
float *array;

typedef struct{
    float minVal;
    float maxVal
} arrayValue;

void * tarefa(void * arg){
    long long id = (long int) arg;
    long long int tamBloco = N/nthreads;
    long long int ini = id * tamBloco;
    long long int fim;

    arrayValue * values;
    values = (arrayValue *)malloc(sizeof(arrayValue));

    if(values == NULL){exit(1);}

    if(id == nthreads -1) fim = N;
    else fim = ini+tamBloco;

    values->minVal = array[ini];
    values->maxVal = array[ini];
    for(long long int i=ini; i<fim; i++){
        if(array[i] < values->minVal){
            values->minVal = array[i];
        }
        else if(array[i] > values->maxVal){
            values->maxVal = array[i];
        }
    }

    pthread_exit((void*) values);
}

void tarefaSequencial(float **inputArray, arrayValue *seqValue, int numE){
    for (long long int i = 1 ; i < numE; i++){
        if ((*inputArray[i] < seqValue->minVal)){
            seqValue->minVal = (*inputArray)[i];
        }
        if ((*inputArray[i] > seqValue->maxVal)){
            seqValue->maxVal = (*inputArray)[i];
        }
    }
}

void initThreads(pthread_t **tid, arrayValue *concValue, int numThreads){
    (*tid) = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if (*tid == NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        exit(2);
    }

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

        if(threadsValue->minVal > concValue->minVal) {
            concValue->minVal = threadsValue->minVal;
        }

        if(threadsValue->maxVal < concValue->maxVal) {
            concValue->maxVal = threadsValue->maxVal;
        }
    }
}

void checaIguadade(arrayValue *seqValue, arrayValue *concValue){
    if(seqValue->maxVal == concValue->maxVal && seqValue->minVal == concValue->minVal){
        printf("OK\n");
    }else{
        printf("ERRO-igualdade\n");
    }
}

//fluxo princial
int main(int argc, char *argv[]){
    arrayValue seqValue, concValue;
    double ini, fim;
    pthread_t *tid;
    if(argc < 3){
        fprintf(stderr, "Digite: %s <dimensao do vetor> <numero de threads>\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    nthreads = atoi(argv[2]);

    array = (float*) malloc(sizeof(float)*N);
    if(array == NULL){
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }

    for(long int i=0;i<N;i++)
        array[i] = 1000 * (float)rand() / (float)RAND_MAX;

    concValue.maxVal = seqValue.maxVal = array[0];
    concValue.minVal = seqValue.minVal = array[0];
    GET_TIME(ini);
    tarefaSequencial(&array, &seqValue, N);
    GET_TIME(fim);
    printf("Tempo sequencial: %lf\n", fim-ini);
    GET_TIME(ini);
    iniThreads(&tid, &concValue, nthreads);
    GET_TIME(fim);
    printf("Tempo concorrente: %lf\n", fim-ini);

    checaIgualdade(&seqValue, &concValue);

    free(array);
    free(tid);
    return 0;
}