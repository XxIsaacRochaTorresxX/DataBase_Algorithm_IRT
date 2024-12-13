#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sql_parser.h"  // Importamos el parse
#include "command_processor.h" // 


#ifdef _WIN32
typedef int ssize_t;
#endif

#define INPUT_BUFFER_SIZE 256

// Estructura para almacenar el buffer de entrada
typedef struct {
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

// Inicializar el buffer de entrada
InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer->buffer = (char*)malloc(INPUT_BUFFER_SIZE * sizeof(char)); // Asignación de memoria
    input_buffer->buffer_length = INPUT_BUFFER_SIZE;
    input_buffer->input_length = 0;
    return input_buffer;
}

// Leer la entrada del usuario
void read_input(InputBuffer* input_buffer) {
    if (fgets(input_buffer->buffer, input_buffer->buffer_length, stdin) == NULL) {
        printf("Error leyendo entrada\n");
        exit(EXIT_FAILURE);
    }

    // Eliminar salto de línea final
    input_buffer->input_length = strlen(input_buffer->buffer) - 1;
    if (input_buffer->buffer[input_buffer->input_length] == '\n') {
        input_buffer->buffer[input_buffer->input_length] = 0;
    }
}

// Función para liberar el buffer de entrada
void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

// Función para iniciar el REPL
void start_repl() {
    InputBuffer* input_buffer = new_input_buffer();
    printf("Bienvenido al motor de base de datos minima.\n");

    while (1) {
        printf("db > ");
        read_input(input_buffer);

        // Manejo del comando especial ".exit"
        if (strcmp(input_buffer->buffer, ".exit") == 0) {
            close_input_buffer(input_buffer);
            printf("Saliendo...\n");
            exit(EXIT_SUCCESS);
        }

        // Inicializar el comando parseado
        // Analizar y ejecutar el comando
        ParsedCommand command = parse_command(input_buffer->buffer);
        if (command.type != COMMAND_UNKNOWN) {
            execute_command(command);
        } else {
            printf("Error: Comando SQL no reconocido.\n");
        }
    }
}
