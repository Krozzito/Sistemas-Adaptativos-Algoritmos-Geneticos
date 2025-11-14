#ifndef OPERADORES_H
#define OPERADORES_H

#include <vector>
#include <utility>
#include <random>
#include <unordered_set>
#include "Individuo.h"

/**
 * @brief Clase con operadores genéticos del algoritmo
 */
class Operadores {
public:
    /**
     * @brief Repara un individuo para que sea un conjunto independiente válido
     *        y calcula su fitness
     * @param ind Individuo a reparar (modificado in-place)
     * @param adj_set Set de adyacencia del grafo
     */
    static void reparar_y_evaluar(
        Individuo& ind,
        const std::vector<std::unordered_set<int>>& adj_set
    );

    /**
     * @brief Selecciona un individuo usando torneo de tamaño k=2
     * @param poblacion Población actual
     * @param gen Generador de números aleatorios
     * @return Individuo seleccionado
     */
    static Individuo seleccionPorTorneo(
        const std::vector<Individuo>& poblacion,
        std::mt19937& gen
    );

    /**
     * @brief Aplica cruce uniforme entre dos padres
     * @param padre1 Primer padre
     * @param padre2 Segundo padre
     * @param p_cruce Probabilidad de aplicar cruce
     * @param gen Generador de números aleatorios
     * @return Par de hijos generados
     */
    static std::pair<Individuo, Individuo> cruceUniforme(
        const Individuo& padre1,
        const Individuo& padre2,
        double p_cruce,
        std::mt19937& gen
    );

    /**
     * @brief Aplica mutación bit-flip a un individuo
     * @param ind Individuo a mutar (modificado in-place)
     * @param p_mut Probabilidad de mutar cada bit
     * @param gen Generador de números aleatorios
     */
    static void mutacionBitFlip(
        Individuo& ind,
        double p_mut,
        std::mt19937& gen
    );

    /**
     * @brief Encuentra el mejor individuo de una población
     * @param poblacion Población actual
     * @return Mejor individuo
     */
    static Individuo obtenerMejor(const std::vector<Individuo>& poblacion);
};

#endif // OPERADORES_H
