#include "algoritmos_reemplazo.h"
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
vector<int> cargarReferencias(const string &filename) {
   
    ifstream archivo(filename);
    
    if (!archivo.is_open()) {
        throw runtime_error("No se pudo abrir el archivo de referencias");
        exit(EXIT_FAILURE);
    }
    // int pagina;
    // while (archivo >> pagina) {
    //     referencias.push_back(pagina);
    // }
    // return referencias;

    vector<int> referencias;
    int ref;
    while (archivo >> ref) {
        referencias.push_back(ref);
    }
    archivo.close();
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

    PageTable tablaPaginacion(marcos);  // Crear tabla de páginas

    cout << "Marcos: " << marcos << ", Algoritmo: " << algoritmo << ", Archivo: " << archivoReferencias << endl << endl; // Mostrar argumentos 

    try { // Iniciar simulación
        string algoritmoReemplazo = algoritmo;
        vector<int> referencias = cargarReferencias(archivoReferencias);
        int fallosPagina = 0;

        // Crear la tabla de páginas con el número de marcos especificado
        PageTable pageTable(marcos);

        // Insertar las referencias en la tabla de páginas según el algoritmo
        if (algoritmoReemplazo == "FIFO") {
            fallosPagina = insertarFIFO(&referencias, marcos, &pageTable);
        } else if (algoritmoReemplazo == "LRU") {
            fallosPagina = insertarLRU(&referencias, marcos, &pageTable);
        } else if (algoritmoReemplazo == "CLOCK") {
            fallosPagina = insertarReloj(&referencias, marcos, &pageTable);
        } else if (algoritmoReemplazo == "OPTIMAL") {
            fallosPagina = insertarOptimo(&referencias, marcos, &pageTable);
        } else {
            cerr << "Algoritmo de reemplazo no válido.\n";
            return 1;
        }

        pageTable.displayTable();
        cout << "\nFallos de página: " << fallosPagina << endl;

    } catch (const exception &e) { // Capturar excepciones
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}