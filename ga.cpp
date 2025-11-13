#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <unordered_set>
#include <iomanip> // Para setprecision
#include <random>
#include <algorithm>
#include <sstream>
#include <string>
#include <cmath>
#include <numeric> // Para std::iota

using namespace std;
using Clock = chrono::high_resolution_clock;

// --- ESTRUCTURA DEL INDIVIDUO ---
// Usaremos un cromosoma binario (vector<bool>)
// cromosoma[i] = true  -> el nodo i ESTÁ en la solución
// cromosoma[i] = false -> el nodo i NO ESTÁ en la solución

struct Individuo {
    vector<bool> cromosoma;
    int fitness; // (El tamaño del IS después de reparar)

    // Constructor
    Individuo(int n) : cromosoma(n), fitness(0) {}
};

// Funciones de la Heurística Greedy (Seeding)
vector<int> generarSolucionGreedy(const vector<vector<int>>& adj, const vector<unordered_set<int>>& adj_set, int k, mt19937& gen);
Individuo crearIndividuoGreedy(const vector<vector<int>>& adj, const vector<unordered_set<int>>& adj_set, int k, mt19937& gen);

// Funciones del Algoritmo Genético
void inicializarPoblacion(vector<Individuo>& poblacion, const vector<vector<int>>& adj, const vector<unordered_set<int>>& adj_set,   mt19937& gen,   int k_greedy, double seeding_rate); // <-- Firma corregida
void reparar_y_evaluar(Individuo& ind, const vector<unordered_set<int>>& adj_set);
Individuo seleccionPorTorneo(const vector<Individuo>& poblacion, mt19937& gen);
pair<Individuo, Individuo> cruceUniforme(const Individuo& padre1, const Individuo& padre2, double p_cruce, mt19937& gen);
void mutacionBitFlip(Individuo& ind, double p_mut, mt19937& gen);
Individuo obtenerMejor(const vector<Individuo>& poblacion);

void geneticAlgorithm(const string& instancia, double max_time, int pop_size, 
                      double p_cruce, double p_mut, 
                      int k_greedy, double seeding_rate, int seed = -1) { 
    
    // --- Carga del grafo (MODIFICADO para cargar adj y adj_set) ---
    ifstream in(instancia);
    if (!in) {
        cerr << "Error: No se pudo abrir el archivo: " << instancia << endl;
        return;
    }
    int n = 0;
    in >> n; 
    
    vector<vector<int>> adj(n); 
    vector<unordered_set<int>> adj_set(n);
    int u, v;
    while (in >> u >> v) {
        if (u < 0 || u >= n || v < 0 || v >= n || u == v) continue;
        
        // Usamos adj_set.insert().second para no duplicar aristas en 'adj'
        if (adj_set[u].insert(v).second) { 
            adj[u].push_back(v);
        }
        if (adj_set[v].insert(u).second) {
            adj[v].push_back(u);
        }
    }
    in.close();

    // --- Inicio del Algoritmo ---
    auto start_time = Clock::now();
    
    // Configuración del generador aleatorio con seed configurable
    mt19937 gen;
    if (seed >= 0) {
        gen.seed(seed);  // Seed fija (reproducible)
    } else {
        gen.seed(random_device{}());  // Seed aleatoria (no reproducible)
    }

    // 1. Crear Población Inicial
    vector<Individuo> poblacion;
    for(int i = 0; i < pop_size; ++i) {
        poblacion.push_back(Individuo(n));
    }
    
    // Llamada MODIFICADA a inicializarPoblacion
    inicializarPoblacion(poblacion, adj, adj_set, gen, k_greedy, seeding_rate); 

    // 2. Encontrar la mejor solución inicial
    Individuo mejor_solucion_global = obtenerMejor(poblacion);
    
    // 3. Primera solución (comentado para output limpio)
    // cout << "Calidad: " << mejor_solucion_global.fitness << ", Tiempo: " << fixed << setprecision(4) << elapsed_found << "s" << endl;
    
    // --- Bucle principal del GA---
    while (true) {
        auto current_time = Clock::now();
        double elapsed_total = chrono::duration<double>(current_time - start_time).count();
        if (elapsed_total >= max_time) break;

        // --- LÓGICA DE UNA GENERACIÓN ---
        vector<Individuo> nueva_poblacion;
        while (nueva_poblacion.size() < pop_size) {
            // 1. Selección
            Individuo padre1 = seleccionPorTorneo(poblacion, gen);
            Individuo padre2 = seleccionPorTorneo(poblacion, gen);

            // 2. Cruce
            pair<Individuo, Individuo> hijos = cruceUniforme(padre1, padre2, p_cruce, gen);

            // 3. Mutación
            mutacionBitFlip(hijos.first, p_mut, gen);
            mutacionBitFlip(hijos.second, p_mut, gen);

            // 4. Evaluación (Reparación + Fitness)
            reparar_y_evaluar(hijos.first, adj_set);
            reparar_y_evaluar(hijos.second, adj_set);

            nueva_poblacion.push_back(hijos.first);
            if (nueva_poblacion.size() < pop_size) {
                nueva_poblacion.push_back(hijos.second);
            }
        }
        
        // Reemplazo generacional
        poblacion = nueva_poblacion;

        // --- REPORTE ANY-TIME (comentado para output limpio) ---
        Individuo mejor_generacion = obtenerMejor(poblacion);
        if (mejor_generacion.fitness > mejor_solucion_global.fitness) {
            mejor_solucion_global = mejor_generacion;
            
            // Salida comentada (anytime reporting desactivado)
            // cout << "Calidad: " << mejor_solucion_global.fitness << ", Tiempo: " << fixed << setprecision(4) << elapsed_found << "s" << endl;
        }
    }
    
    // --- Reporte Final: SOLO el número ---
    cout << mejor_solucion_global.fitness << endl;
}

