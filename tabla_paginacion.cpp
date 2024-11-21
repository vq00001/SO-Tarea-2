#include <iostream>
#include <unordered_map>
#include <string>
#include <unordered_set>

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
private:
    unordered_map<int, PageEntry> table;  // Tabla hash para almacenar las entradas de página
    unordered_set<int> usedFrames;        // Rastrear marcos ocupados
    const int numFrames;                  // Número de marcos físicos (constante)
    int pageFaults = 0;                   // Contador de fallos de página

public:
    /**
     * @brief Constructor de la clase PageTable.
     * 
     * @param frames Número de marcos físicos en el sistema.
     */
    PageTable(int frames) : numFrames(frames) {}

    /**
     * @brief Método para insertar una página en la tabla
     * @param pageNumber: Número de página
     * @param frameNumber: Número de marco físico asociado
     */
    void insertPage(int pageNumber, int frameNumber) {
        
        // Verificar si el número de marco es válido
        if (frameNumber >= numFrames || frameNumber < 0) {
            cout << "Número de marco inválido.\n";
            return;
        }

        // Verificar si la tabla de páginas está llena
        if (static_cast<int>(table.size()) >= numFrames) {
            cout << "Tabla de páginas llena. Se necesita expulsar una página.\n";
            return;
        }

        // Verificar si el marco ya está ocupado
        if (usedFrames.find(frameNumber) != usedFrames.end()) {
            cout << "Error: El marco " << frameNumber << " ya está ocupado.\n";
            return;
        }

        // Verificar si la página ya está asignada
        if (table.find(pageNumber) != table.end()) {
            cout << "La página " << pageNumber << " ya está asignada al marco " << table[pageNumber].frameNumber << ".\n";
            return;
        }

        // Crear una nueva entrada de página
        PageEntry entry = {frameNumber, true};
        table[pageNumber] = entry;
        usedFrames.insert(frameNumber);
        pageFaults++; // Incrementar el contador de fallos de página

        cout << "Página " << pageNumber << " mapeada al marco " << frameNumber << ".\n";
    }

    /**
     * @brief Método para buscar una página en la tabla
     * @param pageNumber: Número de página a buscar
     * @return Número de marco físico asociado a la página, o -1 si no se encuentra
     */
    int getFrame(int pageNumber) const {
        auto it = table.find(pageNumber);
        if (it != table.end() && it->second.valid) {
            cout << "Página " << pageNumber << " encontrada en el marco " << it->second.frameNumber << ".\n";
            return it->second.frameNumber;
        }
        cout << "Página " << pageNumber << " no encontrada.\n";
        return -1;
    }

    /**
     * @brief Método para eliminar una página de la tabla
     * @param pageNumber: Número de página a eliminar
     */
    void removePage(int pageNumber) {
        auto it = table.find(pageNumber);
        if (it != table.end()) {
            usedFrames.erase(it->second.frameNumber);
            table.erase(it);
            cout << "Página " << pageNumber << " eliminada.\n";
            _sleep(1000); // Simular un retraso de 1 segundo
        } else {
            cout << "Página " << pageNumber << " no encontrada.\n";
        }
    }

    /**
     * @brief Método para invalidar una página
     * @param pageNumber: Número de página a invalidar
     */
    void invalidatePage(int pageNumber) {
        auto it = table.find(pageNumber);
        if (it != table.end()) {
            it->second.valid = false;
            cout << "Página " << pageNumber << " invalidada.\n";
        } else {
            cout << "Página " << pageNumber << " no existe.\n";
        }
    }

    /**
     * @brief Método para reemplazar una página por otra
     * @param pageNumber: Número de página por ingresar
     * @param pageToReplace: Número de página a reemplazar
     */
    void replacePage(int pageNumber, int pageToReplace) {
        int frameNumber = getFrame(pageToReplace);
        removePage(pageToReplace);
        insertPage(pageNumber, frameNumber);
        cout << "Página " << pageNumber << " reemplazada por el marco " << frameNumber << ".\n";
    }

    /**
     * @brief Método para mostrar el contenido de la tabla
     */
    void displayTable() const {
        cout << "Contenido de la tabla de páginas:\n";
        for (const auto& pair : table) {
            cout << "Página " << pair.first << ": [Marco: " << pair.second.frameNumber << ", Validez: " << pair.second.valid << "]\n";
        }
    }
};