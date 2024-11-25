#include "tabla_paginacion.h"
#include <iostream>
#include <algorithm>

PageTable::PageTable(int marcos) : numMarcos(marcos), bitsUso(marcos, 0), marcosReloj(marcos, -1) {}

int PageTable::obtenerFallosPagina() const {
    return fallosPagina;
}

void PageTable::insertarFIFO(int numeroPagina) {
    if (tabla.size() < numMarcos) {
        tabla[numeroPagina] = {colaFIFO.size(), true};
        colaFIFO.push(numeroPagina);
        fallosPagina++;
    } else {
        int reemplazar = colaFIFO.front();
        colaFIFO.pop();
        colaFIFO.push(numeroPagina);
        tabla.erase(reemplazar);
        tabla[numeroPagina] = {colaFIFO.size() - 1, true};
        fallosPagina++;
    }
}

void PageTable::insertarLRU(int numeroPagina) {
    auto it = tabla.find(numeroPagina);
    if (it != tabla.end()) {
        listaLRU.remove(numeroPagina);
        listaLRU.push_back(numeroPagina);
    } else {
        if (tabla.size() >= numMarcos) {
            int reemplazar = listaLRU.front();
            listaLRU.pop_front();
            tabla.erase(reemplazar);
        }
        tabla[numeroPagina] = {static_cast<int>(tabla.size()), true};
        listaLRU.push_back(numeroPagina);
        fallosPagina++;
    }
}

void PageTable::insertarReloj(int numeroPagina) {
    auto it = std::find(marcosReloj.begin(), marcosReloj.end(), numeroPagina);
    if (it != marcosReloj.end()) {
        int indice = std::distance(marcosReloj.begin(), it);
        bitsUso[indice] = 1;
    } else {
        while (bitsUso[punteroReloj] == 1) {
            bitsUso[punteroReloj] = 0;
            punteroReloj = (punteroReloj + 1) % numMarcos;
        }
        tabla.erase(marcosReloj[punteroReloj]);
        marcosReloj[punteroReloj] = numeroPagina;
        tabla[numeroPagina] = {punteroReloj, true};
        bitsUso[punteroReloj] = 1;
        punteroReloj = (punteroReloj + 1) % numMarcos;
        fallosPagina++;
    }
}

void PageTable::insertarOptimo(int numeroPagina, std::vector<int>& referencias, int indiceActual) {
    if (tabla.find(numeroPagina) != tabla.end()) return;

    if (tabla.size() < numMarcos) {
        tabla[numeroPagina] = {static_cast<int>(tabla.size()), true};
    } else {
        int indiceMasLejano = -1, reemplazar = -1;
        for (const auto& par : tabla) {
            int indiceFuturo = std::find(referencias.begin() + indiceActual, referencias.end(), par.first) - referencias.begin();
            if (indiceFuturo > indiceMasLejano || indiceFuturo == static_cast<int>(referencias.size())) {
                indiceMasLejano = indiceFuturo;
                reemplazar = par.first;
            }
        }
        tabla.erase(reemplazar);
        tabla[numeroPagina] = {tabla[reemplazar].numeroMarco, true};
    }
    fallosPagina++;
}