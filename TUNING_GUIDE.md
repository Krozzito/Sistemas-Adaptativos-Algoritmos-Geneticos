# 🎯 Guía de Tuning de Parámetros - Algoritmo Genético MISP

## 📊 Rangos Recomendados para Tuning

Esta guía proporciona rangos científicamente fundamentados para el tuning automático de parámetros (e.g., irace, SMAC, Optuna).

---

## ⚠️ IMPORTANTE: Uso del Parámetro --seed en Tuning

### Durante el Tuning Automático (irace/SMAC/Optuna)

**❌ NO usar `--seed` fija:**
- El tuning necesita variabilidad entre ejecuciones
- Una seed fija puede llevar a sobreajuste a un escenario específico
- Dejar que el algoritmo use seed aleatoria (comportamiento por defecto)

**Ejemplo CORRECTO para tuning:**
```bash
./ga -i instance.graph -t 30 --pop_size 100 --p_cruce 0.9 --p_mut 0.01
# Sin --seed → usa seed aleatoria cada vez ✅
```

### Durante la Validación Final

**✅ SÍ usar múltiples `--seed` fijas:**
- Permite reproducir exactamente los resultados
- Genera estadísticas robustas (media, desv. estándar)
- Facilita la comparación entre configuraciones

**Ejemplo CORRECTO para validación:**
```bash
for seed in 1 2 3 4 5 10 20 30 40 50; do
    ./ga -i instance.graph -t 60 --pop_size 120 --seed $seed
done
# Múltiples seeds fijas → resultados reproducibles ✅
```

---

## 🔧 Parámetros y sus Rangos

### 1. **pop_size** - Tamaño de la Población

**Descripción:** Número de individuos en la población.

| Aspecto | Valor |
|---------|-------|
| **Rango recomendado** | [30, 300] |
| **Tipo** | Entero (integer) |
| **Escala** | Logarítmica o lineal |
| **Default** | 100 |

#### Rangos por categoría:

```
Exploración básica:     [30, 100, 200, 300]
Tuning fino:            [50, 75, 100, 125, 150, 200, 250]
Tuning exhaustivo:      [30, 40, 50, 60, 80, 100, 120, 150, 180, 200, 250, 300]
```

#### Consideraciones:
- **Valores bajos (30-60):** Convergen rápido, pueden quedar en óptimos locales
- **Valores medios (80-150):** Balance entre exploración y velocidad
- **Valores altos (200-300):** Mayor diversidad, más lento
- ⚠️ Para grafos >2000 nodos, considerar [50, 150]

---

### 2. **p_cruce** - Probabilidad de Cruce

**Descripción:** Probabilidad de aplicar el operador de cruce entre dos padres.

| Aspecto | Valor |
|---------|-------|
| **Rango recomendado** | [0.6, 1.0] |
| **Tipo** | Real (float/double) |
| **Escala** | Lineal |
| **Default** | 0.9 |

#### Rangos por categoría:

```
Exploración básica:     [0.7, 0.8, 0.9, 1.0]
Tuning fino:            [0.65, 0.75, 0.85, 0.90, 0.95, 1.0]
Tuning exhaustivo:      [0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95, 1.0]
```

#### Consideraciones:
- **Rango típico en literatura:** [0.7, 0.95]
- **Valores bajos (0.6-0.7):** Menos mezcla genética
- **Valores altos (0.9-1.0):** Máxima recombinación (recomendado)
- ⚠️ Valores <0.6 raramente son óptimos

---

### 3. **p_mut** - Probabilidad de Mutación

**Descripción:** Probabilidad de mutar cada bit del cromosoma.

| Aspecto | Valor |
|---------|-------|
| **Rango recomendado** | [0.001, 0.1] |
| **Tipo** | Real (float/double) |
| **Escala** | **LOGARÍTMICA** (importante) |
| **Default** | 0.01 |

#### Rangos por categoría:

```
Exploración básica:     [0.001, 0.005, 0.01, 0.05, 0.1]
Tuning fino:            [0.001, 0.002, 0.005, 0.01, 0.02, 0.05, 0.08]
Tuning exhaustivo:      [0.0005, 0.001, 0.002, 0.005, 0.008, 0.01, 0.015, 
                         0.02, 0.03, 0.05, 0.07, 0.1]
```

