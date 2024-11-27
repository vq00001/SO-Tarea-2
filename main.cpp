#include "tabla_paginacion.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

/**
 * @brief Carga referencias desde un archivo.
 *
 * @param filename Archivo de entrada.
 * @return Vector de referencias de páginas.
 */
vector<int> cargarReferencias(const string& filename) {
    vector<int> referencias;
    ifstream archivo(filename);
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo de referencias.\n";
        exit(EXIT_FAILURE);
    }
    int pagina;
    while (archivo >> pagina) {
        referencias.push_back(pagina);
    }
    return referencias;
}

int main(int argc, char* argv[]) {
    int marcos = 0;
    string algoritmo, archivoReferencias;


    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
            marcos = stoi(argv[++i]);
        } else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) {
            algoritmo = argv[++i];
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            archivoReferencias = argv[++i];
        }
    }


    if (marcos <= 0 || algoritmo.empty() || archivoReferencias.empty()) {
        cerr << "Uso: ./mvirtual -m <marcos> -a <algoritmo> -f <archivo>\n";
        return 1;
    }

    vector<int> referencias = cargarReferencias(archivoReferencias);
    PageTable tablaPaginacion(marcos);


    if (algoritmo == "FIFO") {
        for (int ref : referencias) {
            tablaPaginacion.insertarFIFO(ref);
        }
    } else if (algoritmo == "LRU") {
        for (int ref : referencias) {
            tablaPaginacion.insertarLRU(ref);
        }
    } else if (algoritmo == "CLOCK") {
        for (int ref : referencias) {
            tablaPaginacion.insertarReloj(ref);
        }
    } else if (algoritmo == "OPTIMAL") {
        for (size_t i = 0; i < referencias.size(); ++i) {
            tablaPaginacion.insertarOptimo(referencias[i], referencias, i);
        }
    } else {
        cerr << "Error: Algoritmo no reconocido.\n";
        return 1;
    }

    cout << "Fallos de página: " << tablaPaginacion.obtenerFallosPagina() << endl;
    return 0;
}