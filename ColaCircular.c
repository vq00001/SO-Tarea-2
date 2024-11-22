#include <stdio.h>
#include <stdlib.h>

typedef struct  colaCircular{
	int elementos;
	int large;
	int final;
	int frente;
	int* cola;
} colaCircular;

void printCola(colaCircular cola){

	for (int i = 0; i < cola.large; ++i)
	{
		printf("%d ",cola.cola[i]);
	}
}

colaCircular crearCola(int large){
	colaCircular cola;
	cola.elementos = 0;
	cola.large = large;
	cola.final = 0;
	cola.frente = 0;
	cola.cola = (int*)calloc(large,sizeof(int));
	return cola;
}

void addToCola(colaCircular* cola,int n){
	if(cola->elementos >= cola->large ){
		int* ptr = realloc(cola->cola,sizeof(cola->large*2));
		if (ptr != NULL) {
			cola->cola = ptr;
			cola->final= cola->large;
			cola->large = cola->large*2;
			printf("La cola se ha duplicado con exito y ahora su tamaño es %d \n",cola->large);
		}
	}
	else if (cola->elementos == cola->large/4){}	

	cola->cola[cola->final] = n;
	cola->final= (cola->final + 1)%cola->large;
	cola->elementos= cola->elementos+1;
	
}

int quitarElemento(colaCircular* cola){
	printf("elementos : %d\n",cola->elementos);
	if(cola->elementos == 0){
		printf("entre aca\n");
		return -1 ;
	}
	int n = cola->cola[cola->frente];
	cola->cola[cola->frente] = -1;
	cola->elementos= cola->elementos-1;
	cola->frente = (cola->frente + 1) %cola->large;
	if(cola->elementos == cola->large/4 && cola->large > 1){
		int* ptr = realloc(cola->cola,sizeof(cola->large/2));
		if (ptr != NULL){
			cola->cola = ptr;
			cola->large = cola->large/2;
			printf("La cola se ha reducido con exito y ahora su tamaño es %d \n",cola->large);
		} 
	}
	return n;
}

int main(){
	int n, aux;
	printf("Ingresar tamaño de cola: ");
	scanf("%d",&n);
	colaCircular cola = crearCola(n);
	while (n != 0){
		printf("Ingresar 1 para añair a la cola. 2 para sacar elemento. 0 para salir:\n" );
		scanf("%d",&n);
		printf("wtf\n");
		if(n == 1){
			printf("Numero a poner en la cola: ");
			scanf("%d",&aux);
			addToCola(&cola,aux);
		}
		else if(n==2){
			printf("help\n");
			printf("El elemento elimindao es: %d\n",quitarElemento(&cola));
		}
		else{
			return 0;
		}
		printf("Cola: ");
		printCola(cola);
		printf("\n");
	}
}