#### Consideraciones:
- **Regla heurística:** `p_mut ≈ 1/n` donde n = número de nodos
  - n=100  → p_mut ≈ 0.01
  - n=1000 → p_mut ≈ 0.001
  - n=3000 → p_mut ≈ 0.0003

- **Valores bajos (0.001-0.005):** Explotación, refinamiento
- **Valores medios (0.01-0.02):** Balance (recomendado para n=1000)
- **Valores altos (0.05-0.1):** Exploración intensa
- ⚠️ Usar escala logarítmica en herramientas de tuning

---

### 4. **k_greedy** - Parámetro de Aleatoriedad Greedy

**Descripción:** Número de candidatos considerados en cada paso de la heurística greedy (seeding).

| Aspecto | Valor |
|---------|-------|
| **Rango recomendado** | [1, 50] |
| **Tipo** | Entero (integer) |
| **Escala** | Logarítmica o lineal |
| **Default** | 10 |

#### Rangos por categoría:

```
Exploración básica:     [1, 5, 10, 20, 50]
Tuning fino:            [1, 3, 5, 8, 10, 15, 20, 30, 40]
Tuning exhaustivo:      [1, 2, 3, 5, 7, 10, 12, 15, 20, 25, 30, 40, 50]
```

#### Consideraciones:
- **k=1:** Greedy puro (determinista, alta calidad)
- **k=5-15:** Semi-aleatorio (balance)
- **k>30:** Muy aleatorio (alta diversidad)
- ⚠️ Interactúa fuertemente con `seeding_rate`

---

### 5. **seeding_rate** - Proporción de Seeding

**Descripción:** Fracción de la población inicial generada con heurística greedy.

| Aspecto | Valor |
|---------|-------|
| **Rango recomendado** | [0.0, 0.5] |
| **Tipo** | Real (float/double) |
| **Escala** | Lineal |
| **Default** | 0.2 |

#### Rangos por categoría:

```
Exploración básica:     [0.0, 0.1, 0.2, 0.3, 0.5]
Tuning fino:            [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.4, 0.5]
Tuning exhaustivo:      [0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 
                         0.4, 0.45, 0.5]
```

#### Consideraciones:
- **0.0:** Sin seeding (100% aleatorio)
- **0.1-0.3:** Balance típico (recomendado)
- **0.5:** Mitad greedy, mitad aleatorio
- ⚠️ Valores >0.5 pueden reducir diversidad excesivamente
- ⚠️ Valores <0.1 pueden hacer convergencia muy lenta

---

## 🎲 Configuraciones Predefinidas

### Configuración 1: **Exploración Intensa**
```bash
--pop_size 200 --p_cruce 0.85 --p_mut 0.05 --k_greedy 30 --seeding_rate 0.1
```
**Uso:** Instancias difíciles, cuando se queda atrapado en óptimos locales

---

### Configuración 2: **Balance Estándar**
```bash
--pop_size 100 --p_cruce 0.9 --p_mut 0.01 --k_greedy 10 --seeding_rate 0.2
```
**Uso:** Configuración por defecto, funciona bien en la mayoría de casos

---

### Configuración 3: **Explotación Rápida**
```bash
--pop_size 50 --p_cruce 0.95 --p_mut 0.002 --k_greedy 5 --seeding_rate 0.4
```
**Uso:** Cuando se necesita una buena solución rápidamente

---

### Configuración 4: **Población Grande**
```bash
--pop_size 300 --p_cruce 0.9 --p_mut 0.008 --k_greedy 15 --seeding_rate 0.15
```
**Uso:** Instancias muy grandes (>2000 nodos), con mucho tiempo disponible

---

## 📋 Formato para irace

### Archivo `parameters.txt`:

```
pop_size         "--pop_size "      i    (30, 300)
p_cruce          "--p_cruce "       r    (0.6, 1.0)
p_mut            "--p_mut "         r    (0.001, 0.1)
k_greedy         "--k_greedy "      i    (1, 50)
seeding_rate     "--seeding_rate "  r    (0.0, 0.5)
```

