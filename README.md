# Algoritmo Genético para Maximum Independent Set Problem (MISP)

## 📋 Descripción

Implementación de un **Algoritmo Genético (GA)** para resolver el problema del **Maximum Independent Set** en grafos no dirigidos. El problema consiste en encontrar el conjunto más grande de vértices tal que ningún par de vértices en el conjunto esté conectado por una arista.

### Características principales:
- ✅ Representación binaria (cromosoma de bits)
- ✅ Inicialización con **seeding** (mezcla de soluciones greedy y aleatorias)
- ✅ Selección por torneo (k=2)
- ✅ Cruce uniforme
- ✅ Mutación bit-flip
- ✅ Operador de reparación automática
- ✅ Reporte **any-time** (muestra mejoras incrementales)
- ✅ Parámetros configurables vía línea de comandos
- ✅ **Seed configurable** para reproducibilidad (debugging/validación)

---

## 🧬 Funcionamiento del Algoritmo

### 1. Representación del Individuo

Cada individuo es un **cromosoma binario** (vector de booleanos):
```
cromosoma[i] = true  → el nodo i ESTÁ en el conjunto independiente
cromosoma[i] = false → el nodo i NO ESTÁ en el conjunto independiente
```

**Ejemplo:**
Para un grafo de 5 nodos, el cromosoma `[1, 0, 1, 0, 1]` significa que los nodos {0, 2, 4} están en la solución.

### 2. Población Inicial (Seeding)

La población inicial se crea combinando dos estrategias:

#### a) **Individuos Greedy** (seeding_rate × pop_size)
- Utiliza una heurística greedy que selecciona nodos de forma inteligente
- Parámetro `k_greedy`: controla la aleatoriedad (valores bajos = más diversidad)
- Genera soluciones de alta calidad desde el inicio

#### b) **Individuos Aleatorios** ((1 - seeding_rate) × pop_size)
- Cromosomas generados completamente al azar
- Proporcionan diversidad genética
- Se reparan automáticamente tras ser creados

**Ventaja del seeding:** Acelera la convergencia sin perder diversidad.

### 3. Ciclo Evolutivo

El algoritmo ejecuta generaciones hasta alcanzar el tiempo límite:

```
Mientras (tiempo < max_time):
    Para cada par de padres:
        1. Selección por Torneo (k=2)
        2. Cruce Uniforme (probabilidad p_cruce)
        3. Mutación Bit-Flip (probabilidad p_mut)
        4. Reparación y Evaluación
    
    Reemplazar población anterior con nueva generación
    
    Si hay mejora:
        Reportar nueva mejor solución (any-time)
```

#### **Selección por Torneo**
- Escoge 2 individuos al azar
- Selecciona el de mejor fitness
- Permite que soluciones buenas (pero no óptimas) sobrevivan

#### **Cruce Uniforme**
- Para cada posición del cromosoma, decide aleatoriamente (50%) de qué padre heredar el bit
- Genera dos hijos intercambiando genes entre padres
- Solo se aplica con probabilidad `p_cruce`

#### **Mutación Bit-Flip**
- Cada bit del cromosoma se invierte con probabilidad `p_mut`
- Introduce variabilidad para evitar convergencia prematura

#### **Reparación**
El operador de reparación garantiza que la solución sea válida (conjunto independiente):

```cpp
Para cada nodo u en el cromosoma (en orden 0, 1, 2, ...):
    Si cromosoma[u] = true:
        Si u tiene algún vecino v < u donde cromosoma[v] = true:
            Conflicto detectado → cromosoma[u] = false (reparar)
        Sino:
            Mantener u en la solución
```

**Propiedad importante:** Al procesar nodos en orden, garantizamos que nunca hay conflictos.

### 4. Reporte Any-Time

Cada vez que se encuentra una solución mejor, se imprime:
```
Calidad: 105, Tiempo: 21.1136s
```

Al finalizar el tiempo, se reporta la mejor solución encontrada:
```
Final: 105, Tiempo: 21.1136s
```

---

## 🔧 Compilación

### Requisitos
- **Compilador:** g++ con soporte C++17 o superior
- **Sistema operativo:** Linux, macOS, Windows (con MinGW/WSL)

### Comando de compilación

```bash
g++ -std=c++17 -O3 -o ga ga.cpp
```

**Opciones:**
- `-std=c++17`: Habilita características de C++17
- `-O3`: Optimización máxima del código
- `-o ga`: Nombre del ejecutable de salida

### Verificar compilación

```bash
./ga --help
# Si no hay mensaje de error al ejecutar sin parámetros, compiló correctamente
```

---

## 🚀 Uso

### Sintaxis básica

```bash
./ga -i <archivo_grafo> -t <tiempo_segundos> [opciones]
```

### Parámetros obligatorios

| Parámetro | Descripción | Ejemplo |
|-----------|-------------|---------|
| `-i <archivo>` | Archivo del grafo (formato .graph) | `-i grafo.graph` |
| `-t <segundos>` | Tiempo máximo de ejecución | `-t 10` |

