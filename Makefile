# Variables del compilador y opciones
CC = gcc
CFLAGS = -Wall -Iinclude
OBJ = src/main.o src/repl.o src/sql_parser.o src/command_processor.o src/storage.o src/skiplist.o src/utils.o src/unit_tests.o

# Objetivo principal
all: build/mini_db

# Enlace del binario final
build/mini_db: $(OBJ)
	$(CC) $(CFLAGS) -o build/mini_db $(OBJ)

# Regla para compilar cada archivo .c a .o
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -rf build/* src/*.o tests/*.o