**⚠️ IMPORTANTE: NO incluir `--seed` en el tuning automático**
- El tuning necesita variabilidad entre ejecuciones
- Dejar que el algoritmo use seed aleatoria (default)
- Solo usar `--seed` fija en fase de validación final

### Archivo `scenario.txt`:

```
parameterFile = "parameters.txt"
execDir = "."
trainInstancesDir = "../dataset_grafos_no_dirigidos/new_1000_dataset"
trainInstancesFile = "instances.txt"
targetRunner = "./target-runner.sh"
maxExperiments = 1000
logFile = "irace.log"
```

### Script `target-runner.sh`:

```bash
#!/bin/bash
INSTANCE=$1
shift
PARAMS=$@

# Ejecutar algoritmo SIN seed fija (usa seed aleatoria por defecto)
# Esto es CORRECTO para tuning - queremos variabilidad
OUTPUT=$(./ga -i "$INSTANCE" -t 30 $PARAMS 2>&1 | tail -1)

# Extraer calidad (negativo porque irace minimiza)
QUALITY=$(echo "$OUTPUT" | grep -oP 'Final: \K\d+')
echo "-$QUALITY"
```

---

## 📋 Formato para SMAC

### Archivo `pcs` (Parameter Configuration Space):

```
pop_size [30, 300] [100]i
p_cruce [0.6, 1.0] [0.9]
p_mut [0.001, 0.1] [0.01] log
k_greedy [1, 50] [10]i
seeding_rate [0.0, 0.5] [0.2]
```

**⚠️ IMPORTANTE: NO incluir `--seed` en el tuning automático**

---

## 📋 Formato para Optuna (Python)

```python
import optuna

def objective(trial):
    pop_size = trial.suggest_int('pop_size', 30, 300)
    p_cruce = trial.suggest_float('p_cruce', 0.6, 1.0)
    p_mut = trial.suggest_float('p_mut', 0.001, 0.1, log=True)
    k_greedy = trial.suggest_int('k_greedy', 1, 50)
    seeding_rate = trial.suggest_float('seeding_rate', 0.0, 0.5)
    
    # Ejecutar algoritmo SIN --seed (usa seed aleatoria por defecto)
    # IMPORTANTE: No incluir --seed aquí para tuning correcto
    cmd = f"./ga -i instance.graph -t 30 --pop_size {pop_size} " \
          f"--p_cruce {p_cruce} --p_mut {p_mut} " \
          f"--k_greedy {k_greedy} --seeding_rate {seeding_rate}"
    
    # ... (ejecutar y parsear resultado)
    
    return -quality  # Negativo porque Optuna minimiza

study = optuna.create_study()
study.optimize(objective, n_trials=100)
```

---

## 🔬 Recomendaciones de Tuning

### 1. **Estrategia de Búsqueda**

#### Fase 1: Exploración Gruesa (Grid Search)
- Usar configuraciones predefinidas
- Probar extremos de cada parámetro
- Identificar rangos prometedores

#### Fase 2: Tuning Automático
- Usar irace/SMAC/Optuna
- 500-1000 evaluaciones
- Múltiples instancias representativas

#### Fase 3: Refinamiento
- Ajustar rangos según resultados
- Tuning fino en rango reducido
- Validación cruzada

---

### 2. **Consideraciones por Tamaño de Instancia**

#### Instancias Pequeñas (n < 500)
```
pop_size:       [30, 150]
p_mut:          [0.01, 0.05]
seeding_rate:   [0.2, 0.4]
```

#### Instancias Medianas (500 ≤ n ≤ 2000)
```
pop_size:       [50, 200]
p_mut:          [0.002, 0.02]
seeding_rate:   [0.1, 0.3]
```

#### Instancias Grandes (n > 2000)
```
pop_size:       [80, 300]
p_mut:          [0.0003, 0.005]
seeding_rate:   [0.05, 0.2]
```

---

### 3. **Interacciones entre Parámetros**

#### ⚠️ Importante: Algunos parámetros interactúan

**Interacción 1:** `seeding_rate` ↔ `k_greedy`
- Alto seeding + k bajo = Convergencia rápida, poca diversidad
- Bajo seeding + k alto = Diversidad alta, convergencia lenta

