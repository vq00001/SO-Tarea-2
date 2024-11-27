# Sistemas-Operativos-T2

Este proyecto consiste en la implementacion de un programa en c/c++ que cumpla los siguientes requisitos:

1. Crear un simulador que maneje una cola circular de tamaño dinamico compartida entre múltiples hebras. Múltiples hebras productoras pueden agregar items a la cola y múltiples hebras consumidoras
pueden extraer elementos de la cola.
2. Crear un simulador de memoria virtual con paginación y algoritmos de reemplazo de páginas.


## Compilación

1. g++ monitor.cpp ColaCircular.cpp -o monitor.exe
2. g++ simulador_paginacion.cpp algoritmos_reemplazo.cpp tabla_paginacion.cpp -o simulador_paginacion.exe



## Parámetros de entrada

Estas dos funciones estan en dos archivos respectivamente. Los parametros de entrada para cada uno de ellos es:

#### Programa 1 ###
Ejemplo de ejecución: $./simulapc -p 10 -c 5 -s 50 -t 1

-p N: N el número de productores, 
-c M: M el número de consumidores, 
-s W: W el tamaño inicial de la cola, 
-t T: T es cuanto tiempo en segundos tienen que esperar los consumidores despues de que los productores terminen.


#### Programa 2
Ejemplo de ejecución: ./mvirtual -m 3 -a FIFO -f referencias.txt. 
Donde referencias.txt contiene “0 1 3 4 1 2 5 1 2 3 4”