### Parámetros opcionales (Algoritmo Genético)

| Parámetro | Descripción | Rango | Valor por defecto |
|-----------|-------------|-------|-------------------|
| `--pop_size <N>` | Tamaño de la población | 10-500 | 100 |
| `--p_cruce <P>` | Probabilidad de cruce | 0.0-1.0 | 0.9 |
| `--p_mut <P>` | Probabilidad de mutación | 0.0-1.0 | 0.01 |
| `--k_greedy <K>` | Parámetro de aleatoriedad greedy | 1-50 | 10 |
| `--seeding_rate <R>` | Proporción de individuos greedy | 0.0-1.0 | 0.2 |
| `--seed <S>` | Seed aleatoria (reproducibilidad) | -1 = aleatorio, ≥0 = fija | -1 |

### Ejemplos de uso

#### 1. Ejecución básica (10 segundos)
```bash
./ga -i dataset/erdos_n1000_p0c0.05_1.graph -t 10
```

#### 2. Población grande y mayor tiempo
```bash
./ga -i dataset/grafo_grande.graph -t 30 --pop_size 200
```

#### 3. Sin seeding (100% aleatorio)
```bash
./ga -i grafo.graph -t 10 --seeding_rate 0.0
```

#### 4. Exploración intensiva (alta mutación)
```bash
./ga -i grafo.graph -t 10 --p_mut 0.05
```

#### 5. Explotación intensiva (seeding completo)
```bash
./ga -i grafo.graph -t 10 --seeding_rate 1.0 --p_mut 0.001
```

#### 6. Configuración balanceada personalizada
```bash
./ga -i grafo.graph -t 20 \
    --pop_size 150 \
    --p_cruce 0.85 \
    --p_mut 0.02 \
    --k_greedy 15 \
    --seeding_rate 0.3
```

#### 7. Con seed fija para debugging/reproducibilidad
```bash
# Siempre produce los mismos resultados
./ga -i grafo.graph -t 10 --seed 42
```

#### 8. Validación estadística (múltiples seeds)
```bash
# Para obtener estadísticas robustas
for seed in 1 2 3 4 5; do
    ./ga -i grafo.graph -t 60 --seed $seed
done
```

---

## 📁 Formato del archivo de entrada

El algoritmo espera grafos en el siguiente formato:

```
N
u1 v1
u2 v2
u3 v3
...
```

Donde:
- **N**: Número de nodos (primera línea)
- **ui vi**: Arista entre los nodos `ui` y `vi` (una por línea)

### Ejemplo: Triángulo (K3)
```
3
0 1
1 2
2 0
```

### Ejemplo: Grafo estrella (6 nodos)
```
6
0 1
0 2
0 3
0 4
0 5
```

**Notas:**
- Los nodos se numeran desde 0 hasta N-1
- Las aristas se consideran no dirigidas
- Aristas duplicadas se ignoran automáticamente
- Self-loops (u == v) se ignoran

---

## 🧪 Testing

### Ejecutar suite de pruebas completa

```bash
cd tests
./run_tests.sh
```

Este script ejecuta:
1. ✅ **Pruebas básicas** con grafos pequeños
2. ✅ **Pruebas de parámetros** (variando configuraciones)
3. ✅ **Pruebas con dataset** (grafos grandes si está disponible)
4. ✅ **Prueba any-time** (observando mejoras incrementales)

### Ejecutar pruebas individuales

```bash
# Triángulo (resultado esperado: 1)
./ga -i tests/test_triangle.graph -t 1

# Grafo estrella (resultado esperado: 5)
./ga -i tests/test_star.graph -t 1

# Ciclo de 5 nodos (resultado esperado: 2)
./ga -i tests/test_small.graph -t 1
```

### Verificación de resultados

| Grafo | Nodos | Aristas | IS Máximo | Descripción |
|-------|-------|---------|-----------|-------------|
| `test_triangle.graph` | 3 | 3 | **1** | Grafo completo K3 |
| `test_star.graph` | 6 | 5 | **5** | Estrella (todos menos el centro) |
| `test_small.graph` | 5 | 5 | **2** | Ciclo C5 |

---

## 📊 Interpretación de la salida

### Ejemplo de salida

```
Calidad: 103, Tiempo: 0.0150s
Calidad: 107, Tiempo: 1.2340s
Calidad: 110, Tiempo: 3.5678s
Calidad: 112, Tiempo: 5.8901s
Final: 112, Tiempo: 5.8901s
```

**Interpretación:**
- Línea 1: Solución inicial encontrada (103 nodos, 0.015s)
- Líneas 2-4: Mejoras incrementales encontradas durante la ejecución
- Línea 5: Mejor solución final (112 nodos, encontrada a los 5.89s)

**Formato any-time:**
- Permite detener el algoritmo en cualquier momento y obtener la mejor solución hasta ese instante
- Útil para comparar algoritmos o ajustar tiempo según recursos disponibles

---

## 🎯 Recomendaciones de parámetros

