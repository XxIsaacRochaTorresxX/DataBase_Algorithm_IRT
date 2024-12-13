/*#include <stdio.h>
#include <string.h>    // Para manejar argumentos
#include "repl.h"      // REPL del motor de base de datos
#include "unit_tests.c" // Pruebas unitarias

int main(int argc, char* argv[]) {
    // Verificar argumentos de línea de comandos
    if (argc > 1) {
        if (strcmp(argv[1], "--help") == 0) {
            printf("Uso: mini_db [opciones]\n");
            printf("Opciones:\n");
            printf("  --help         Muestra este mensaje de ayuda\n");
            printf("  --version      Muestra la versión del programa\n");
            printf("  --test         Ejecuta las pruebas unitarias\n");
            return 0;
        } else if (strcmp(argv[1], "--version") == 0) {
            printf("Motor de base de datos mínima, versión 1.0\n");
            return 0;
        } else if (strcmp(argv[1], "--test") == 0) {
            printf("Ejecutando pruebas unitarias...\n");
            run_tests(); // Llamar a la función de pruebas unitarias
            return 0;
        } else {
            printf("Error: Opción no reconocida '%s'. Usa '--help' para ayuda.\n", argv[1]);
            return 1;
        }
    }

    // Iniciar el motor de base de datos en modo interactivo (REPL)
    printf("Bienvenido al motor de base de datos minima.\n");
    start_repl();
    return 0;
}
*/