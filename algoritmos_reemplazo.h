#ifndef ALGORITMOS_REEMPLAZO_H
#define ALGORITMOS_REEMPLAZO_H

#include "tabla_paginacion.h"
#include <unordered_map>
#include <queue>
#include <list>
#include <vector>

int insertarFIFO(std::vector<int>* referencias, int numMarcos, PageTable* tabla_ptr);
int insertarLRU(std::vector<int>* referencias, int numMarcos, PageTable* tabla_ptr);
int insertarReloj(std::vector<int>* referencias, int numMarcos, PageTable* tabla_ptr);
//int insertarOptimo(std::vector<int>* referencias, int numMarcos, PageTable* tabla_ptr);

#endif