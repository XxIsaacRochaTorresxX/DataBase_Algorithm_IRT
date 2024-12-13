#include "sql_parser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <stdio.h>
#include <ctype.h> // Para manejar cadenas

// Analiza un comando SQL y retorna el tipo y argumentos
ParsedCommand parse_command(const char *input)
{
    ParsedCommand command = {COMMAND_UNKNOWN, ""};

    // Convertir a minúsculas para facilitar la comparación
    char lower_input[512];
    for (size_t i = 0; i < strlen(input) && i < sizeof(lower_input) - 1; i++)
    {
        lower_input[i] = tolower(input[i]);
    }
    lower_input[strlen(input)] = '\0'; // Asegurarse de que termine en '\0'

    if (strncmp(lower_input, "create database", 15) == 0)
    {
        command.type = COMMAND_CREATE_DATABASE;
        strncpy(command.arguments, input + 16, sizeof(command.arguments) - 1);
    }
    else if (strncmp(lower_input, "create table", 12) == 0)
    {
        command.type = COMMAND_CREATE_TABLE;
        sscanf(input + 13, "%s", command.table_name); // Guarda el nombre de la tabla
        strncpy(command.arguments, input + 13, sizeof(command.arguments) - 1);
    }
    else if (strncmp(lower_input, "use", 3) == 0)
    {
        command.type = COMMAND_USE;
        strncpy(command.arguments, input + 4, sizeof(command.arguments) - 1);
    }
    else if (strncmp(lower_input, "insert into", 11) == 0)
    {
        command.type = COMMAND_INSERT;
        strncpy(command.arguments, input + 12, sizeof(command.arguments) - 1);
    }
    else if (strncmp(lower_input, "select", 6) == 0)
    {
        command.type = COMMAND_SELECT;
        strncpy(command.arguments, input + 7, sizeof(command.arguments) - 1);
    }
    else if (strncmp(lower_input, "drop table", 10) == 0)
    {
        command.type = COMMAND_DROP_TABLE;
        strncpy(command.arguments, input + 11, sizeof(command.arguments) - 1);
    }
    else if (strncmp(lower_input, "load table", 10) == 0)
    {
        command.type = COMMAND_LOAD_TABLE;
        strncpy(command.arguments, input + 11, sizeof(command.arguments) - 1);
    }
    else if (strncmp(lower_input, "select", 6) == 0 && strstr(lower_input, "where") != NULL)
    {
        command.type = COMMAND_SELECT_WHERE;

        char *where_clause = strstr(input, "WHERE");
        if (where_clause)
        {
            sscanf(where_clause + 6, "%s %s %s", command.column_name, command.operator, command.value);
        }

        // Ajustar para capturar correctamente el nombre de la tabla
        char *from_clause = strstr(lower_input, "from") + 5;
        sscanf(from_clause, "%s", command.table_name);
    }

    else if (strncmp(lower_input, "select", 6) == 0 && strstr(lower_input, "limit") != NULL)
    {
        command.type = COMMAND_SELECT_LIMIT;
        strncpy(command.arguments, input + 7, sizeof(command.arguments) - 1);
    }

    return command;
}

/*
ParseResult parse_sql(const char *input, ParsedCommand *command)
{
    // Inicializar el comando como desconocido
    command->type = COMMAND_UNKNOWN;
    command->arguments = NULL;

    // Parsear los comandos SQL soportados
    if (strncmp(input, "CREATE DATABASE", 15) == 0)
    {
        command->type = COMMAND_CREATE_DATABASE;
        command->arguments = strdup(input + 16); // Extraer argumentos
        return PARSE_OK;
    }
    else if (strncmp(input, "CREATE TABLE", 12) == 0)
    {
        command->type = COMMAND_CREATE_TABLE;
        command->arguments = strdup(input + 13); // Extraer argumentos
        return PARSE_OK;
    }
    else if (strncmp(input, "INSERT INTO", 11) == 0)
    {
        //
        if (strstr(input, "VALUES") != NULL)
        {
            command->type = COMMAND_INSERT;
            command->arguments = strdup(input + 12); // Extraer argumentos
            return PARSE_OK;                         // Reconocemos el comando si incluye "VALUES"
        }
        else
        {
            return PARSE_SYNTAX_ERR;
        }
        //
    }
    else if (strncmp(input, "SELECT", 6) == 0)
    {
        command->type = COMMAND_SELECT;
        command->arguments = strdup(input + 7); // Extraer argumentos
        return PARSE_OK;
    }
    else if (strncmp(input, "DROP TABLE", 10) == 0)
    {
        command->type = COMMAND_DROP_TABLE;
        command->arguments = strdup(input + 11); // Extraer argumentos
        return PARSE_OK;
    }

    // Si no se reconoce el comando, devolver error de sintaxis
    return PARSE_SYNTAX_ERR;
}
*/