vector<int> generarSolucionGreedy(const vector<vector<int>>& adj, const vector<unordered_set<int>>& adj_set, int k, mt19937& gen) {
    int n = adj.size();
    if (n == 0) return {};
    vector<int> curr_deg(n);
    for (int i = 0; i < n; ++i) curr_deg[i] = adj[i].size();
    vector<char> removed(n, 0);
    int remaining = n;
    vector<int> independent_set;
    
    // mt19937 gen(random_device{}()); // <-- ESTA LÍNEA SE ELIMINA

    while (remaining > 0) {
        vector<pair<double, vector<int>>> candidates;
        for (int a = 0; a < n; ++a) {
            if (removed[a]) continue;
            candidates.push_back({(double)curr_deg[a], {a}});
            for (int b = a + 1; b < n; ++b) {
                if (removed[b] || adj_set[a].count(b)) continue;
                candidates.push_back({(double)(curr_deg[a] + curr_deg[b]) / 2.0, {a, b}});
            }
        }
        if (candidates.empty()) break;
        sort(candidates.begin(), candidates.end());
        int limit = min(k, (int)candidates.size());
        if (limit == 0) break;
        uniform_int_distribution<> dis(0, limit - 1);
        vector<int> bestH = candidates[dis(gen)].second;
        independent_set.insert(independent_set.end(), bestH.begin(), bestH.end());
        vector<int> Slist; vector<char> inS(n, 0);
        for (int x : bestH) { if (!inS[x]) { inS[x] = 1; Slist.push_back(x); } }
        for (int x : bestH) { for (int nb : adj[x]) { if (!inS[nb] && !removed[nb]) { inS[nb] = 1; Slist.push_back(nb); } } }
        for (int s : Slist) { if (removed[s]) continue; removed[s] = 1; remaining--; for (int nb : adj[s]) { if (!removed[nb]) curr_deg[nb]--; } curr_deg[s] = 0; }
    }
    return independent_set;
}
Individuo crearIndividuoGreedy(const vector<vector<int>>& adj, const vector<unordered_set<int>>& adj_set, int k, mt19937& gen) {
    int n = adj.size();
    
    // 1. Llama a tu heurística para obtener la lista de nodos
    vector<int> independent_set = generarSolucionGreedy(adj, adj_set, k, gen);

    // 2. Convierte el vector<int> a un cromosoma vector<bool>
    Individuo ind(n);
    for (int nodo : independent_set) {
        ind.cromosoma[nodo] = true;
    }

    // 3. Como la heurística YA es válida, no necesita reparación.
    // El fitness es simplemente el tamaño del conjunto.
    ind.fitness = independent_set.size();
    
    return ind;
}

/**
 * @brief (MODIFICADA) Llena la población inicial con "sembrado" (seeding).
 */
void inicializarPoblacion(vector<Individuo>& poblacion, 
                          const vector<vector<int>>& adj, // <-- Parámetro nuevo
                          const vector<unordered_set<int>>& adj_set, 
                          mt19937& gen, 
                          int k_greedy,       // <-- Parámetro nuevo
                          double seeding_rate) { // <-- Parámetro nuevo
    
    int pop_size = poblacion.size();
    int n = (pop_size > 0) ? poblacion[0].cromosoma.size() : 0;
    if (n == 0) return;

    // Calcula cuántos individuos serán "greedy"
    int n_greedy = static_cast<int>(pop_size * seeding_rate);

    // 1. Crear individuos "Greedy"
    for (int i = 0; i < n_greedy; ++i) {
        poblacion[i] = crearIndividuoGreedy(adj, adj_set, k_greedy, gen);
    }

    // 2. Crear individuos "Aleatorios" (el resto)
    uniform_int_distribution<> dis_bit(0, 1);
    for (int i = n_greedy; i < pop_size; ++i) {
        // 'poblacion[i]' ya es un Individuo(n) vacío
        for (int j = 0; j < n; ++j) {
            poblacion[i].cromosoma[j] = (dis_bit(gen) == 1);
        }
        // ¡Importante! Reparar y evaluar cada individuo ALEATORIO
        reparar_y_evaluar(poblacion[i], adj_set);
    }
}

/**
 * @brief (3) Selecciona un individuo usando Torneo de tamaño k=2.
 */
