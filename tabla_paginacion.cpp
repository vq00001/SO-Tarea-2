#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

// Estructura para representar una entrada en la tabla de páginas
struct PageEntry {
    int frameNumber;  // Número de marco físico asociado
    bool valid;       // Bit de validez
};

// Clase para la tabla de paginación
class PageTable {
private:
    unordered_map<int, PageEntry> table;  // Tabla hash para almacenar las entradas de página
    int numFrames;                        // Número de marcos físicos

public:
    // Constructor
    PageTable(int frames) : numFrames(frames) {}

    /*
    Método para insertar una página en la tabla
    @param pageNumber: Número de página
    @param frameNumber: Número de marco físico asociado
    */
    void insertPage(int pageNumber, int frameNumber) {
        if (table.find(pageNumber) != table.end()) {
            cout << "Página " << pageNumber << " ya existe.\n";
            return;
        }

        if (table.size() >= numFrames) {
            cout << "Tabla de páginas llena. Se necesita expulsar una página.\n";
            return;
        }

        if (frameNumber >= numFrames || frameNumber < 0) {
            cout << "Número de marco inválido.\n";
            return;
        }

        // Crear una nueva entrada de página
        PageEntry entry = {frameNumber, true};
        table[pageNumber] = entry;
        cout << "Página " << pageNumber << " mapeada al marco " << frameNumber << ".\n";
    }

    /*
    Método para buscar una página en la tabla
    @param pageNumber: Número de página a buscar
    @return Número de marco físico asociado a la página, o -1 si no se encuentra
    */
    int getFrame(int pageNumber) const {
        auto it = table.find(pageNumber);
        if (it != table.end() && it->second.valid) {
            cout << "Página " << pageNumber << " encontrada en el marco " << it->second.frameNumber << ".\n";
            return it->second.frameNumber;
        }
        cout << "Página " << pageNumber << " no está en memoria.\n";
        return -1; // Indica fallo de página
    }

    /*
    Método para invalidar una página (simula la expulsión de una página)
    @param pageNumber: Número de página a invalidar
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

    /*
    Método para mostrar el contenido de la tabla
    */
    void displayTable() const {
        cout << "Contenido de la tabla de páginas:\n";
        for (const auto& pair : table) {
            cout << "Página " << pair.first << ": [Marco: " << pair.second.frameNumber << ", Validez: " << pair.second.valid << "]\n";
        }
    }
};

int main() { // Prueba de la tabla de páginas
    // Crear una tabla de páginas con 3 marcos iniciales
    PageTable pageTable(3);

    // Insertar páginas
    pageTable.insertPage(1, 0);
    pageTable.insertPage(2, 1);
    pageTable.insertPage(3, 2);

    // Mostrar el contenido de la tabla
    pageTable.displayTable();

    // Insertar una página que ya existe
    pageTable.insertPage(1, 3);

    // Buscar una página
    pageTable.getFrame(1);

    // Buscar una página que no está en memoria
    pageTable.getFrame(4);

    // Invalidar una página
    pageTable.invalidatePage(2);

    // Buscar una página que ha sido invalidada
    pageTable.getFrame(2);

    // Invalidar una página que no existe
    pageTable.invalidatePage(5);

    // Mostrar el contenido de la tabla nuevamente
    pageTable.displayTable();

    return 0;
}
