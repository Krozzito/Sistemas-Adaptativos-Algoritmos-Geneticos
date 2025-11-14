#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <unordered_set>
#include <string>

/**
 * @brief Clase para manejar la carga y representación del grafo
 */
class Grafo {
public:
    int n; // Número de nodos
    std::vector<std::vector<int>> adj;           // Lista de adyacencia
    std::vector<std::unordered_set<int>> adj_set; // Set de adyacencia (búsqueda O(1))

    /**
     * @brief Carga un grafo desde un archivo
     * @param filename Ruta al archivo del grafo
     * @return true si la carga fue exitosa, false en caso contrario
     */
    bool cargarDesdeArchivo(const std::string& filename);
};

#endif // GRAFO_H
