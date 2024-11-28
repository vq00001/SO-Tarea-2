#ifndef TABLA_PAGINACION_H
#define TABLA_PAGINACION_H

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <string>

using namespace std;

/**
 * @brief Representa una entrada en la tabla de paginación.
 *
 * Esta estructura contiene información sobre una entrada en la tabla de paginación,
 * incluyendo el número de marco físico asociado y un bit de validez.
 */
struct PageEntry {
    int frameNumber;  // Número de marco físico asociado
    bool valid;       // Bit de validez que indica si la entrada es válida (true) o no (false).
};

/**
 * @brief Clase que representa una tabla de páginas.
 *
 * Esta clase implementa una tabla de páginas para un sistema de paginación simple.
 * La tabla de páginas mapea números de página a números de marco físico y mantiene
 * un conjunto de marcos físicos ocupados.
 */
class PageTable {
public:
    /**
     * @brief Constructor de la clase PageTable.
     * @param frames Número de marcos físicos disponibles.
     */
    PageTable(int frames);

    /**
     * @brief Inserta una página en la tabla de páginas.
     * @param pageNumber Número de página.
     * @param frameNumber Número de marco físico.
     */
    void insertPage(int pageNumber, int frameNumber);

    /**
     * @brief Obtiene el número de marco físico asociado a un número de página.
     * @param pageNumber Número de página.
     * @return int Número de marco físico asociado, o -1 si la página no está en la tabla.
     */
    int getFrame(int pageNumber) const;

     /**
     * @brief Método para eliminar una página de la tabla
     * @param pageNumber: Número de página a eliminar
     */
    void removePage(int pageNumber);

     /**
     * @brief Método para invalidar una página
     * @param pageNumber: Número de página a invalidar
     */
    void invalidatePage(int pageNumber);

     /**
     * @brief Método para reemplazar una página por otra
     * @param pageNumber: Número de página por ingresar
     * @param pageToReplace: Número de página a reemplazar
     */
    void replacePage(int pageNumber, int pageToReplace);

    /**
     * @brief Muestra el contenido de la tabla de páginas.
     */
    void displayTable() const;

    /**
     * @brief Obtiene el número de fallos de página.
     * @return int Número de fallos de página.
     */
    int getFallosPagina() const;

private:
    unordered_map<int, PageEntry> table;  // Mapa de números de página a entradas de página.
    unordered_set<int> usedFrames;        // Conjunto de marcos físicos ocupados.
    const int numFrames;                            // Número de marcos físicos disponibles.
    int pageFaults = 0;                        // Contador de fallos de página.
};

#endif // TABLA_PAGINACION_H