#ifndef COLA_CIRCULAR_H
#define COLA_CIRCULAR_H

#include <fstream>
#include <string>
#include <sstream>

class ColaCircular {
private:
    int elementos;        // Número de elementos en la cola
    int large;            // Capacidad de la cola
    int final;            // Índice del último elemento
    int frente;           // Índice del primer elemento
    int* cola;            // Puntero a la cola (array dinámico)
    std::ofstream registro; // Archivo para registrar cambios
    std::string archivo = "registros.txt"; // Nombre del archivo de registro

public:
    // Constructor de la clase
    ColaCircular();

    // Imprime la cola en la consola
    void printCola();

    // Inicializa la cola con un tamaño dado
    void init(int largo);

    // Registra un mensaje en el archivo de registro
    void registrar(std::string content);

    // Registra el estado completo de la cola en el archivo de registro
    void registrarCola();

    // Reduce el tamaño de la cola si es necesario (cuando la cola está muy vacía)
    void reducir();

    // Duplica el tamaño de la cola si es necesario (cuando la cola está llena)
    void expandir();

    // Agrega un nuevo elemento a la cola
    void addToCola(int n);

    // Elimina un elemento de la cola (el primero en la fila)
    int quitarElemento();

    // Devuelve el puntero al array que representa la cola
    int* getCola();

    int getElementos();
};

#endif // COLA_CIRCULAR_H
