#ifndef ALGORITMOS_REEMPLAZO_H
#define ALGORITMOS_REEMPLAZO_H

#include <unordered_map>
#include <queue>
#include <list>
#include <vector>

// struct EntradaPagina {
//     int numeroMarco;
//     bool valido;
//     int bitUso; // Para el algoritmo del reloj
// };

// class PageTable {
// private:
//     std::unordered_map<int, EntradaPagina> tabla;
//     std::queue<int> colaFIFO;
//     std::list<int> listaLRU;
//     std::vector<int> marcosReloj;
//     std::vector<int> bitsUso;
//     int punteroReloj = 0;
//     int numMarcos;
//     int fallosPagina = 0;

// public:
//     PageTable(int marcos);
//     int obtenerFallosPagina() const;

//     void insertarLRU(int numeroPagina);
//     void insertarReloj(int numeroPagina);
//     void insertarOptimo(int numeroPagina, std::vector<int>& referencias, int indiceActual);
// };

int insertarFIFO(void* referencias_ptr, int numMarcos, void* tabla_ptr);

#endif