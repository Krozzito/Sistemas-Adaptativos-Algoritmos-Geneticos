#!/bin/bash

# Ejemplos de uso del Algoritmo Genético - MISP
# Este script muestra diferentes formas de ejecutar el algoritmo

echo "======================================================"
echo "  EJEMPLOS DE USO - ALGORITMO GENÉTICO PARA MISP"
echo "======================================================"
echo ""

# Verificar que el ejecutable existe
if [ ! -f "./ga" ]; then
    echo "Error: El ejecutable './ga' no existe."
    echo "Compila primero con: g++ -std=c++17 -O3 -o ga ga.cpp"
    exit 1
fi

echo "=== EJEMPLO 1: Ejecución Básica ==="
echo "Comando: ./ga -i tests/test_small.graph -t 5"
echo "Descripción: Ejecuta el algoritmo durante 5 segundos con parámetros por defecto"
echo ""
./ga -i tests/test_small.graph -t 5
echo ""
read -p "Presiona Enter para continuar..."
echo ""

echo "=== EJEMPLO 2: Población Grande ==="
echo "Comando: ./ga -i tests/test_small.graph -t 5 --pop_size 200"
echo "Descripción: Usa una población de 200 individuos para mayor diversidad"
echo ""
./ga -i tests/test_small.graph -t 5 --pop_size 200
echo ""
read -p "Presiona Enter para continuar..."
echo ""

echo "=== EJEMPLO 3: Alta Exploración ==="
echo "Comando: ./ga -i tests/test_small.graph -t 5 --p_mut 0.05 --seeding_rate 0.1"
echo "Descripción: Mayor mutación y menos seeding para explorar más el espacio de búsqueda"
echo ""
./ga -i tests/test_small.graph -t 5 --p_mut 0.05 --seeding_rate 0.1
echo ""
read -p "Presiona Enter para continuar..."
echo ""

echo "=== EJEMPLO 4: Alta Explotación ==="
echo "Comando: ./ga -i tests/test_small.graph -t 5 --p_mut 0.001 --seeding_rate 0.5"
echo "Descripción: Baja mutación y alto seeding para intensificar búsqueda local"
echo ""
./ga -i tests/test_small.graph -t 5 --p_mut 0.001 --seeding_rate 0.5
echo ""
read -p "Presiona Enter para continuar..."
echo ""

echo "=== EJEMPLO 5: Configuración Personalizada Completa ==="
echo "Comando: ./ga -i tests/test_small.graph -t 10 --pop_size 150 --p_cruce 0.85 --p_mut 0.02 --k_greedy 15 --seeding_rate 0.3"
echo "Descripción: Todos los parámetros configurados manualmente"
echo ""
./ga -i tests/test_small.graph -t 10 --pop_size 150 --p_cruce 0.85 --p_mut 0.02 --k_greedy 15 --seeding_rate 0.3
echo ""
read -p "Presiona Enter para continuar..."
echo ""

if [ -f "../dataset_grafos_no_dirigidos/new_1000_dataset/erdos_n1000_p0c0.05_1.graph" ]; then
    echo "=== EJEMPLO 6: Grafo Grande del Dataset ==="
    echo "Comando: ./ga -i ../dataset_grafos_no_dirigidos/new_1000_dataset/erdos_n1000_p0c0.05_1.graph -t 10 --pop_size 100"
    echo "Descripción: Grafo de 1000 nodos durante 10 segundos"
    echo ""
    ./ga -i ../dataset_grafos_no_dirigidos/new_1000_dataset/erdos_n1000_p0c0.05_1.graph -t 10 --pop_size 100
    echo ""
fi

echo ""
echo "======================================================"
echo "  EJEMPLOS COMPLETADOS"
echo "======================================================"
echo ""
echo "TIP: Puedes combinar cualquier parámetro según tus necesidades."
echo "Consulta el README.md para más información sobre los parámetros."
