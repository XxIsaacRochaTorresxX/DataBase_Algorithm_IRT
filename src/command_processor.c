#include "command_processor.h"
#include "sql_parser.h"
#include "storage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>   // Agregar esta línea
#include <sys/stat.h> // Para crear directorios
#include <unistd.h>   // Para verificar archivos

#ifdef _WIN32
#include <direct.h> // mkdir en Windows
#endif
// Se está usando una macro condicional para asegurar la portabilidad del código entre sistemas operativos.

// Base de datos activa
char current_database[256] = "./"; // Directorio raíz por defecto
// Verifica si una base de datos está seleccionada
int check_database_selected()
{
    struct stat st;
    if (stat(current_database, &st) != 0 || !S_ISDIR(st.st_mode))
    {
        printf("Error: No se ha seleccionado una base de datos o no existe.\n");
        return 0;
    }
    return 1;
}

// Tabla global para el ejemplo
// #define TABLE_MAX_ROWS 100
Table *global_table = NULL;

// Función para construir la ruta completa de una tabla
/*void build_table_filepath(const char *table_name, char *filepath, size_t size)
{
    snprintf(filepath, size, "%s/%s.bin", current_database, table_name);
}*/

void build_table_filepath(const char *table_name, char *filepath, size_t size)
{
    // Verificar si `current_database` ya incluye una barra final
    const char *separator = (current_database[strlen(current_database) - 1] == '/') ? "" : "/";
    snprintf(filepath, size, "%s%s%s.bin", current_database, separator, table_name);
}

