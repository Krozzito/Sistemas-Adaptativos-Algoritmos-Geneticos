#include "AlgoritmoGenetico.h"
#include "Operadores.h"
#include "Greedy.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>

using namespace std;
using Clock = chrono::high_resolution_clock;

void AlgoritmoGenetico::inicializarPoblacion(
    vector<Individuo>& poblacion,
    const Grafo& grafo,
    mt19937& gen,
    int k_greedy,
    double seeding_rate
) {
    int pop_size = poblacion.size();
    int n = (pop_size > 0) ? poblacion[0].cromosoma.size() : 0;
    if (n == 0) return;

    int n_greedy = static_cast<int>(pop_size * seeding_rate);

    // Crear individuos greedy
    for (int i = 0; i < n_greedy; ++i) {
        poblacion[i] = Greedy::crearIndividuo(grafo.adj, grafo.adj_set, k_greedy, gen);
    }

    // Crear individuos aleatorios
    uniform_int_distribution<> dis_bit(0, 1);
    for (int i = n_greedy; i < pop_size; ++i) {
        for (int j = 0; j < n; ++j) {
            poblacion[i].cromosoma[j] = (dis_bit(gen) == 1);
        }
        Operadores::reparar_y_evaluar(poblacion[i], grafo.adj_set);
    }
}

void AlgoritmoGenetico::ejecutar(const ParametrosGA& params) {
    // Cargar grafo
    Grafo grafo;
    if (!grafo.cargarDesdeArchivo(params.instancia)) {
        return;
    }

    // Inicio del algoritmo
    auto start_time = Clock::now();
    mt19937 gen(random_device{}());

    // Crear población inicial
    vector<Individuo> poblacion;
    for (int i = 0; i < params.pop_size; ++i) {
        poblacion.push_back(Individuo(grafo.n));
    }
    
    inicializarPoblacion(poblacion, grafo, gen, params.k_greedy, params.seeding_rate);

    // Encontrar la mejor solución inicial
    Individuo mejor_solucion_global = Operadores::obtenerMejor(poblacion);
    auto time_found = Clock::now();
    double elapsed_found = chrono::duration<double>(time_found - start_time).count();

    // Imprimir primera solución
    cout << "Calidad: " << mejor_solucion_global.fitness 
         << ", Tiempo: " << fixed << setprecision(4) << elapsed_found << "s" << endl;
    
    // Bucle principal del GA
    while (true) {
        auto current_time = Clock::now();
        double elapsed_total = chrono::duration<double>(current_time - start_time).count();
        if (elapsed_total >= params.max_time) break;

        // Lógica de una generación
        vector<Individuo> nueva_poblacion;
        while (nueva_poblacion.size() < static_cast<size_t>(params.pop_size)) {
            // Selección
            Individuo padre1 = Operadores::seleccionPorTorneo(poblacion, gen);
            Individuo padre2 = Operadores::seleccionPorTorneo(poblacion, gen);

            // Cruce
            pair<Individuo, Individuo> hijos = 
                Operadores::cruceUniforme(padre1, padre2, params.p_cruce, gen);

            // Mutación
            Operadores::mutacionBitFlip(hijos.first, params.p_mut, gen);
            Operadores::mutacionBitFlip(hijos.second, params.p_mut, gen);

            // Evaluación
            Operadores::reparar_y_evaluar(hijos.first, grafo.adj_set);
            Operadores::reparar_y_evaluar(hijos.second, grafo.adj_set);

            nueva_poblacion.push_back(hijos.first);
            if (nueva_poblacion.size() < static_cast<size_t>(params.pop_size)) {
                nueva_poblacion.push_back(hijos.second);
            }
        }
        
        // Reemplazo generacional
        poblacion = nueva_poblacion;

        // Reporte any-time
        Individuo mejor_generacion = Operadores::obtenerMejor(poblacion);
        if (mejor_generacion.fitness > mejor_solucion_global.fitness) {
            mejor_solucion_global = mejor_generacion;
            time_found = Clock::now();
            elapsed_found = chrono::duration<double>(time_found - start_time).count();
            
            cout << "Calidad: " << mejor_solucion_global.fitness 
                 << ", Tiempo: " << fixed << setprecision(4) << elapsed_found << "s" << endl;
        }
    }
    
    // Reporte final
    cout << "Final: " << mejor_solucion_global.fitness 
         << ", Tiempo: " << fixed << setprecision(4) << elapsed_found << "s" << endl;
}
