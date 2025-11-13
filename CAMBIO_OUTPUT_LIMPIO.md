# Cambio a Output Limpio - Solo Número

## 📋 Resumen de Cambios

Se ha modificado el programa para que **imprima exclusivamente un número** (la calidad de la mejor solución encontrada) en lugar de los mensajes detallados anteriores.

## 🔄 Output Anterior vs Nuevo

### Antes (Output detallado con anytime reporting):
```
Calidad: 5, Tiempo: 0.0001s
Calidad: 5, Tiempo: 0.0153s
Calidad: 6, Tiempo: 0.0847s
Final: 6, Tiempo: 0.0847s
```

### Ahora (Output limpio - solo número):
```
6
```

## ✅ Archivos Modificados

### 1. `ga.cpp` (Versión Monolítica)
- **Línea ~93**: Comentado el print de la primera solución
- **Línea ~136**: Comentado el print de mejoras incrementales (anytime)
- **Línea ~142**: Cambiado a imprimir solo el número: `cout << mejor_solucion_global.fitness << endl;`

### 2. `src/AlgoritmoGenetico.cpp` (Versión Modular)
- **Línea ~67**: Comentado el print de la primera solución
- **Línea ~113**: Comentado el print de mejoras incrementales (anytime)
- **Línea ~121**: Cambiado a imprimir solo el número: `cout << mejor_solucion_global.fitness << endl;`

### 3. `tests/run_tests.sh`
- Actualizado para parsear el nuevo formato (solo número)
- Eliminada la extracción regex de "Final: X"
- Añadida validación de que el output sea un número válido

## 🎯 Ventajas del Nuevo Formato

### Para Automatización
```bash
# Fácil de capturar y procesar
resultado=$(./ga -i grafo.graph -t 60)
echo "La calidad es: $resultado"

# Perfecto para scripts de tuning
for config in config1 config2 config3; do
    quality=$(./ga -i inst.graph -t 30 $config)
    echo "$config,$quality" >> resultados.csv
done
```

### Para irace/SMAC
```bash
#!/bin/bash
# target-runner.sh
INSTANCE=$1
shift
PARAMS=$@

# Output directo sin necesidad de parseo complejo
QUALITY=$(./ga -i "$INSTANCE" -t 30 $PARAMS)
echo "-$QUALITY"  # Negativo porque irace minimiza
```

### Para Análisis Estadístico
```bash
# Ejecutar 30 réplicas y obtener estadísticas
for i in {1..30}; do
    ./ga -i grafo.graph -t 60 --seed $i
done | awk '{sum+=$1; sq+=$1*$1} END {
    print "Media:", sum/NR
    print "Desv:", sqrt(sq/NR - (sum/NR)^2)
}'
```

## 🔍 Debugging (si necesitas el output detallado)

Si necesitas ver el progreso del algoritmo para debugging, puedes descomentar las líneas:

### En ga.cpp:
```cpp
// Línea ~93 - Primera solución
cout << "Calidad: " << mejor_solucion_global.fitness << ", Tiempo: " << fixed << setprecision(4) << elapsed_found << "s" << endl;

// Línea ~136 - Mejoras incrementales
cout << "Calidad: " << mejor_solucion_global.fitness << ", Tiempo: " << fixed << setprecision(4) << elapsed_found << "s" << endl;
```

### En src/AlgoritmoGenetico.cpp:
```cpp
// Línea ~67 - Primera solución
cout << "Calidad: " << mejor_solucion_global.fitness 
     << ", Tiempo: " << fixed << setprecision(4) << elapsed_found << "s" << endl;

// Línea ~113 - Mejoras incrementales
cout << "Calidad: " << mejor_solucion_global.fitness 
     << ", Tiempo: " << fixed << setprecision(4) << elapsed_found << "s" << endl;
```

## 📊 Uso Típico

### Ejecución Simple
```bash
$ ./ga -i grafo.graph -t 10
42
```

### Capturar Resultado
```bash
resultado=$(./ga -i grafo.graph -t 60 --pop_size 100)
echo "Mejor solución: $resultado"
```

### Múltiples Ejecuciones
```bash
for i in {1..10}; do
    ./ga -i grafo.graph -t 30 --seed $i >> resultados.txt
done

# Analizar resultados
cat resultados.txt | sort -rn | head -1  # Mejor
cat resultados.txt | sort -n | head -1   # Peor
```

### Para Papers/Reportes
```bash
# Obtener estadísticas de 30 ejecuciones
echo "seed,quality" > experimento.csv
for seed in {1..30}; do
    quality=$(./ga -i instancia.graph -t 120 --seed $seed)
    echo "$seed,$quality" >> experimento.csv
done

# Análisis en Python/R
python analyze_results.py experimento.csv
```

## ✅ Pruebas Realizadas

Todos los tests siguen pasando correctamente:

```bash
$ cd tests && bash run_tests.sh
=========================================
  PRUEBAS DEL ALGORITMO GENÉTICO - MISP
=========================================

✓ PASS - Triángulo (K3): 1
✓ PASS - Grafo Estrella: 5
✓ PASS - Ciclo C5: 2
... (9 tests más)

=========================================
  PRUEBAS COMPLETADAS
=========================================
```

## 🔧 Compilación

Ambas versiones compilan sin warnings:

```bash
# Versión modular
make clean && make
# ✅ Build complete: ga

# Versión monolítica
g++ -std=c++17 -O3 -o ga_mono ga.cpp
# ✅ Compilado exitosamente
```

## 📝 Notas Importantes

1. **Compatibilidad hacia atrás**: Si tienes scripts que esperan el formato anterior, necesitarás actualizarlos
2. **Debugging**: Descomenta las líneas si necesitas ver el progreso del algoritmo
3. **Tuning automático**: El nuevo formato es **perfecto** para herramientas como irace/SMAC
4. **Scripts de validación**: Mucho más simples de escribir ahora

---

**Fecha de cambio:** Noviembre 7, 2025  
**Motivo:** Simplificar el output para automatización y tuning  
**Estado:** ✅ Implementado y probado en ambas versiones
