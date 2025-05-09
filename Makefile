# Nombre del compilador de C++
CXX = clang++

# Flags del compilador:
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Directorios
IDIR = ./include
SRCDIR = ./src
ODIR = ./obj

# --- SFML ---
SFML_INCLUDE_DIR = /opt/homebrew/Cellar/sfml/3.0.1/include
SFML_LIB_DIR = /opt/homebrew/Cellar/sfml/3.0.1/lib # Asumiendo que las bibliotecas están aquí
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# --- OpenCV ---
OPENCV_CFLAGS = $(shell pkg-config --cflags opencv4)
OPENCV_LIBS = $(shell pkg-config --libs opencv4)

# --- Agregado de Inclusiones y Bibliotecas ---
INCLUDES = -I$(IDIR) -I$(SFML_INCLUDE_DIR) $(OPENCV_CFLAGS)
LDFLAGS = -L$(SFML_LIB_DIR) # Para que el enlazador encuentre las bibliotecas SFML
LIBS = $(SFML_LIBS) $(OPENCV_LIBS)

EXEC = motion_transfer_app

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(ODIR)/%.o,$(SRCS))

# --- Reglas del Makefile ---

all: $(EXEC)

$(EXEC): $(OBJS)
	@echo "Enlazando para crear el ejecutable: $@"
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)
	@echo "Ejecutable '$@' creado exitosamente."
	chmod +x $@

$(ODIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(ODIR)
	@echo "Compilando: $< -> $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "Limpiando archivos compilados..."
	rm -f $(ODIR)/*.o $(EXEC)
	@echo "Limpieza completada."

run: all
	@echo "Ejecutando la aplicación..."
	./$(EXEC)

.PHONY: all clean run