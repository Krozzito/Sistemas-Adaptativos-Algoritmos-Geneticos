# 🚀 Quick Start - Algoritmo Genético MISP

## Inicio Rápido (3 pasos)

### 1️⃣ Compilar

```bash
g++ -std=c++17 -O3 -o ga ga.cpp
```

### 2️⃣ Ejecutar

```bash
./ga -i tests/test_small.graph -t 5
```

### 3️⃣ Resultado esperado

```
Calidad: 2, Tiempo: 0.0000s
Final: 2, Tiempo: 0.0000s
```

---

## 📖 Uso Básico

```bash
./ga -i <archivo_grafo> -t <segundos>
```

**Ejemplo:**
```bash
./ga -i mi_grafo.graph -t 10
```

---

## ⚙️ Parámetros Principales

| Parámetro | Descripción | Por defecto |
|-----------|-------------|-------------|
| `-i <file>` | Archivo del grafo | *requerido* |
| `-t <time>` | Tiempo en segundos | *requerido* |
| `--pop_size` | Tamaño población | 100 |
| `--p_mut` | Prob. mutación | 0.01 |

---

## 🧪 Probar que funciona

```bash
# Ejecutar tests
./tests/run_tests.sh

# Ver ejemplos interactivos
./ejemplos_uso.sh
```

---

## 📚 Documentación Completa

- **README.md** → Guía completa de uso
- **VALIDACION.md** → Resultados de las pruebas
- **ANALISIS_TECNICO.md** → Análisis profundo del código

---

## 💡 Ejemplos Rápidos

### Exploración alta (buscar nuevas soluciones)
```bash
./ga -i grafo.graph -t 10 --p_mut 0.05
```

### Explotación alta (refinar soluciones)
```bash
./ga -i grafo.graph -t 10 --seeding_rate 0.5
```

### Población grande
```bash
./ga -i grafo.graph -t 30 --pop_size 200
```

---

## ❓ Problemas Comunes

**No compila:**
```bash
# Verifica versión de g++
g++ --version  # Debe ser >= 7.0
```

**Archivo no encontrado:**
```bash
# Usa ruta completa
./ga -i /ruta/completa/al/grafo.graph -t 10
```

---

## ✅ Estado de Validación

| Componente | Estado |
|------------|--------|
| Compilación | ✅ |
| Tests básicos | ✅ |
| Tests con parámetros | ✅ |
| Grafos grandes | ✅ |
| Any-time | ✅ |

**Implementación validada y lista para usar** 🎉

---

**¿Necesitas más ayuda?** Consulta README.md