Individuo seleccionPorTorneo(const vector<Individuo>& poblacion, mt19937& gen) {
    // --- TODO: Implementar la selección por torneo ---
    
    // (Esta es una implementación estándar de k=2)
    uniform_int_distribution<> dis_idx(0, poblacion.size() - 1);
    
    int idx1 = dis_idx(gen);
    int idx2 = dis_idx(gen);

    if (poblacion[idx1].fitness > poblacion[idx2].fitness) {
        return poblacion[idx1];
    } else {
        return poblacion[idx2];
    }
}


pair<Individuo, Individuo> cruceUniforme(const Individuo& padre1, const Individuo& padre2, double p_cruce, mt19937& gen) {
    uniform_real_distribution<> dis_prob(0.0, 1.0);

    // Respetar la probabilidad de cruce
    if (dis_prob(gen) > p_cruce) {
        return {padre1, padre2}; // Devuelve los padres sin cruzar
    }

    // --- TODO: Implementar el cruce uniforme ---
    int n = padre1.cromosoma.size();
    Individuo hijo1(n);
    Individuo hijo2(n);

    // (Esta es una implementación estándar de cruce uniforme)
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
/**
 * @brief (5) Muta un individuo usando Bit-Flip.
 */
void mutacionBitFlip(Individuo& ind, double p_mut, mt19937& gen) {
    uniform_real_distribution<> dis_prob(0.0, 1.0);
    
    // --- TODO: Implementar la mutación bit-flip ---
    // (Implementación estándar)
    for (int i = 0; i < ind.cromosoma.size(); ++i) {
        if (dis_prob(gen) < p_mut) {
            ind.cromosoma[i] = !ind.cromosoma[i]; // Voltea el bit
        }
    }
}

/**
 * @brief (Función auxiliar) Encuentra el mejor individuo en una población.
 */
Individuo obtenerMejor(const vector<Individuo>& poblacion) {
    Individuo mejor = poblacion[0];
    for (const Individuo& ind : poblacion) {
        if (ind.fitness > mejor.fitness) {
            mejor = ind;
        }
    }
    return mejor;
}
void reparar_y_evaluar(Individuo& ind, const vector<unordered_set<int>>& adj_set) {
    int n = ind.cromosoma.size();

    
    int fitness_calculado = 0;
    for (int u = 0; u < n; ++u) {
        
        // Si el cromosoma dice "no incluir u", no lo contamos.
        if (ind.cromosoma[u] == false) {
            continue;
        }

        // El cromosoma dice "incluir u". Vamos a ver si es válido.
        bool es_valido = true;
        for (int v : adj_set[u]) {
            // Solo nos importa si hay conflicto con un nodo ANTERIOR
            // que YA decidimos incluir.
            if (v < u && ind.cromosoma[v] == true) {
                es_valido = false;
                break; // Conflicto encontrado
            }
        }

        if (es_valido) {
            // 'u' está en el cromosoma y no tiene conflictos con
            // nodos anteriores (v < u) que también estén.
            // LO ACEPTAMOS.
            ind.cromosoma[u] = true; // (Ya era true, pero lo confirmamos)
            fitness_calculado++;
        } else {
            // 'u' está en el cromosoma pero entra en conflicto.
            // LO RECHAZAMOS (reparamos).
            ind.cromosoma[u] = false; 
        }
    }
    
    ind.fitness = fitness_calculado;
}

int main(int argc, char** argv) {
 // Parámetros por defecto para el GA
    string instancia = "";
    double max_time = 10.0;
    int pop_size = 100;     
    double p_cruce = 0.9;   
    double p_mut = 0.01;
    
    // Parámetros nuevos para el "seeding"
    int k_greedy = 10;           // (Valor de tu SA)
    double seeding_rate = 0.2;   // (20% de la población será "greedy")
    int seed = -1;               // -1 = aleatorio, >= 0 = fija

    // Lectura de argumentos (AÑADIR los nuevos)
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-i" && i + 1 < argc) instancia = argv[++i];
        else if (arg == "-t" && i + 1 < argc) max_time = stod(argv[++i]);
        // Parámetros del GA
        else if (arg == "--pop_size" && i + 1 < argc) pop_size = stoi(argv[++i]);
        else if (arg == "--p_cruce" && i + 1 < argc) p_cruce = stod(argv[++i]);
        else if (arg == "--p_mut" && i + 1 < argc) p_mut = stod(argv[++i]);
        // Parámetros nuevos para irace
        else if (arg == "--k_greedy" && i + 1 < argc) k_greedy = stoi(argv[++i]);
        else if (arg == "--seeding_rate" && i + 1 < argc) seeding_rate = stod(argv[++i]);
        // Parámetro de seed
        else if (arg == "--seed" && i + 1 < argc) seed = stoi(argv[++i]);
    }

    if (instancia.empty()) {
        cerr << "Error: Se requiere el nombre de la instancia con -i <archivo>" << endl;
        // (Tu mensaje de uso...)
        return 1;
    }

    // Llamada MODIFICADA a geneticAlgorithm
    geneticAlgorithm(instancia, max_time, pop_size, p_cruce, p_mut, k_greedy, seeding_rate, seed);

    return 0;
}


