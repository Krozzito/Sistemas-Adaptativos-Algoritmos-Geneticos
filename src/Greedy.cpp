#include "Greedy.h"
#include <algorithm>

std::vector<int> Greedy::generarSolucion(
    const std::vector<std::vector<int>>& adj,
    const std::vector<std::unordered_set<int>>& adj_set,
    int k,
    std::mt19937& gen
) {
    int n = adj.size();
    if (n == 0) return {};
    
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
                if (removed[b] || adj_set[a].count(b)) continue;
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
    const std::vector<std::unordered_set<int>>& adj_set,
    int k,
    std::mt19937& gen
) {
    int n = adj.size();
    
    std::vector<int> independent_set = generarSolucion(adj, adj_set, k, gen);
    
    Individuo ind(n);
    for (int nodo : independent_set) {
        ind.cromosoma[nodo] = true;
    }
    
    ind.fitness = independent_set.size();
    
    return ind;
}
