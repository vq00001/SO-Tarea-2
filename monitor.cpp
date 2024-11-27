#include <bits/stdc++.h>
#include <sys/time.h>
#include "ColaCircular.h"
using namespace std;

#define MAX_QUEUE_SIZE 5  // Tamaño máximo de la cola

// Estructura del monitor (la cola compartida)

// Inicializar el monitor

// Mostrar el contenido de la cola (solo para depuración)


class Monitor{

    private:
        ColaCircular cola;
        pthread_mutex_t mutex;        // Mutex para protección de la cola
        pthread_cond_t not_empty;     // Variable de condición para la cola no vacía
        pthread_cond_t not_full;      // Variable de condición para la cola no llena
       
        int tiempo_espera;

    public:

        Monitor(int sec_espera, int largo_cola){
            pthread_mutex_init(&mutex, NULL);
            pthread_cond_init(&not_empty, NULL);
            pthread_cond_init(&not_full, NULL);

            tiempo_espera = sec_espera;
            cola.init(largo_cola);
            
            // spinlock.init(&cola, sec_espera);
        }

        // void printQueue() {
        //     printf("Cola: ");
        //     for (int i = 0; i < cola.getElementos(); i++) {
        //         printf("%d ", cola.buffer[(cola.front + i) % MAX_QUEUE_SIZE]);
        //     }
        //     printf("\n");
        // }

        
        void insert(int value){
            
            pthread_mutex_lock(&mutex);

            // Esperar si la cola está llena
            // while (cola.count == MAX_QUEUE_SIZE) {
            //     pthread_cond_wait(&not_full, &mutex);
            // }

            // Insertar el valor
            printf("Productor: insertando %d\n", value);


            //
            // llamar a funcion de cola circular
            cola.addToCola(value);
            // llamar func
            //

            cola.printCola();

            // Señalar que la cola no está vacía
            pthread_cond_signal(&not_empty);
            pthread_mutex_unlock(&mutex);        
        }
        

        int extract(){

            pthread_mutex_lock(&mutex);
            
            // inicializar variables de tiempo
            auto start = chrono::high_resolution_clock::now();
            auto end = start + std::chrono::seconds(tiempo_espera);  // Tiempo futuro en el que se espera
            
            timespec timeout;
            timeout.tv_sec = std::chrono::duration_cast<std::chrono::seconds>(end.time_since_epoch()).count();
            timeout.tv_nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(end.time_since_epoch()).count() % 1000000000;


            // Esperar si la cola está vacía
            while (cola.getElementos() == 0) {
                int rc = pthread_cond_timedwait(&not_empty, &mutex, &timeout);

                // Si rc es 0, significa que el hilo fue despertado por la condición
                // Si rc es ETIMEDOUT, significa que el hilo terminó por el tiempo de espera
                if (rc == ETIMEDOUT) {
                    printf("Consumidor: tiempo de espera agotado, terminando\n");
                    pthread_mutex_unlock(&mutex);
                    this_thread::yield();
                    return -1; // O cualquier valor que indique que el consumidor terminó sin éxito
                }
            }
    
            
            // Extraer el valor
            int value = cola.quitarElemento();

            if (value != -1) {
                printf("Consumidor: extrayendo %d\n", value);
                cola.printCola();
            }

            // Señalar que la cola no está llena
            // pthread_cond_signal(&not_full);
            pthread_mutex_unlock(&mutex);
            
            return value;
        }

        void liberar_memoria_cola(){
            free(cola.getCola());
        }
} ;



// Hilo productor
void* producer(void* arg){

    Monitor *q = (Monitor*)arg;
    usleep(50000);    
    for(int i = 0; i < 3; i++)
        q->insert(i);

    return NULL;
}

// Hilo consumidor
void* consumer(void* arg) {
    Monitor *q = (Monitor*)arg;            

    // Extraer el valor
    for(int i = 0; i < 3; i++){
        int val = q->extract();
        if(val == -1) {
            break;
        }
    }

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
    for(int i = 1; i < argc - 1; i += 2) {

        if(strcmp(argv[i], "-p") == 0) {cant_productores = atoi(argv[i+1]);}
        else if(strcmp(argv[i], "-c") == 0) {cant_consumidores = atoi(argv[i+1]);}
        else if(strcmp(argv[i], "-s") == 0) {cola_size = atoi(argv[i+1]);}
        else if(strcmp(argv[i], "-t") == 0) {tiempo_espera = atoi(argv[i+1]);       }
        else { printf("%s es un parametro invalido\n", argv[i]); exit(1);} 
        
    }
    
    
    struct timeval tval_inicio, tval_fin, tval_final;
    gettimeofday(&tval_inicio, NULL);

    // inicializar variables
    Monitor queue(tiempo_espera, cola_size);
    
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


    // esperar que todos los hilos terminen
    for(int i = 0; i < cant_productores; i++){
        pthread_join(productores[i], NULL);
    }
   
    for(int i = 0; i < cant_consumidores; i++){
        pthread_join(consumidores[i], NULL);
    }

   
    free(productores);
    free(consumidores);
    queue.liberar_memoria_cola();
  
    gettimeofday(&tval_fin, NULL);
    timersub(&tval_fin, &tval_inicio, &tval_final);
    
    printf("Tiempo total : %ld.%06ld\n", (long int)tval_final.tv_sec, (long int)tval_final.tv_usec);

}