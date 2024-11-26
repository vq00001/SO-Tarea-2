#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
//#include <unistd.h>

class colaCircular {
	std::ofstream registro;
	std::string archivo = "registros.txt";
private:
    int elementos;
	int large;
	int final;
	int frente;
	int* cola;
public:

	colaCircular(int largo){
		this->elementos = 0;
		this->large = largo;
		this->final = 0;
		this->frente = 0;
		this->cola = (int*)calloc(large,sizeof(int));
		std::stringstream aux ;
		aux << "\n\n===================Cargando cola numero:"<<this <<"=======================\n\n";
		std::string inicio = aux.str();
		this->registrar(inicio);
	}
    void printCola(){
		for (int i = 0; i < this->large; ++i){
			printf("%d ",this->cola[i]);
		}
		printf("\n");
	}

	void registrar(std::string content){
		registro.open(archivo,std::ios::out | std::ios::app);
		if (!registro){
			printf("Error al registrar...");
			return;
		}
		registro << content << std::endl;
		registro.close();
	}
	void registrarCola(){
		registro.open(archivo,std::ios::out | std::ios::app);
		if (!registro){
			printf("Error al registrar...");
			return;
		}
		int aux= this->final ;
		if (this->final==0) aux= this->large;
			
		if (aux > this->frente){
			for (int i = 0; i < this->large; ++i){
				if (i >= this->frente && i < aux){
					registro << this->cola[i] << " ";
				}
				else{
					registro << "0 ";
				}
			}
			registro << "\n";
		}
		else{
			for (int i = 0; i < this->large; ++i){
				if (i < aux || i >= this->frente){
					registro << this->cola[i] << " ";
				}
				else{
					registro << "0 ";
				}
			}
			registro << "\n";
		}
		for (int i = 0; i < this->large; ++i){
			if(i == this->frente && i == this->final){
				registro << "B ";
			}
			else if(i == this->frente){
				registro << "I ";
			}
			else if(i== this->final){
				registro << "F " ;
			}
			else{
				registro << "_ ";
			}
		}
		registro << "\n" ;
		printf("\n");

		registro.close();
	}

	void reducir(){
		if (this->final >= this->frente ){
			memmove(this->cola,this->cola+ this->frente,(this->final-this->frente)*sizeof(int));
			this->frente = 0;
			this->final = this->elementos;
			printCola();
			int* ptr =  (int*)realloc(this->cola ,(this->large/2)*sizeof(int));
			printf("%d\n",*(this->cola));
			if(ptr != NULL){
				printf("%d\n",*(ptr));
				this->cola = ptr;
				this->large = this->large / 2;
				std::stringstream aux ;
				aux << "La cola se ha reducido con exito y ahora su tamaño es "<<this->large <<"\n";
				std::string mensaje = aux.str();
				registrar(mensaje);
			}
			else{
				printf("algo ocurrio...\n");
			}
		}
		else{
			printf("mueve otra forma\n" );
			int buffer[this->final];
			for (int i = 0; i < this->final ; ++i)
			{
				buffer[i] = this->cola[i];
			}
			int aux = this->large - this->frente;
			memmove(this->cola,this->cola+ this->frente,aux*sizeof(int));
			memmove(this->cola+aux,buffer,(this->final)*sizeof(int));
			this->frente = 0;
			this->final = aux + this->final;
			printCola();
			int* ptr = (int*)realloc(this->cola ,(this->large/2)*sizeof(int));
			if(ptr != NULL){
				printf("%d\n",*(ptr));
				this->cola = ptr;
				this->large = this->large / 2;
				std::stringstream aux ;
				aux << "La cola se ha reducido con exito y ahora su tamaño es "<<this->large <<"\n";
				std::string mensaje = aux.str();
				registrar(mensaje);
			}
			else{
				printf("algo ocurrio...\n");
			}
		}
	}

	void expandir(){
		if (this->final > this->frente ){
			int* ptr = (int*)realloc(this->cola ,(this->large*2)*sizeof(int));
			printf("%d\n",*(this->cola));
			if(ptr != NULL){
				printf("%d\n",*(ptr));
				this->cola = ptr;
				this->large = this->large * 2;

				std::stringstream aux ;
				aux << "La cola se ha duplicado con exito y ahora su tamaño es "<<this->large <<"\n";
				std::string mensaje = aux.str();
				registrar(mensaje);
			}
			else{
				printf("algo ocurrio...\n");
			}
		}
		else{
			printf("mueve otra forma\n" );
			int aux = this->large - this->frente;
			int buffer[aux];
			for (int i = this->frente; i < this->large ; ++i){
				buffer[i-this->frente] = this->cola[i];
			}
			int* ptr = (int*)realloc(this->cola ,(this->large*2)*sizeof(int));
			if(ptr != NULL){
				printCola();
				this->cola = ptr;
				this->large = this->large * 2;
				this->frente = this->large - aux;
				memmove(this->cola+ this->frente,buffer,aux*sizeof(int));
				printCola();
				printf("buffer:\n");
				for (int i = 0; i < aux; ++i)
				{
					printf("%d\n", buffer[i]);
				}
				printf("\nfin.. aux: %d\n", aux);
				std::stringstream aux ;
				aux << "La cola se ha reducido con exito y ahora su tamaño es "<<this->large <<"\n";
				std::string mensaje = aux.str();
				registrar(mensaje);
			}
			else{
				printf("algo ocurrio...\n");
			}
		}
	}

	void addToCola(int n){
		registrar("----------------------\n");
		if(this->elementos >= this->large ){
			if (this->final == 0) this->final = this->large;
			expandir();
		}
		else if (this->elementos == this->large/4){}	

		this->cola[this->final] = n;
		this->final= (this->final + 1)%this->large;
		this->elementos= this->elementos+1;
		std::stringstream aux ;
		aux << "Se añadio: " << n << "\n";
		std::string mensaje = aux.str();
		registrar(mensaje);
		registrarCola();
		registrar("----------------------\n");
		
	}

	int quitarElemento(){
		registrar("-----------------------\n");
		printf("elementos : %d\n",this->elementos);
		if(this->elementos == 0){
			printf("entre aca\n");
			return -1 ;
		}
		int n = this->cola[this->frente];
		this->cola[this->frente] = -1;
		this->elementos= this->elementos-1;
		this->frente = (this->frente + 1) %this->large;
		if(this->elementos == this->large/4 && this->large > 1){
			reducir();
		}
		std::stringstream aux ;
		aux << "Se elimino: " << n << "\n";
		std::string mensaje = aux.str();
		registrar(mensaje);
		registrarCola() ;
		registrar("-----------------------\n");
		return n;
	}
	int* getCola(){
		return this->cola;
	}
};

int main(){
	int n, aux;
	printf("Ingresar tamaño de cola: ");
	scanf("%d",&n);
	colaCircular cola(n);
	while (n != 0){
		printf("Ingresar 1 para añair a la cola. 2 para sacar elemento. 0 para salir:\n" );
		scanf("%d",&n);
		printf("wtf\n");
		if(n == 1){
			printf("Numero a poner en la cola: ");
			scanf("%d",&aux);
			cola.addToCola(aux);
		}
		else if(n==2){
			printf("help\n");
			printf("El elemento elimindao es: %d\n",cola.quitarElemento());
		}
		else{
			free(cola.getCola());
			return 0;
		}
		printf("Cola: ");
		cola.printCola();
		printf("\n");
	}
}