### Para grafos pequeños (<100 nodos)
```bash
--pop_size 50 --p_cruce 0.9 --p_mut 0.02 --seeding_rate 0.3
```

### Para grafos medianos (100-1000 nodos)
```bash
--pop_size 100 --p_cruce 0.9 --p_mut 0.01 --seeding_rate 0.2
```

### Para grafos grandes (>1000 nodos)
```bash
--pop_size 200 --p_cruce 0.85 --p_mut 0.005 --seeding_rate 0.15
```

### Exploración vs Explotación

| Objetivo | `p_mut` | `seeding_rate` | `k_greedy` |
|----------|---------|----------------|------------|
| **Explorar** (diversidad) | ↑ (0.05) | ↓ (0.1) | ↑ (20) |
| **Explotar** (intensificar) | ↓ (0.001) | ↑ (0.5) | ↓ (5) |
| **Balanceado** | 0.01 | 0.2 | 10 |

---

## 🐛 Solución de problemas

### Error: "No se pudo abrir el archivo"
**Causa:** Ruta incorrecta al archivo del grafo.
```bash
# Verifica que el archivo existe
ls -l <ruta_del_grafo>

# Usa ruta absoluta si es necesario
./ga -i /ruta/completa/al/grafo.graph -t 10
```

### El algoritmo no mejora después de cierto punto
**Solución:** Aumenta la mutación o reduce el seeding:
```bash
./ga -i grafo.graph -t 20 --p_mut 0.03 --seeding_rate 0.1
```

### Resultados inconsistentes entre ejecuciones
**Causa:** El algoritmo es estocástico (usa aleatoriedad).
**Solución:** Ejecuta múltiples veces y toma el promedio o el mejor resultado.

### Compilación falla
**Verifica versión de g++:**
```bash
g++ --version  # Debe ser >= 7.0
```

**Si usas un compilador más antiguo:**
```bash
g++ -std=c++11 -O3 -o ga ga.cpp  # Intenta con C++11
```

---

## 📚 Estructura del código

```
ga.cpp
├── struct Individuo              # Representación del cromosoma
├── generarSolucionGreedy()       # Heurística greedy para seeding
├── crearIndividuoGreedy()        # Convierte solución greedy a individuo
├── inicializarPoblacion()        # Crea población inicial (seeding + aleatorio)
├── reparar_y_evaluar()           # Valida y calcula fitness
├── seleccionPorTorneo()          # Selección de padres
├── cruceUniforme()               # Operador de cruce
├── mutacionBitFlip()             # Operador de mutación
├── obtenerMejor()                # Encuentra mejor individuo
├── geneticAlgorithm()            # Bucle principal del GA
└── main()                        # Parser de argumentos
```

---

## ✅ Validación de la implementación

### Verificaciones realizadas:

1. ✅ **Compilación exitosa** sin warnings
2. ✅ **Grafos pequeños:** Resultados correctos
3. ✅ **Grafos grandes:** Encuentra soluciones de calidad aceptable
4. ✅ **Any-time:** Reporta mejoras incrementales correctamente
5. ✅ **Parámetros:** Todos los parámetros funcionan según lo esperado
6. ✅ **Reparación:** Garantiza que todas las soluciones son conjuntos independientes válidos
7. ✅ **Seeding:** Mejora significativa en convergencia

### Resultados de pruebas:

| Grafo | IS Máximo esperado | IS encontrado | Estado |
|-------|-------------------|---------------|--------|
| Triángulo | 1 | 1 | ✅ |
| Estrella | 5 | 5 | ✅ |
| Ciclo C5 | 2 | 2 | ✅ |
| Erdős-Rényi (n=1000, p=0.05) | ~100-110 | 103-112 | ✅ |

---

## 📝 Notas adicionales

### Diferencias con otros enfoques:
- **vs Greedy puro:** El GA puede escapar de óptimos locales
- **vs Simulated Annealing:** Mantiene población (no solo una solución)
- **vs Búsqueda Local:** Mayor diversidad de exploración

### Complejidad computacional:
- **Por generación:** O(pop_size × n²) donde n = número de nodos
- **Reparación:** O(n × m) donde m = número de aristas

### Posibles mejoras futuras:
- [ ] Elitismo (preservar mejores individuos)
- [ ] Cruce de múltiples puntos
- [ ] Mutación adaptativa (varía `p_mut` según generación)
- [ ] Paralelización del fitness
- [ ] Estrategias de reinicio (restart)

---

## 👥 Autores

Implementación desarrollada como parte del curso de **Sistemas Adaptativos**.

---

## 📄 Licencia

Este código es de uso académico. Consulta con tu institución sobre políticas de uso y distribución.

---

## 🆘 Soporte

Si encuentras algún problema o tienes dudas:
1. Revisa la sección de **Solución de problemas**
2. Ejecuta el script de tests: `./tests/run_tests.sh`
3. Verifica que los parámetros estén en rangos válidos

---

**¡Happy Hacking! 🚀**
