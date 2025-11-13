#include "Grafo.h"
#include <fstream>
#include <iostream>

bool Grafo::cargarDesdeArchivo(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Error: No se pudo abrir el archivo: " << filename << std::endl;
        return false;
    }
    
    n = 0;
    in >> n;
    
    adj.resize(n);
    std::vector<std::unordered_set<int>> adj_set(n);
    
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
    
    return true;
}
