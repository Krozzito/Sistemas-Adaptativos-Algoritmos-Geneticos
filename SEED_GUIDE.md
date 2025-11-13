# Guía de Uso del Parámetro Seed

## ¿Qué es la Seed?

La **seed** es un valor numérico que inicializa el generador de números pseudo-aleatorios. Con la misma seed, el algoritmo producirá exactamente los mismos resultados en ejecuciones diferentes.

## Opciones Disponibles

### Comportamiento Aleatorio (Default)
```bash
# Sin especificar seed (usa seed aleatoria cada vez)
./ga -i grafo.graph -t 10
```
- **Uso:** Ejecuciones normales, experimentación, tuning automático
- **Comportamiento:** Cada ejecución produce resultados diferentes
- **Ventajas:** Explora mejor el espacio de búsqueda

### Comportamiento Reproducible
```bash
# Con seed fija (reproducible)
./ga -i grafo.graph -t 10 --seed 42
```
- **Uso:** Debugging, validación, análisis detallado
- **Comportamiento:** Cada ejecución con la misma seed produce resultados idénticos
- **Ventajas:** Facilita debugging y análisis de comportamiento

## Casos de Uso

### 1. Experimentación Normal
```bash
# NO usar seed fija - queremos diferentes ejecuciones
for i in {1..30}; do
    ./ga -i instancia.graph -t 60 >> resultados.txt
done
```

### 2. Debugging de un Problema
```bash
# Usar seed fija para reproducir el problema
./ga -i instancia_problema.graph -t 60 --seed 123
# Siempre veremos el mismo comportamiento problemático
```

### 3. Validación para Artículos/Reportes
```bash
# Múltiples seeds fijas para obtener estadísticas
for seed in 1 2 3 4 5 10 20 30 40 50; do
    ./ga -i instancia.graph -t 60 --seed $seed
done
```
- **Nota:** Usar diferentes seeds fijas (no la misma) para tener variabilidad estadística
- **Objetivo:** Poder reproducir exactamente los resultados del paper

### 4. Comparación Justa entre Configuraciones
```bash
# Mismas seeds para comparar dos configuraciones de parámetros
for seed in 1 2 3 4 5; do
    ./ga -i inst.graph -t 60 --pop_size 50 --seed $seed > config1_$seed.txt
    ./ga -i inst.graph -t 60 --pop_size 100 --seed $seed > config2_$seed.txt
done
```

### 5. Tuning Automático (irace, SMAC, etc.)
```bash
# NO usar seed fija - dejar aleatorio
./ga -i instancia.graph -t 10 --pop_size $1 --p_cruce $2 --p_mut $3
```
- **Importante:** Los configuradores automáticos necesitan variabilidad
- **Razón:** Evitar sobreajuste a una seed específica

## Valores Recomendados de Seed

### Para Debugging
- **42:** Clásica en computación
- **123:** Fácil de recordar
- **12345:** Otra opción simple

### Para Validación Científica
Se recomienda usar múltiples seeds para obtener estadísticas robustas:
- **Seeds pequeñas:** 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
- **Seeds espaciadas:** 10, 20, 30, 40, 50, 60, 70, 80, 90, 100
- **Seeds aleatorias:** 12345, 54321, 98765, etc.

**Nota:** Lo importante es usar la misma lista de seeds en todos los experimentos para poder compararlos.

## Script de Ejemplo para Validación

```bash
#!/bin/bash
# validacion_estadistica.sh

INSTANCIA="instancia.graph"
TIEMPO=60
SEEDS=(1 2 3 4 5 10 20 30 40 50 60 70 80 90 100)

echo "Instancia,Seed,Calidad,Tiempo" > resultados.csv

for seed in "${SEEDS[@]}"; do
    echo "Ejecutando seed $seed..."
    resultado=$(./ga -i $INSTANCIA -t $TIEMPO --seed $seed | tail -1)
    calidad=$(echo $resultado | grep -oP 'Final: \K[0-9]+')
    tiempo=$(echo $resultado | grep -oP 'Tiempo: \K[0-9.]+')
    echo "$INSTANCIA,$seed,$calidad,$tiempo" >> resultados.csv
done

echo "Resultados guardados en resultados.csv"
```

## Análisis Estadístico Básico

```python
import pandas as pd
import numpy as np

# Leer resultados
df = pd.read_csv('resultados.csv')

# Estadísticas descriptivas
print(f"Media: {df['Calidad'].mean():.2f}")
print(f"Desviación estándar: {df['Calidad'].std():.2f}")
print(f"Mediana: {df['Calidad'].median():.2f}")
print(f"Mínimo: {df['Calidad'].min()}")
print(f"Máximo: {df['Calidad'].max()}")
print(f"CV: {(df['Calidad'].std() / df['Calidad'].mean()) * 100:.2f}%")
```

## Mejores Prácticas

### ✅ HACER:
- Usar seed aleatoria (sin --seed) en experimentos normales
- Usar múltiples seeds fijas diferentes para validación estadística
- Documentar qué seeds se usaron en los experimentos
- Usar la misma seed para reproducir un bug o comportamiento específico

### ❌ NO HACER:
- Usar una sola seed fija para todos los experimentos
- Usar seed fija en tuning automático (irace/SMAC)
- Comparar resultados con seeds diferentes (no son comparables)
- Olvidar documentar las seeds usadas en el paper

## Resumen

| Escenario | Seed | Razón |
|-----------|------|-------|
| Experimentación normal | Sin especificar | Máxima variabilidad |
| Debugging | Fija (ej: 42) | Reproducibilidad exacta |
| Validación científica | Múltiples fijas (1-10) | Estadísticas reproducibles |
| Tuning automático | Sin especificar | Evitar sobreajuste |
| Comparación de configs | Mismas seeds fijas | Comparación justa |

---

**Autor:** Generado automáticamente  
**Versión:** 1.0  
**Fecha:** 2025
