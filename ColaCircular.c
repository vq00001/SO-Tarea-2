#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _ ->

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
	printf("\n");
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
void reducirCola(colaCircular* cola){
	if (cola->final >= cola->frente ){
		memmove(cola->cola,cola->cola+ cola->frente,(cola->final-cola->frente)*sizeof(int));
		cola->frente = 0;
		cola->final = cola->elementos;
		printCola(*cola);
		int* ptr = realloc(cola->cola ,(cola->large/2)*sizeof(int));
		printf("%d\n",*(cola->cola));
		if(ptr != NULL){
			printf("%d\n",*(ptr));
			cola->cola = ptr;
			cola->large = cola->large / 2;

			printf("La cola se ha reducido con exito y ahora su tamaño es %d \n",cola->large);
		}
		else{
			printf("algo ocurrio...\n");
		}
	}
	else{
		printf("mueve otra forma\n" );
		int buffer[cola->final];
		for (int i = 0; i < cola->final ; ++i)
		{
			buffer[i] = cola->cola[i];
		}
		int aux = cola->large - cola->frente;
		memmove(cola->cola,cola->cola+ cola->frente,aux*sizeof(int));
		memmove(cola->cola+aux,buffer,(cola->final)*sizeof(int));
		cola->frente = 0;
		cola->final = aux + cola->final;
		printCola(*cola);
		int* ptr = realloc(cola->cola ,(cola->large/2)*sizeof(int));
		if(ptr != NULL){
			printf("%d\n",*(ptr));
			cola->cola = ptr;
			cola->large = cola->large / 2;

			printf("La cola se ha reducido con exito y ahora su tamaño es %d \n",cola->large);
		}
		else{
			printf("algo ocurrio...\n");
		}
	}
}

void expandirCola(colaCircular* cola){
	if (cola->final > cola->frente ){
		int* ptr = realloc(cola->cola ,(cola->large*2)*sizeof(int));
		printf("%d\n",*(cola->cola));
		if(ptr != NULL){
			printf("%d\n",*(ptr));
			cola->cola = ptr;
			cola->large = cola->large * 2;

			printf("La cola se ha duplicado con exito y ahora su tamaño es %d \n",cola->large);
		}
		else{
			printf("algo ocurrio...\n");
		}
	}
	else{
		printf("mueve otra forma\n" );
		int aux = cola->large - cola->frente;
		int buffer[aux];
		for (int i = cola->frente; i < cola->large ; ++i)
		{
			buffer[i-cola->frente] = cola->cola[i];
		}
		/*memmove(cola->cola,cola->cola+ a,aux*sizeof(int));
		memmove(cola->cola+aux,buffer,(cola->final)*sizeof(int));
		cola->frente = 0;
		cola->final = aux + cola->final;
		printCola(*cola);*/
		int* ptr = realloc(cola->cola ,(cola->large*2)*sizeof(int));
		if(ptr != NULL){
			printCola(*cola);
			cola->cola = ptr;
			cola->large = cola->large * 2;
			cola->frente = cola->large - aux;
			memmove(cola->cola+ cola->frente,buffer,aux*sizeof(int));
			printCola(*cola);
			printf("buffer:\n");
			for (int i = 0; i < aux; ++i)
			{
				printf("%d\n", buffer[i]);
			}
			printf("\nfin.. aux: %d\n", aux);
			printf("La cola se ha duplicado con exito y ahora su tamaño es %d \n",cola->large);
		}
		else{
			printf("algo ocurrio...\n");
		}
	}
}

void addToCola(colaCircular* cola,int n){
	if(cola->elementos >= cola->large ){
		if (cola->final == 0) cola->final = cola->large;
		expandirCola(cola);
		/*int* ptr = realloc(cola->cola,cola->large*2*sizeof(int));
		if (ptr != NULL) {
			cola->cola = ptr;
			cola->final= cola->large;
			cola->large = cola->large*2;
			printf("La cola se ha duplicado con exito y ahora su tamaño es %d \n",cola->large);
		}*/
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
		reducirCola(cola);
		/*int* ptr = realloc(cola->cola,sizeof(int)*cola->large/2);
		if (ptr != NULL){
			cola->cola = ptr;
			cola->large = cola->large/2;
			printf("%d\n",*(cola->cola));
			printf("%d\n",*(cola->cola+3));
			printf("La cola se ha reducido con exito y ahora su tamaño es %d \n",cola->large);*/
			//reducirCola(cola);
		//} 
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
			free(cola.cola);
			return 0;
		}
		printf("Cola: ");
		printCola(cola);
		printf("\n");
	}
}