#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <semaphore.h>

#define MAX_QUEUE_SIZE 5  // Tamaño máximo de la cola

// Estructura del monitor (la cola compartida)

// Inicializar el monitor

// Mostrar el contenido de la cola (solo para depuración)

typedef struct {

} Monitor;


// Hilo productor
void* producer(void* arg) {
    Monitor *q = (Monitor*)arg;
    for (int i = 0; i < 10; i++) {

        usleep(100000);  // Simula el tiempo de producción
    }
    return NULL;
}

// Hilo consumidor
void* consumer(void* arg) {
    Monitor *q = (Monitor*)arg;
    for (int i = 0; i < 10; i++) {        
        usleep(150000);  // Simula el tiempo de consumo
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    
    if(argc % 2 == 0){
        printf("uso: %s -p <num productores> -c <num consumidores> -s <tamaño cola> -t <segundos de espera>\n",argv[0]);
        printf("default: -p 5 -c 5 -s 5 -t 2\n");
        exit(1);
    }
 
    int cant_productores = 5, cant_consumidores = 5, cola_size = 5, tiempo_espera = 2;
    
    // identificar parametros
    for(int i = 1; i < argc - 1; i++) {

        if(strcmp(argv[i], "-p")) cant_productores = atoi(argv[i+1]);
        else if(strcmp(argv[i], "-c") == 0) cant_consumidores = atoi(argv[i+1]);
        else if(strcmp(argv[i], "-s") == 0) cola_size = atoi(argv[i+1]);
        else if(strcmp(argv[i], "-t") == 0) tiempo_espera = atoi(argv[i+1]);       
        else printf("%s es un parametro invalido", argv[i]); // puede tener error
    }
    
    
    struct timeval tval_inicio, tval_fin, tval_final;
    gettimeofday(&tval_inicio, NULL);

    // inicializar variables
    Monitor queue;
    
    int rc;        

    pthread_t* productores;
    productores = (pthread_t*) malloc(cant_productores*sizeof(pthread_t));
    if(productores == NULL) {
        printf("error en malloc \n");
        exit(1);
    }
    pthread_t* consumidores;
    consumidores = (pthread_t*) malloc(cant_consumidores*sizeof(pthread_t));
    if(consumidores == NULL){
        printf("error en malloc \n");
        exit(1);
    }

 
    // crear productores
    for (int j=0; j < cant_productores; j++) {
       
        rc = pthread_create(&productores[j], NULL, producer, &queue);
        if (rc) {
            printf("pthread_create() error %d\n", rc);
            exit(-1);
        }
    }

    // crear consumidores
    for (int j = 0; j < cant_consumidores; j++) {

        rc = pthread_create(&consumidores[j], NULL, consumer, &queue);             
        if (rc) {
            printf("pthread_create() error %d\n", rc);
            exit(-1);
        }
    }


    for(int i = 0; i < cant_productores; i++){
        pthread_join(productores[i], NULL);
    // pthread_join(consumer_thread, NULL);
    }
   
    for(int i = 0; i < cant_consumidores; i++){
        pthread_join(consumidores[i], NULL);
    // pthread_join(consumer_thread, NULL);
    }

    free(consumidores);
    free(productores);
  
    gettimeofday(&tval_fin, NULL);
    timersub(&tval_fin, &tval_inicio, &tval_final);
    printf("Tiempo total : %ld.%06ld\n", (long int)tval_final.tv_sec, (long int)tval_final.tv_usec);


}