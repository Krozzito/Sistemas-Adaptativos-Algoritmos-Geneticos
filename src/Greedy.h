#ifndef GREEDY_H
#define GREEDY_H

#include <vector>
#include <unordered_set>
#include <random>
#include "Individuo.h"

/**
 * @brief Clase con funcionalidad de heurística greedy para seeding
 */
class Greedy {
public:
    /**
     * @brief Genera una solución usando heurística greedy semi-aleatoria
     * @param adj Lista de adyacencia
     * @param adj_set Set de adyacencia
     * @param k Parámetro de aleatoriedad (menor = más aleatorio)
     * @param gen Generador de números aleatorios
     * @return Vector con los nodos del conjunto independiente
     */
    static std::vector<int> generarSolucion(
        const std::vector<std::vector<int>>& adj,
        int k,
        std::mt19937& gen
    );

    /**
     * @brief Crea un individuo usando la heurística greedy
     * @param adj Lista de adyacencia
     * @param adj_set Set de adyacencia
     * @param k Parámetro de aleatoriedad
     * @param gen Generador de números aleatorios
     * @return Individuo con solución greedy
     */
    static Individuo crearIndividuo(
        const std::vector<std::vector<int>>& adj,
        int k,
        std::mt19937& gen
    );
};

#endif // GREEDY_H
