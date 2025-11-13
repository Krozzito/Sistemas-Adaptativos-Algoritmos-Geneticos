# Makefile para Algoritmo Genético - MISP
# Compilación modular con soporte para debug y release

# Compilador y flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
LDFLAGS = 

# Directorios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .
TEST_DIR = tests

# Archivos fuente
SOURCES = $(SRC_DIR)/Individuo.cpp \
          $(SRC_DIR)/Grafo.cpp \
          $(SRC_DIR)/Greedy.cpp \
          $(SRC_DIR)/Operadores.cpp \
          $(SRC_DIR)/AlgoritmoGenetico.cpp \
          $(SRC_DIR)/main.cpp

# Archivos objeto
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Ejecutable
TARGET = $(BIN_DIR)/ga

# Configuración de build (default: release)
BUILD ?= release

ifeq ($(BUILD),debug)
    CXXFLAGS += -g -O0 -DDEBUG
    $(info Building in DEBUG mode)
else
    CXXFLAGS += -O3 -DNDEBUG
    $(info Building in RELEASE mode)
endif

# Reglas principales
.PHONY: all clean test run help debug release

all: $(TARGET)

# Compilar ejecutable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	@echo "Linking $@..."
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Build complete: $@"

# Compilar archivos objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Crear directorios si no existen
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compilación en modo debug
debug:
	@$(MAKE) BUILD=debug

# Compilación en modo release
release:
	@$(MAKE) BUILD=release

# Limpiar archivos compilados
clean:
	@echo "Cleaning..."
	rm -rf $(OBJ_DIR)
	rm -f $(TARGET)
	@echo "Clean complete"

# Ejecutar tests
test: $(TARGET)
	@echo "Running tests..."
	@if [ -f $(TEST_DIR)/run_tests.sh ]; then \
		cd $(TEST_DIR) && ./run_tests.sh; \
	else \
		echo "Test script not found!"; \
	fi

# Ejecutar con ejemplo simple
run: $(TARGET)
	@echo "Running example..."
	./$(TARGET) -i $(TEST_DIR)/test_small.graph -t 5

# Ejecutar con grafo grande
run-large: $(TARGET)
	@echo "Running on large graph..."
	./$(TARGET) -i ../dataset_grafos_no_dirigidos/new_1000_dataset/erdos_n1000_p0c0.05_1.graph -t 10 --pop_size 100

# Mostrar ayuda
help:
	@echo "Makefile para Algoritmo Genético - MISP"
	@echo ""
	@echo "Targets disponibles:"
	@echo "  make              - Compila en modo release (default)"
	@echo "  make debug        - Compila en modo debug (con símbolos)"
	@echo "  make release      - Compila en modo release (optimizado)"
	@echo "  make clean        - Elimina archivos compilados"
	@echo "  make test         - Compila y ejecuta tests"
	@echo "  make run          - Compila y ejecuta ejemplo simple"
	@echo "  make run-large    - Compila y ejecuta en grafo grande"
	@echo "  make help         - Muestra esta ayuda"
	@echo ""
	@echo "Variables:"
	@echo "  BUILD=debug       - Compila en modo debug"
	@echo "  BUILD=release     - Compila en modo release (default)"
	@echo ""
	@echo "Ejemplos:"
	@echo "  make clean && make          # Compilación limpia release"
	@echo "  make BUILD=debug            # Compilación en debug"
	@echo "  make clean test             # Limpiar y ejecutar tests"

# Dependencias automáticas (opcional, para recompilar si cambian headers)
-include $(OBJECTS:.o=.d)

$(OBJ_DIR)/%.d: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) -MM -MT '$(OBJ_DIR)/$*.o' $< > $@
