#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include "tabla_paginacion.cpp"

using namespace std;

/**
 * @brief Enumeración para los algoritmos de reemplazo de páginas.
 */
enum ReplacementAlgorithm {
    FIFO,   // Algoritmo First-In-First-Out
    LRU,    // Algoritmo Least Recently Used
    CLOCK,  // Algoritmo del reloj
    OPTIMAL // Algoritmo óptimo
};

/**
 * @brief Función para parsear el argumento del algoritmo.
 * @param algorithm Nombre del algoritmo en formato string.
 * @return ReplacementAlgorithm El algoritmo de reemplazo correspondiente.
 * @throws invalid_argument Si el algoritmo no es soportado.
 */
ReplacementAlgorithm parseAlgorithm(const string &algorithm) {
    if (algorithm == "FIFO") return ReplacementAlgorithm::FIFO;
    if (algorithm == "LRU") return ReplacementAlgorithm::LRU;
    if (algorithm == "CLOCK") return ReplacementAlgorithm::CLOCK;
    if (algorithm == "OPTIMAL") return ReplacementAlgorithm::OPTIMAL;
    throw invalid_argument("Algoritmo no soportado");
}

/**
 * @brief Función para leer referencias desde el archivo.
 * 
 * @param filename Nombre del archivo que contiene las referencias.
 * @return vector<int> Vector con las referencias de páginas.
 * @throws runtime_error Si no se puede abrir el archivo.
 */
vector<int> readReferences(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("No se pudo abrir el archivo de referencias");
    }
    vector<int> references;
    int ref;
    while (file >> ref) {
        references.push_back(ref);
    }
    file.close();
    return references;
}

/**
 * @brief Función principal del programa.
 * 
 * Esta función parsea los argumentos de la línea de comandos, lee las referencias de páginas
 * desde un archivo y simula el comportamiento de los algoritmos de reemplazo de páginas.
 * 
 * @param argc Número de argumentos.
 * @param argv Array de argumentos.
 * @return int Código de retorno del programa.
 */
int main(int argc, char *argv[]) {
    int frames = 0;
    string algorithm;
    string referenceFile;

    // Parseo de argumentos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
            frames = stoi(argv[++i]);
        } else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) {
            algorithm = argv[++i];
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            referenceFile = argv[++i];
        }
    }

    if (frames <= 0 || algorithm.empty() || referenceFile.empty()) { // Verificar argumentos
        cerr << "Uso: ./mvirtual -m <marcos> -a <algoritmo> -f <archivo>" << endl;
        return 1;
    }

    cout << "Marcos: " << frames << ", Algoritmo: " << algorithm << ", Archivo: " << referenceFile << endl; // Mostrar argumentos (Solo para depuración)

    try { // Iniciar simulación
        ReplacementAlgorithm algo = parseAlgorithm(algorithm);
        vector<int> references = readReferences(referenceFile);

        // Crear la tabla de páginas con el número de marcos especificado
        PageTable pageTable(frames);


        // Insertar las referencias en la tabla de páginas (Simulación para pruebas)
        for (size_t i = 0; i < references.size(); i++) {
            int pageNumber = references[i];
            if (pageTable.getFrame(pageNumber) == -1) {
                cout << "Insertando página " << pageNumber << endl;
                pageTable.insertPage(pageNumber, i % frames);
            }
        }

        pageTable.displayTable();  // Mostrar la tabla de páginas

    } catch (const exception &e) { // Capturar excepciones
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}