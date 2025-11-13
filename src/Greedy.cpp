#include "Greedy.h"
#include <algorithm>

std::vector<int> Greedy::generarSolucion(
    const std::vector<std::vector<int>>& adj,
    int k,
    std::mt19937& gen
) {
    int n = adj.size();
    if (n == 0) return {};
    
    // --- NUEVO: Crear una matriz de adyacencia local ---
    // Esto nos da búsquedas O(1) sin necesitar el adj_set permanente
    std::vector<std::vector<char>> adj_matrix(n, std::vector<char>(n, 0));
    for (int u = 0; u < n; ++u) {
        for (int v : adj[u]) {
            adj_matrix[u][v] = 1;
        }
    }
    // --- FIN NUEVO ---

    std::vector<int> curr_deg(n);
    for (int i = 0; i < n; ++i) curr_deg[i] = adj[i].size();
    
    std::vector<char> removed(n, 0);
    int remaining = n;
    std::vector<int> independent_set;
    
    while (remaining > 0) {
        std::vector<std::pair<double, std::vector<int>>> candidates;
        
        for (int a = 0; a < n; ++a) {
            if (removed[a]) continue;
            candidates.push_back({(double)curr_deg[a], {a}});
            
            for (int b = a + 1; b < n; ++b) {
                // --- CAMBIO CLAVE ---
                // Usamos la matriz O(1) en lugar del set
                if (removed[b] || adj_matrix[a][b]) continue;
                // --- FIN CAMBIO ---
                candidates.push_back({(double)(curr_deg[a] + curr_deg[b]) / 2.0, {a, b}});
            }
        }
        
        if (candidates.empty()) break;
        
        std::sort(candidates.begin(), candidates.end());
        int limit = std::min(k, (int)candidates.size());
        if (limit == 0) break;
        
        std::uniform_int_distribution<> dis(0, limit - 1);
        std::vector<int> bestH = candidates[dis(gen)].second;
        
        independent_set.insert(independent_set.end(), bestH.begin(), bestH.end());
        
        std::vector<int> Slist;
        std::vector<char> inS(n, 0);
        
        for (int x : bestH) {
            if (!inS[x]) {
                inS[x] = 1;
                Slist.push_back(x);
            }
        }
        
        for (int x : bestH) {
            for (int nb : adj[x]) {
                if (!inS[nb] && !removed[nb]) {
                    inS[nb] = 1;
                    Slist.push_back(nb);
                }
            }
        }
        
        for (int s : Slist) {
            if (removed[s]) continue;
            removed[s] = 1;
            remaining--;
            for (int nb : adj[s]) {
                if (!removed[nb]) curr_deg[nb]--;
            }
            curr_deg[s] = 0;
        }
    }
    
    return independent_set;
}

Individuo Greedy::crearIndividuo(
    const std::vector<std::vector<int>>& adj,
    int k,
    std::mt19937& gen
) {
    int n = adj.size();
    
    // La llamada ahora solo pasa 'adj'
    std::vector<int> independent_set = generarSolucion(adj, k, gen);
    
    Individuo ind(n);
    for (int nodo : independent_set) {
        ind.cromosoma[nodo] = true;
    }
    
    ind.fitness = independent_set.size();
    
    return ind;
}