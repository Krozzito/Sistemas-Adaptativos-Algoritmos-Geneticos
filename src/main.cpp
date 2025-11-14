#include <iostream>
#include <string>
#include "AlgoritmoGenetico.h"

using namespace std;

void mostrarUso() {
    cout << "Uso: ./ga -i <archivo_grafo> -t <tiempo_segundos> [opciones]" << endl;
    cout << "\nOpciones:" << endl;
    cout << "  -i <archivo>        Archivo del grafo (REQUERIDO)" << endl;
    cout << "  -t <segundos>       Tiempo máximo de ejecución (REQUERIDO)" << endl;
    cout << "  --bestP             Los mejores Parametros por Irace (REQUERIDO) " << endl;
    cout << "  --pop_size <N>      Tamaño de la población (default: 100)" << endl;
    cout << "  --p_cruce <P>       Probabilidad de cruce (default: 0.9)" << endl;
    cout << "  --p_mut <P>         Probabilidad de mutación (default: 0.01)" << endl;
    cout << "  --k_greedy <K>      Parámetro greedy (default: 10)" << endl;
    cout << "  --seeding_rate <R>  Proporción de seeding (default: 0.2)" << endl;
    cout << "  --seed <S>          Seed aleatoria (default: -1 = aleatorio)" << endl;
}

int main(int argc, char** argv) {
    ParametrosGA params;


    bool usarBestP = false;
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--bestP") {
            usarBestP = true;
            break;
        }
    }
    if (usarBestP) {
        params.pop_size = 299;
        params.p_cruce = 0.76;
        params.p_mut = 0.14;
        params.k_greedy = 11;
        params.seeding_rate = 0.44;
    }
    // Parsear argumentos
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        
        if (arg == "-i" && i + 1 < argc) {
            params.instancia = argv[++i];
        } else if (arg == "-t" && i + 1 < argc) {
            params.max_time = stod(argv[++i]);
        } else if (arg == "--pop_size" && i + 1 < argc) {
            params.pop_size = stoi(argv[++i]);
        } else if (arg == "--p_cruce" && i + 1 < argc) {
            params.p_cruce = stod(argv[++i]);
        } else if (arg == "--p_mut" && i + 1 < argc) {
            params.p_mut = stod(argv[++i]);
        } else if (arg == "--k_greedy" && i + 1 < argc) {
            params.k_greedy = stoi(argv[++i]);
        } else if (arg == "--seeding_rate" && i + 1 < argc) {
            params.seeding_rate = stod(argv[++i]);
        } else if (arg == "--seed" && i + 1 < argc) {
            params.seed = stoi(argv[++i]);
        } else if (arg == "--help" || arg == "-h") {
            mostrarUso();
            return 0;
        }else if (arg == "--bestP") {
            // --- NUEVO ---
            // Ya lo procesamos, no hacer nada.
            ; 
        }
    }

    // Validar parámetros obligatorios
    if (params.instancia.empty()) {
        cerr << "Error: Se requiere el nombre de la instancia con -i <archivo>" << endl;
        mostrarUso();
        return 1;
    }
    
    if (params.max_time <= 0) {
         cerr << "Error: Se requiere un tiempo de ejecución válido con -t <segundos>" << endl;
         mostrarUso();
         return 1;
    }

    // Ejecutar algoritmo
    AlgoritmoGenetico::ejecutar(params);

    return 0;
}
