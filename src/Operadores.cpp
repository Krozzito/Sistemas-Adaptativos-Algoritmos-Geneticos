#include "Operadores.h"

void Operadores::reparar_y_evaluar(
    Individuo& ind,
    const std::vector<std::unordered_set<int>>& adj_set
) {
    int n = ind.cromosoma.size();
    int fitness_calculado = 0;
    
    for (int u = 0; u < n; ++u) {
        if (ind.cromosoma[u] == false) {
            continue;
        }

        bool es_valido = true;
        for (int v : adj_set[u]) {
            if (v < u && ind.cromosoma[v] == true) {
                es_valido = false;
                break;
            }
        }

        if (es_valido) {
            ind.cromosoma[u] = true;
            fitness_calculado++;
        } else {
            ind.cromosoma[u] = false;
        }
    }
    
    ind.fitness = fitness_calculado;
}

Individuo Operadores::seleccionPorTorneo(
    const std::vector<Individuo>& poblacion,
    std::mt19937& gen
) {
    std::uniform_int_distribution<> dis_idx(0, poblacion.size() - 1);
    
    int idx1 = dis_idx(gen);
    int idx2 = dis_idx(gen);

    if (poblacion[idx1].fitness > poblacion[idx2].fitness) {
        return poblacion[idx1];
    } else {
        return poblacion[idx2];
    }
}

std::pair<Individuo, Individuo> Operadores::cruceUniforme(
    const Individuo& padre1,
    const Individuo& padre2,
    double p_cruce,
    std::mt19937& gen
) {
    std::uniform_real_distribution<> dis_prob(0.0, 1.0);

    if (dis_prob(gen) > p_cruce) {
        return {padre1, padre2};
    }

    int n = padre1.cromosoma.size();
    Individuo hijo1(n);
    Individuo hijo2(n);

    for (int i = 0; i < n; ++i) {
        if (dis_prob(gen) < 0.5) {
            hijo1.cromosoma[i] = padre1.cromosoma[i];
            hijo2.cromosoma[i] = padre2.cromosoma[i];
        } else {
            hijo1.cromosoma[i] = padre2.cromosoma[i];
            hijo2.cromosoma[i] = padre1.cromosoma[i];
        }
    }

    return {hijo1, hijo2};
}

void Operadores::mutacionBitFlip(
    Individuo& ind,
    double p_mut,
    std::mt19937& gen
) {
    std::uniform_real_distribution<> dis_prob(0.0, 1.0);
    
    for (size_t i = 0; i < ind.cromosoma.size(); ++i) {
        if (dis_prob(gen) < p_mut) {
            ind.cromosoma[i] = !ind.cromosoma[i];
        }
    }
}

Individuo Operadores::obtenerMejor(const std::vector<Individuo>& poblacion) {
    Individuo mejor = poblacion[0];
    for (const Individuo& ind : poblacion) {
        if (ind.fitness > mejor.fitness) {
            mejor = ind;
        }
    }
    return mejor;
}
