#ifndef INDIVIDUO_H
#define INDIVIDUO_H

#include <vector>

/**
 * @brief Estructura que representa un individuo en el algoritmo genético
 * 
 * Usa una representación binaria donde:
 * - cromosoma[i] = true  -> el nodo i está en la solución
 * - cromosoma[i] = false -> el nodo i NO está en la solución
 */
struct Individuo {
    std::vector<bool> cromosoma;
    int fitness; // Tamaño del conjunto independiente (después de reparar)

    // Constructor
    Individuo(int n);
};

#endif // INDIVIDUO_H
