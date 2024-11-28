#include "algoritmos_reemplazo.h"
#include "tabla_paginacion.h"
#include <iostream>
#include <algorithm>

using namespace std;

int insertarFIFO(std::vector<int>* referencias, int numMarcos, PageTable* tabla_ptr) {
    vector<int> referenciasVec = *referencias;
    queue<int> colaFIFO;

    for (int numeroPagina : referenciasVec) {
        if (tabla_ptr->getFrame(numeroPagina) == -1) {
            if (static_cast<int>(colaFIFO.size()) < numMarcos) {
                colaFIFO.push(numeroPagina);
                tabla_ptr->insertPage(numeroPagina, colaFIFO.back());
            } else {
                int reemplazar = colaFIFO.front();
                colaFIFO.pop();
                colaFIFO.push(numeroPagina);
                tabla_ptr->replacePage(numeroPagina, reemplazar);
            }
        }
    }
    int fallosPagina = tabla_ptr->getFallosPagina();

    return fallosPagina;
}

int insertarLRU(std::vector<int>* referencias, int numMarcos, PageTable* tabla_ptr) {
    vector<int> referenciasVec = *referencias;
    list<int> listaLRU;

    for (int numeroPagina : referenciasVec) {
        if (tabla_ptr->getFrame(numeroPagina) != -1) {
            listaLRU.remove(numeroPagina);
            listaLRU.push_back(numeroPagina);
        } else {
            if (static_cast<int>(listaLRU.size()) >= numMarcos) {
                int reemplazar = listaLRU.front();
                listaLRU.pop_front();
                tabla_ptr->replacePage(numeroPagina, reemplazar);
            } else {
                tabla_ptr->insertPage(numeroPagina, static_cast<int>(listaLRU.size()));
            }
            listaLRU.push_back(numeroPagina);
        }
    }
    int fallosPagina = tabla_ptr->getFallosPagina();

    return fallosPagina;
}

int insertarReloj(std::vector<int>* referencias, int numMarcos, PageTable* tabla_ptr) {
    vector<int> referenciasVec = *referencias;
    vector<int> marcosReloj(numMarcos, -1);
    vector<int> bitsUso(numMarcos, 0);
    int punteroReloj = 0;

    for (int numeroPagina : referenciasVec) {
        auto it = find(marcosReloj.begin(), marcosReloj.end(), numeroPagina);
        if (it != marcosReloj.end()) {
            int indice = distance(marcosReloj.begin(), it);
            bitsUso[indice] = 1;
        } else {
            while (bitsUso[punteroReloj] == 1) {
                bitsUso[punteroReloj] = 0;
                punteroReloj = (punteroReloj + 1) % numMarcos;
            }
            if (marcosReloj[punteroReloj] != -1) {
                tabla_ptr->removePage(marcosReloj[punteroReloj]);
            }
            marcosReloj[punteroReloj] = numeroPagina;
            tabla_ptr->insertPage(numeroPagina, punteroReloj);
            bitsUso[punteroReloj] = 1;
            punteroReloj = (punteroReloj + 1) % numMarcos;
        }
    }
    int fallosPagina = tabla_ptr->getFallosPagina();

    return fallosPagina;
}

// void PageTable::insertarOptimo(int numeroPagina, std::vector<int>& referencias, int indiceActual) {
//     if (tabla.find(numeroPagina) != tabla.end()) return;

//     if (tabla.size() < numMarcos) {
//         tabla[numeroPagina] = {static_cast<int>(tabla.size()), true};
//     } else {
//         int indiceMasLejano = -1, reemplazar = -1;
//         for (const auto& par : tabla) {
//             int indiceFuturo = std::find(referencias.begin() + indiceActual, referencias.end(), par.first) - referencias.begin();
//             if (indiceFuturo > indiceMasLejano || indiceFuturo == static_cast<int>(referencias.size())) {
//                 indiceMasLejano = indiceFuturo;
//                 reemplazar = par.first;
//             }
//         }
//         tabla.erase(reemplazar);
//         tabla[numeroPagina] = {tabla[reemplazar].numeroMarco, true};
//     }
//     fallosPagina++;
// }