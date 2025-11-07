#ifndef ALGORITMO_GENETICO_H
#define ALGORITMO_GENETICO_H

#include <string>
#include <vector>
#include <random>
#include "Grafo.h"
#include "Individuo.h"

/**
 * @brief Parámetros del algoritmo genético
 */
struct ParametrosGA {
    std::string instancia;
    double max_time;
    int pop_size;
    double p_cruce;
    double p_mut;
    int k_greedy;
    double seeding_rate;

    // Valores por defecto
    ParametrosGA() 
        : instancia(""),
          max_time(10.0),
          pop_size(100),
          p_cruce(0.9),
          p_mut(0.01),
          k_greedy(10),
          seeding_rate(0.2) {}
};

/**
 * @brief Clase principal del algoritmo genético
 */
class AlgoritmoGenetico {
public:
    /**
     * @brief Ejecuta el algoritmo genético para MISP
     * @param params Parámetros del algoritmo
     */
    static void ejecutar(const ParametrosGA& params);

private:
    /**
     * @brief Inicializa la población con seeding
     */
    static void inicializarPoblacion(
        std::vector<Individuo>& poblacion,
        const Grafo& grafo,
        std::mt19937& gen,
        int k_greedy,
        double seeding_rate
    );
};

#endif // ALGORITMO_GENETICO_H
