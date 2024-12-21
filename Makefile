# Nombre del ejecutable
TARGET = build/programa

# Archivos fuente
SRCS = $(wildcard src/*.c)

# Archivos de cabecera
INCLUDES = -Iinclude

# Flags de compilación
CFLAGS = -Wall -Wextra -g

# Compilador
CC = gcc

# Regla principal
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCS) -o $(TARGET) -lm

# Limpieza
clean:
	rm -rf build/*
