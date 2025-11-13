#!/bin/bash

# Script de pruebas para el Algoritmo Genético - MISP
# Prueba diferentes configuraciones y grafos

echo "========================================="
echo "  PRUEBAS DEL ALGORITMO GENÉTICO - MISP"
echo "========================================="
echo ""

# Colores para output
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Función para ejecutar un test
run_test() {
    local test_name=$1
    local graph_file=$2
    local expected_min=$3
    local time_limit=$4
    local extra_params=$5
    
    echo -e "${BLUE}Test: ${test_name}${NC}"
    echo "Grafo: $graph_file"
    echo "Tiempo límite: ${time_limit}s"
    
    # El programa ahora solo imprime un número
    quality=$($GA -i "$graph_file" -t "$time_limit" $extra_params 2>&1)
    
    # Validar que sea un número
    if ! [[ "$quality" =~ ^[0-9]+$ ]]; then
        echo -e "${RED}✗ ERROR: No se pudo ejecutar el test o salida inválida: $quality${NC}"
        return 1
    fi
    
    if [ "$quality" -ge "$expected_min" ]; then
        echo -e "${GREEN}✓ PASS${NC} - Calidad obtenida: $quality (mínimo esperado: $expected_min)"
    else
        echo -e "${RED}✗ FAIL${NC} - Calidad obtenida: $quality (mínimo esperado: $expected_min)"
    fi
    
    echo ""
}

# Verificar que el ejecutable existe
if [ ! -f "../ga" ]; then
    echo -e "${RED}Error: El ejecutable '../ga' no existe.${NC}"
    echo "Por favor, compila primero con: make"
    exit 1
fi

# Definir el ejecutable
GA="../ga"

echo "=== PRUEBAS BÁSICAS CON GRAFOS PEQUEÑOS ==="
echo ""

# Test 1: Triángulo (grafo completo K3)
run_test "Triángulo (K3)" "test_triangle.graph" 1 1

# Test 2: Grafo Estrella
run_test "Grafo Estrella" "test_star.graph" 5 1

# Test 3: Ciclo de 5 nodos
run_test "Ciclo C5" "test_small.graph" 2 1

echo ""
echo "=== PRUEBAS CON DIFERENTES PARÁMETROS ==="
echo ""

# Test 4: Población pequeña
run_test "Población pequeña" "test_small.graph" 2 2 "--pop_size 10"

# Test 5: Población grande
run_test "Población grande" "test_small.graph" 2 2 "--pop_size 200"

# Test 6: Alta probabilidad de mutación
run_test "Alta mutación" "test_small.graph" 2 2 "--p_mut 0.1"

# Test 7: Baja probabilidad de mutación
run_test "Baja mutación" "test_small.graph" 2 2 "--p_mut 0.001"

# Test 8: Sin seeding (totalmente aleatorio)
run_test "Sin seeding" "test_small.graph" 2 2 "--seeding_rate 0.0"

# Test 9: Seeding completo
run_test "Seeding al 100%" "test_small.graph" 2 2 "--seeding_rate 1.0"

echo ""
echo "=== PRUEBAS CON GRAFOS GRANDES (DATASET) ==="
echo ""

# Verificar si existe el dataset
if [ -d "../dataset_grafos_no_dirigidos/new_1000_dataset" ]; then
    # Test 10: Grafo Erdős-Rényi mediano
    run_test "Erdős-Rényi n=1000 p=0.05" \
        "../dataset_grafos_no_dirigidos/new_1000_dataset/erdos_n1000_p0c0.05_1.graph" \
        90 3 "--pop_size 50"
    
    # Test 11: Mismo grafo con más tiempo
    run_test "Erdős-Rényi n=1000 p=0.05 (más tiempo)" \
        "../dataset_grafos_no_dirigidos/new_1000_dataset/erdos_n1000_p0c0.05_1.graph" \
        95 10 "--pop_size 100"
else
    echo -e "${BLUE}Info: Dataset de grafos grandes no encontrado. Saltando pruebas de grafos grandes.${NC}"
fi

echo ""
echo "=== EJEMPLO DE EJECUCIÓN (5 segundos) ==="
echo ""
echo -e "${BLUE}Ejecutando algoritmo...${NC}"
resultado=$($GA -i test_star.graph -t 5 --pop_size 50)
echo "Resultado: $resultado"

echo ""
echo "========================================="
echo "  PRUEBAS COMPLETADAS"
echo "========================================="