**Interacción 2:** `p_mut` ↔ `pop_size`
- Población grande + mutación baja = Explotación
- Población pequeña + mutación alta = Exploración

**Interacción 3:** `p_cruce` ↔ `p_mut`
- Alto cruce + baja mutación = Refinamiento de soluciones
- Bajo cruce + alta mutación = Búsqueda aleatoria

---

### 4. **Métricas de Evaluación**

Para tuning efectivo, considera múltiples métricas:

1. **Calidad final:** Valor del fitness al terminar
2. **Tiempo de primera solución:** Rapidez inicial
3. **Mejora promedio:** (Final - Inicial) / Inicial
4. **Consistencia:** Desviación estándar en múltiples runs

---

### 5. **Budget de Evaluaciones**

| Budget | Estrategia |
|--------|------------|
| < 100 evals | Grid search con configuraciones predefinidas |
| 100-500 evals | Random search o Latin Hypercube Sampling |
| 500-1000 evals | irace o Sequential Model-Based Optimization |
| > 1000 evals | Tuning exhaustivo + validación cruzada |

---

## 🎯 Guía Rápida de Decisión

```
¿Cuánto tiempo tienes?
├─ Poco (< 1 hora)
│  └─> Usa configuración 2 (Balance Estándar)
│
├─ Moderado (1-4 horas)
│  └─> Grid search con 4-5 configuraciones predefinidas
│
└─ Mucho (> 4 horas)
   └─> Tuning automático (irace/SMAC)
      ├─ Budget bajo (< 500 evals): Random search
      └─ Budget alto (≥ 500 evals): irace o Bayesian Optimization
```

---

## 📊 Ejemplo de Experimento Completo

```bash
# 1. Crear directorio de experimentos
mkdir tuning_experiments
cd tuning_experiments

# 2. Probar configuraciones predefinidas
for config in 1 2 3 4; do
    echo "Testing config $config..."
    # ... ejecutar con cada configuración
done

# 3. Tuning automático con irace
# (requiere instalación de irace en R)
# IMPORTANTE: No usar --seed aquí - queremos variabilidad
Rscript -e 'library(irace); irace()'

# 4. Validar mejor configuración con múltiples seeds
BEST_CONFIG="--pop_size 120 --p_cruce 0.88 --p_mut 0.015 --k_greedy 12 --seeding_rate 0.25"

# Validación con múltiples seeds fijas para estadísticas reproducibles
for seed in 1 2 3 4 5 10 20 30 40 50; do
    for instance in ../dataset/*.graph; do
        ./ga -i "$instance" -t 60 $BEST_CONFIG --seed $seed >> validation_results.txt
    done
done
```

---

## ✅ Checklist de Tuning

- [ ] Definir conjunto de instancias representativas
- [ ] Fijar tiempo de ejecución por evaluación
- [ ] Elegir estrategia de búsqueda
- [ ] Configurar herramienta de tuning (SIN --seed)
- [ ] Ejecutar tuning (500-1000 evals recomendado)
- [ ] Analizar resultados y convergencia
- [ ] Validar mejor configuración en instancias no vistas (CON múltiples --seed)
- [ ] Documentar configuración final y seeds usadas

---

## 📚 Referencias y Notas

**Tiempo de ejecución recomendado por evaluación:**
- Training: 10-30 segundos (para que irace/SMAC pueda explorar)
- Testing final: 60-300 segundos (para resultados robustos)

**Número de runs por configuración:**
- Tuning: 1 run por evaluación (para explorar más configuraciones)
- Validación: 5-10 runs con diferentes seeds fijas (para estadísticas robustas)

**Uso del parámetro --seed:**
- ❌ **NO** usar en fase de tuning (irace/SMAC/Optuna)
- ✅ **SÍ** usar en fase de validación con múltiples seeds diferentes
- ✅ Documentar seeds usadas: ejemplo `{1, 2, 3, 4, 5, 10, 20, 30, 40, 50}`
- ✅ Reportar estadísticas: media ± desviación estándar sobre las N seeds

---

**¡Buena suerte con el tuning! 🚀**
