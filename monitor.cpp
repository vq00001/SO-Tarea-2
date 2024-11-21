#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <semaphore.h>
#include <iostream>
#include <atomic>
#include <chrono>
#include <bits/std_thread.h>
using namespace std;

#define MAX_QUEUE_SIZE 5  // Tamaño máximo de la cola

// Estructura del monitor (la cola compartida)

// Inicializar el monitor

// Mostrar el contenido de la cola (solo para depuración)

typedef struct {
    int buffer[MAX_QUEUE_SIZE]; // cola
    int front;  // Indice del primer elemento
    int rear;   // Indice del siguiente espacio vacío
    int count;  // Número de elementos en la cola
} Cola;


class Monitor{

    private:
        Cola cola;
        pthread_mutex_t mutex;        // Mutex para protección de la cola
        pthread_cond_t not_empty;     // Variable de condición para la cola no vacía
        pthread_cond_t not_full;      // Variable de condición para la cola no llena

    public:

        Monitor(){
            pthread_mutex_init(&mutex, NULL);
            pthread_cond_init(&not_empty, NULL);
            pthread_cond_init(&not_full, NULL);
        }

        void printQueue() {
            printf("Cola: ");
            for (int i = 0; i < cola.count; i++) {
                printf("%d ", cola.buffer[(cola.front + i) % MAX_QUEUE_SIZE]);
            }
            printf("\n");
        }

        
        void insert(int value){
            
            pthread_mutex_lock(&mutex);

            // Esperar si la cola está llena
            while (cola.count == MAX_QUEUE_SIZE) {
                pthread_cond_wait(&not_full, &mutex);
            }

            // Insertar el valor
            printf("Productor: insertando %d\n", value);

            cola.buffer[cola.rear] = value;
            cola.rear = (cola.rear + 1) % MAX_QUEUE_SIZE;
            cola.count++;
            
            printQueue();

            // Señalar que la cola no está vacía
            pthread_cond_signal(&not_empty);

            pthread_mutex_unlock(&mutex);
        
        }
        

        int extract(){
            pthread_mutex_lock(&mutex);

            // Esperar si la cola está vacía
            while (cola.count == 0) {
                pthread_cond_wait(&not_empty, &mutex);
            }

            // Extraer el valor
            
            int value = cola.buffer[cola.front];

            printf("Consumidor: extrayendo %d\n", value);
            cola.front = (cola.front + 1) % MAX_QUEUE_SIZE;
            cola.count--;

            printQueue();

            // Señalar que la cola no está llena
            pthread_cond_signal(&not_full);
            pthread_mutex_unlock(&mutex);

            return value;
        }
} ;



// Hilo productor
void* producer(void* arg){

    Monitor *q = (Monitor*)arg;
    q->insert(1);    
    return NULL;
}

// Hilo consumidor
void* consumer(void* arg) {
    Monitor *q = (Monitor*)arg;            

    // Extraer el valor
    int val = q->extract();

    usleep(150000);  // Simula el tiempo de consumo

    return NULL;
}

int main(int argc, char* argv[]) {
    
    int cant_productores = 5, cant_consumidores = 5, cola_size = 5, tiempo_espera = 2;

    if(argc % 2 == 0){
        printf("uso: %s -p <num productores> -c <num consumidores> -s <tamaño cola> -t <segundos de espera>\n",argv[0]);
        printf("default: -p 5 -c 5 -s 5 -t 2\n");
        exit(1);
    }
    
    // identificar parametros
    for(int i = 1; i < argc - 1; i++) {

        if(strcmp(argv[i], "-p") == 0) {cant_productores = atoi(argv[i+1]);}
        else if(strcmp(argv[i], "-c") == 0) {cant_consumidores = atoi(argv[i+1]);}
        else if(strcmp(argv[i], "-s") == 0) {cola_size = atoi(argv[i+1]);}
        else if(strcmp(argv[i], "-t") == 0) {tiempo_espera = atoi(argv[i+1]);       }
        else { printf("%s es un parametro invalido", argv[i]); exit(1);} 
        
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
    for (int j = 0; j < cant_productores; j++) {
       
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

    cout << "cant productores" << cant_productores << endl;

    for(int i = 0; i < cant_productores; i++){
        pthread_join(productores[i], NULL);
    // pthread_join(consumer_thread, NULL);
    }
   
    for(int i = 0; i < cant_consumidores; i++){
        pthread_join(consumidores[i], NULL);
    // pthread_join(consumer_thread, NULL);
    }

   
    free(productores);
    free(consumidores);
  
    gettimeofday(&tval_fin, NULL);
    timersub(&tval_fin, &tval_inicio, &tval_final);
    
    printf("Tiempo total : %ld.%06ld\n", (long int)tval_final.tv_sec, (long int)tval_final.tv_usec);

}