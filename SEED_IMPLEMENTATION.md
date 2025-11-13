# Implementación del Parámetro Seed - Resumen Técnico

## ✅ Cambios Realizados

Se ha implementado el parámetro `--seed` en **ambas versiones** del código (monolítico y modular) para permitir la reproducibilidad de experimentos.

### Archivos Modificados

#### Versión Monolítica (`ga.cpp`)
- ✅ Función `geneticAlgorithm()`: Añadido parámetro `int seed = -1`
- ✅ Inicialización del generador: Lógica condicional basada en el valor de seed
- ✅ Función `main()`: Parseo del argumento `--seed`

#### Versión Modular (`src/`)
- ✅ `src/AlgoritmoGenetico.h`: Añadido campo `int seed` a `ParametrosGA`
- ✅ `src/AlgoritmoGenetico.cpp`: Lógica de inicialización condicional del generador
- ✅ `src/main.cpp`: Parseo del argumento `--seed` y actualización del help

## 🔧 Implementación Técnica

### Lógica de Seed

```cpp
// Configuración del generador aleatorio
mt19937 gen;
if (seed >= 0) {
    gen.seed(seed);  // Seed fija → reproducible
} else {
    gen.seed(random_device{}());  // Seed aleatoria → no reproducible
}
```

### Valores del Parámetro

| Valor | Comportamiento |
|-------|----------------|
| `-1` (default) | Usa `random_device{}()` - cada ejecución es diferente |
| `≥ 0` | Usa el valor especificado - resultados reproducibles |

## ✅ Pruebas Realizadas

### Test 1: Reproducibilidad con Seed Fija
```bash
./ga -i tests/test_triangle.graph -t 1 --seed 123
./ga -i tests/test_triangle.graph -t 1 --seed 123
# ✅ Ambas ejecuciones producen el mismo resultado
```

### Test 2: Comportamiento Aleatorio (Sin Seed)
```bash
./ga -i tests/test_star.graph -t 2
./ga -i tests/test_star.graph -t 2
# ✅ Los resultados pueden variar entre ejecuciones
```

### Test 3: Verificación en Grafo Complejo
```bash
./ga -i tests/test_star.graph -t 2 --seed 42
./ga -i tests/test_star.graph -t 2 --seed 42
# ✅ Calidad: 5, Tiempo: 0.0001s (idénticos)
```

## 📚 Documentación Creada

1. **SEED_GUIDE.md**: Guía completa de uso del parámetro seed
   - Casos de uso (experimentación, debugging, validación)
   - Mejores prácticas
   - Scripts de ejemplo
   - Análisis estadístico

2. **README.md**: Actualizado con:
   - Nueva característica en la lista de features
   - Parámetro `--seed` en la tabla de parámetros
   - Ejemplos 7 y 8 con uso de seed

## 🎯 Casos de Uso Principales

### 1. Experimentación Normal (Default)
```bash
./ga -i grafo.graph -t 60
# Sin seed: máxima variabilidad entre ejecuciones
```

### 2. Debugging
```bash
./ga -i grafo.graph -t 60 --seed 42
# Con seed fija: reproducir exactamente el mismo comportamiento
```

### 3. Validación Científica
```bash
for seed in 1 2 3 4 5 10 20 30 40 50; do
    ./ga -i grafo.graph -t 60 --seed $seed
done
# Múltiples seeds fijas: estadísticas reproducibles
```

### 4. Comparación Justa
```bash
for seed in 1 2 3 4 5; do
    ./ga -i inst.graph -t 60 --pop_size 50 --seed $seed > config1_$seed.txt
    ./ga -i inst.graph -t 60 --pop_size 100 --seed $seed > config2_$seed.txt
done
# Mismas seeds: comparación directa entre configuraciones
```

## ⚠️ Advertencias Importantes

### ❌ NO HACER:
- **Tuning automático con seed fija:** Los configuradores (irace/SMAC) necesitan variabilidad
- **Una sola seed para validación:** Usar múltiples seeds para estadísticas robustas
- **Olvidar documentar seeds:** Siempre reportar qué seeds se usaron

### ✅ SÍ HACER:
- **Experimentos normales:** Sin seed (aleatorio)
- **Debugging:** Con seed fija específica
- **Validación paper:** Múltiples seeds fijas diferentes
- **Documentar:** Reportar siempre las seeds utilizadas

## 📊 Impacto en el Proyecto

### Ventajas
1. ✅ **Reproducibilidad:** Fundamental para debugging y validación científica
2. ✅ **Flexibilidad:** Mantiene comportamiento aleatorio por defecto
3. ✅ **Comparabilidad:** Permite comparaciones justas entre configuraciones
4. ✅ **Transparencia:** Facilita la verificación de resultados publicados

### Sin Impacto Negativo
- ✅ No afecta el rendimiento (misma lógica, solo la inicialización cambia)
- ✅ Completamente retrocompatible (seed tiene valor por defecto -1)
- ✅ No modifica la interfaz existente (parámetro opcional)

## 🔄 Compatibilidad

### Versión Anterior (Sin Seed)
```bash
./ga -i grafo.graph -t 10 --pop_size 100
# Comportamiento: aleatorio (igual que --seed -1)
```

### Versión Nueva (Con Seed)
```bash
# Comportamiento idéntico a la versión anterior
./ga -i grafo.graph -t 10 --pop_size 100

# Nueva funcionalidad: reproducibilidad
./ga -i grafo.graph -t 10 --pop_size 100 --seed 42
```

## 📝 Resumen de Compilación

### Versión Modular
```bash
make clean && make
# ✅ Compilación exitosa sin warnings
# ✅ Ejecutable: ./ga
```

### Versión Monolítica
```bash
g++ -std=c++17 -O3 -o ga ga.cpp
# ✅ Compilación exitosa
# ✅ Ejecutable: ./ga
```

## 🎓 Recomendaciones Académicas

Para un paper o reporte técnico, se recomienda:

1. **Experimentación preliminar:** Sin seed (30+ ejecuciones)
2. **Análisis estadístico:** Con múltiples seeds fijas (10-30 seeds)
3. **Reportar en el paper:**
   - Lista exacta de seeds utilizadas
   - Estadísticas: media, desviación estándar, mediana, min, max
   - Posiblemente test de significancia estadística

### Ejemplo de Reporte
```
"Se ejecutaron 30 réplicas independientes con seeds 
{1, 2, 3, ..., 30}. Los resultados reportados corresponden 
a la media ± desviación estándar sobre estas 30 ejecuciones."
```

---

**Fecha de Implementación:** 2025  
**Estado:** ✅ Completado y probado  
**Versiones afectadas:** Monolítica (ga.cpp) y Modular (src/)
