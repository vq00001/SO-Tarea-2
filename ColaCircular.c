#include <stdio.h>
#include <stdlib.h>

typedef struct  colaCircular{
	int elementos
	int large
	int final
	int frente
	int* cola
} colaCircular;

colaCircular colaCircular(int large){
	colaCircular cola
	cola.elementos = 0
	cola.large = large
	cola.final = 0
	cola.frente = 0
	cola.cola = (int*)malloc(large*sizeof(int))
}

void addToCola(colaCircular cola,int n){
	if(cola.elementos >= cola.large ){
		return
	}
	cola.cola[cola.final] = n
	cola.final= (cola.final + 1)%large
	cola.elementos++
	if (cola.elementos==cola.large){
		
	}
	else if (cola.elementos == cola.large/4){

	}
}

int quitarElemento(colaCircular cola){
	if(elementos == 0){
		return NULL 
	}
	int n = cola.cola[cola.frente]
	cola.elementos--
	cola.frente = (cola.frente + 1) %large
	if(cola.elementos == cola.large/4){}
}