// Ejecutar el comando parseado
void execute_command(ParsedCommand command)
{

    if (global_table == NULL)
    {
        global_table = create_table(TABLE_MAX_ROWS);
    }

    switch (command.type)
    {
    case COMMAND_CREATE_DATABASE:
    {
#ifdef _WIN32
        if (mkdir(command.arguments) == 0) // Windows solo necesita un argumento
#else
        if (mkdir(command.arguments, 0777) == 0) // Linux/Unix permite especificar permisos
#endif
        {
            printf("Base de datos '%s' creada.\n", command.arguments);
            strcpy(current_database, command.arguments); // Actualiza el directorio actual
        }
        else
        {
            printf("Error al crear base de datos: %s\n", command.arguments);
        }
        break;
    }
    case COMMAND_USE:
    {
        // Verifica si el directorio existe antes de cambiar
        struct stat st;
        if (stat(command.arguments, &st) == 0 && S_ISDIR(st.st_mode))
        {
            strcpy(current_database, command.arguments); // Actualiza el directorio actual
            printf("Usando base de datos: %s\n", current_database);
        }
        else
        {
            printf("Error: Base de datos '%s' no encontrada.\n", command.arguments);
        }
        break;
    }
    case COMMAND_CREATE_TABLE:
    {
        if (!check_database_selected())
            break;
        char filepath[512];
        build_table_filepath(command.arguments, filepath, sizeof(filepath));

        // Verifica si el directorio actual (base de datos) existe
        struct stat st;
        if (stat(current_database, &st) != 0 || !S_ISDIR(st.st_mode))
        {
            printf("Error: No se ha seleccionado o no existe la base de datos actual.\n");
            break;
        }

        FILE *file = fopen(filepath, "wb");
        if (file)
        {
            fclose(file);
            printf("Tabla '%s' creada en la base de datos '%s'.\n", command.arguments, current_database);
        }
        else
        {
            printf("Error: No se pudo crear la tabla.\n");
        }
        break;
    }
    case COMMAND_INSERT:
    {
        char filepath[512];
        build_table_filepath(command.table_name, filepath, sizeof(filepath));

        if (!global_table)
        {
            // Carga la tabla desde el archivo correspondiente
            global_table = load_table(filepath);
            if (!global_table)
            {
                printf("Error: No se pudo cargar la tabla '%s'.\n", command.table_name);
                break;
            }
        }
        // Buscar "VALUES ("
        const char *values_start = strstr(command.arguments, "VALUES (");
        if (!values_start)
        {
            printf("Error: Formato incorrecto. Uso: INSERT INTO users VALUES (<id>, '<username>', '<email>')\n");
            break;
        }

        // Mover el puntero al primer valor dentro de los paréntesis
        values_start += 8; // Saltar "VALUES ("

        // Buscar el paréntesis de cierre
        const char *values_end = strchr(values_start, ')');
        if (!values_end)
        {
            printf("Error: Formato incorrecto. Falta un paréntesis de cierre.\n");
            break;
        }

        // Extraer los valores como una subcadena
        char values_str[256] = {0};
        strncpy(values_str, values_start, values_end - values_start);

        // Parsear los valores separados por comas
        int id;
        char username[COLUMN_USERNAME_SIZE];
        char email[COLUMN_EMAIL_SIZE];
        if (sscanf(values_str, "%d, '%[^']', '%[^']'", &id, username, email) != 3)
        {
            printf("Error: Formato incorrecto. Uso: INSERT INTO users VALUES (<id>, '<username>', '<email>')\n");
            break;
        }
        // validación de longitud
        if (strlen(username) >= COLUMN_USERNAME_SIZE || strlen(email) >= COLUMN_EMAIL_SIZE)
        {
            printf("Error: Los valores de Username o Email exceden el tamaño permitido.\n");
            break;
        }

        // Crear una fila y agregarla a la tabla
        Row row = {.id = id};
        strncpy(row.username, username, COLUMN_USERNAME_SIZE);
        strncpy(row.email, email, COLUMN_EMAIL_SIZE);
        insert_row(global_table, row);
        printf("Fila insertada: ID = %d, Username = %s, Email = %s\n", id, username, email);

        // Guardar los cambios en disco
        // char filepath[512];
        build_table_filepath(command.table_name, filepath, sizeof(filepath));
        save_table(global_table, filepath);
        break;
    }

    case COMMAND_SELECT:
    {
        if (!global_table)
        {
            printf("Error: Ninguna tabla cargada. Use una tabla primero.\n");
            break;
        }

        printf("Filas en la tabla:\n");
        select_rows(global_table);
        break;
    }
    /* case COMMAND_SELECT_WHERE:
     {
         if (!global_table)
         {
             printf("Error: Ninguna tabla cargada. Use una tabla primero.\n");
             break;
         }

         // Extraer la condición después de "WHERE"
         const char *where_clause = strstr(command.arguments, "WHERE") + 6;
         char condition[128];
         strncpy(condition, where_clause, sizeof(condition) - 1);

         // Procesar y aplicar el filtro
         printf("Filas que cumplen la condición '%s':\n", condition);
         for (uint32_t i = 0; i < global_table->num_rows; i++)
         {
             Row *row = &global_table->rows[i];
             // Aquí podrías evaluar dinámicamente la condición
             if (strstr(row->username, condition) != NULL)
             { // Ejemplo simple: filtrar por username
                 printf("ID = %d, Username = %s, Email = %s\n", row->id, row->username, row->email);
             }
         }
         break;
     }*/
    case COMMAND_SELECT_WHERE:
    {
        if (!global_table)
        {
            char filepath[512];
            build_table_filepath(command.table_name, filepath, sizeof(filepath));
            global_table = load_table(filepath);
            if (!global_table)
            {
                printf("Error: No se pudo cargar la tabla '%s'.\n", command.table_name);
                return;
            }
        }

        printf("Aplicando filtro WHERE:\n");
        select_rows_with_where(global_table, &command);
        break;
    }

    case COMMAND_SELECT_LIMIT:
    {
        if (!global_table)
        {
            printf("Error: Ninguna tabla cargada. Use una tabla primero.\n");
            break;
        }

        // Extraer el valor de LIMIT
        const char *limit_clause = strstr(command.arguments, "LIMIT") + 6;
        int limit = atoi(limit_clause);

        printf("Mostrando un máximo de %d filas:\n", limit);
        for (uint32_t i = 0; i < global_table->num_rows && i < limit; i++)
        {
            Row *row = &global_table->rows[i];
            printf("ID = %d, Username = %s, Email = %s\n", row->id, row->username, row->email);
        }
        break;
    }
    case COMMAND_LOAD_TABLE:
    {
        char filepath[512];
        build_table_filepath(command.arguments, filepath, sizeof(filepath));

        if (global_table != NULL)
        {
            free_table(global_table);
            global_table = NULL;
        }

        global_table = load_table(filepath);
        if (global_table)
        {
            printf("Tabla '%s' cargada desde la base de datos '%s'.\n", command.arguments, current_database);
        }
        else
        {
            printf("Error al cargar la tabla '%s'.\n", command.arguments);
        }
        break;
    }

    case COMMAND_DROP_TABLE:
    {
        char filepath[512];
        build_table_filepath(command.arguments, filepath, sizeof(filepath));
        if (unlink(filepath) == 0)
        {
            printf("Tabla '%s' eliminada.\n", command.arguments);
        }
        else
        {
            printf("Error: Tabla '%s' no encontrada.\n", command.arguments);
        }
        break;
    }

    case COMMAND_UNKNOWN:
    default:
        printf("Error: Comando no reconocido.\n");
        break;
    }
}
