#include "storage.h"
#include "skiplist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sql_parser.h>

// Nombre del archivo por defecto para almacenar tablas
#define DEFAULT_TABLE_FILE "table_data.bin"

// Crear una tabla en memoria
/*Table *create_table(uint32_t max_rows)
{
    Table *table = (Table *)malloc(sizeof(Table));
    table->num_rows = 0;
    table->max_rows = max_rows;
    table->rows = (Row *)malloc(sizeof(Row) * max_rows);
    // Inicializar el índice Skip List
    table->index = create_skiplist();
    return table;
}*/

Table *create_table(uint32_t max_rows)
{
    Table *table = (Table *)malloc(sizeof(Table));
    if (!table)
    {
        printf("Error al asignar memoria para la tabla.\n");
        exit(EXIT_FAILURE);
    }
    table->num_rows = 0;
    table->max_rows = max_rows;
    table->rows = (Row *)malloc(sizeof(Row) * max_rows);
    table->index = create_skiplist(); // Crear e inicializar la Skip List
    if (!table->rows || !table->index)
    {
        printf("Error al asignar memoria para filas o índice.\n");
        exit(EXIT_FAILURE);
    }
    return table;
}

// Liberar la memoria de una tabla
/*void free_table(Table *table)
{
    if (table->index)
    {
        skiplist_free(table->index); // Liberar el índice
    }
    free(table->rows);
    free(table);
}
*/
void free_table(Table *table)
{
    if (!table)
    {
        return; // No hay nada que liberar si el puntero es nulo
    }

    if (table->rows)
    {
        free(table->rows); // Liberar las filas si existen
    }

    if (table->index)
    {
        skiplist_free(table->index); // Liberar el índice si existe
    }

    free(table); // Finalmente, liberar la tabla
}

// función que evalúa las condiciones
int evaluate_condition(Row *row, ParsedCommand *command)
{
    if (strcmp(command->column_name, "id") == 0)
    {
        int value = atoi(command->value);
        if (strcmp(command->operator, "=") == 0)
            return row->id == value;
        if (strcmp(command->operator, "<") == 0)
            return row->id < value;
        if (strcmp(command->operator, ">") == 0)
            return row->id > value;
        if (strcmp(command->operator, "!=") == 0)
            return row->id != value;
        if (strcmp(command->operator, "<=") == 0)
            return row->id <= value;
        if (strcmp(command->operator, ">=") == 0)
            return row->id >= value;
    }
    if (strcmp(command->column_name, "username") == 0)
    {
        if (strcmp(command->operator, "=") == 0)
            return strncmp(row->username, command->value, COLUMN_USERNAME_SIZE) == 0;
        if (strcmp(command->operator, "!=") == 0)
            return strcmp(row->username, command->value) != 0;
    }
    if (strcmp(command->column_name, "email") == 0)
    {
        if (strcmp(command->operator, "=") == 0)
            return strcmp(row->email, command->value) == 0;
        if (strcmp(command->operator, "!=") == 0)
            return strcmp(row->email, command->value) != 0;
    }
    return 0; // Condición no válida
}
// Guardar la tabla en un archivo
void save_table(Table *table, const char *filename)
{
    if (!table)
    {
        printf("Error: Tabla no válida para guardar.\n");
        return;
    }
    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        printf("Error: No se pudo abrir el archivo para guardar la tabla.\n");
        return;
    }
    fwrite(&table->num_rows, sizeof(uint32_t), 1, file);

    fwrite(table->rows, sizeof(Row), table->num_rows, file);
    fclose(file);
    printf("Tabla guardada en: %s\n", filename);
}

// Cargar una tabla desde un archivo
Table *load_table(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        printf("Archivo no encontrado: Creando una nueva tabla.\n");
        return create_table(TABLE_MAX_ROWS);
    }
    Table *table = (Table *)malloc(sizeof(Table));
    fread(&table->num_rows, sizeof(uint32_t), 1, file);
    table->max_rows = TABLE_MAX_ROWS;
    table->rows = (Row *)malloc(sizeof(Row) * table->max_rows);
    fread(table->rows, sizeof(Row), table->num_rows, file);
    fclose(file);
    printf("Tabla cargada desde: %s\n", filename);
    return table;
}

// Insertar una fila en la tabla
void insert_row(Table *table, Row row)
{
    if (table->num_rows >= table->max_rows)
    {
        printf("Error: La tabla está llena.\n");
        return;
    }
    table->rows[table->num_rows++] = row; // Solo agregar una vez
    if (table->index)
    {
        skiplist_insert(table->index, row.id, table->num_rows - 1);
    }

    printf("Fila insertada: ID = %d, Username = %s, Email = %s\n", row.id, row.username, row.email);
    /*
    table->rows[table->num_rows++] = row;
    printf("Fila insertada: ID = %d, Username = %s, Email = %s\n", row.id, row.username, row.email);
    */
}

// Mostrar todas las filas de la tabla
void select_rows(Table *table)
{
    for (uint32_t i = 0; i < table->num_rows; i++)
    {
        Row *row = &table->rows[i];
        printf("ID = %d, Username = %s, Email = %s\n", row->id, row->username, row->email);
    }
}

/*
void select_rows_with_where(Table *table, ParsedCommand *command)
{
    printf("Filtrando filas con la condicion: %s %s %s\n",
           command->column_name, command->operator, command->value);

    // Usar el índice Skip List si la columna es "id"
    if (strcmp(command->column_name, "id") == 0 && table->index)
    {
        uint32_t row_index = skiplist_search(table->index, atoi(command->value));
        if (row_index != UINT32_MAX)
        {
            Row *row = &table->rows[row_index];
            printf("ID = %d, Username = %s, Email = %s\n", row->id, row->username, row->email);
        }
        else if (row_index == UINT32_MAX)
        {
            printf("No se encontro ninguna fila con la condición especificada.\n");
            return; // Salir de la función si no se encuentra el valor
        }
        else
        {
            printf("No encontrado.\n");
        }
        return;
    }

    for (uint32_t i = 0; i < table->num_rows; i++)
    {
        Row *row = &table->rows[i];
        if (evaluate_condition(row, command))
        { // Usar evaluate_condition
            printf("ID = %d, Username = %s, Email = %s\n",
                   row->id, row->username, row->email);
        }
    }
}*/

void select_rows_with_where(Table *table, ParsedCommand *command)
{
    printf("Filtrando filas con la condicion: %s %s %s\n",
           command->column_name, command->operator, command->value);

    // Usar el índice Skip List si la columna es "id"
    if (strcmp(command->column_name, "id") == 0 && table->index)
    {
        uint32_t row_index = skiplist_search(table->index, atoi(command->value));
        if (row_index == UINT32_MAX)
        {
            printf("No se encontro ninguna fila con la condición especificada.\n");
            return;
        }
        Row *row = &table->rows[row_index];
        if (!row)
        {
            printf("Error: Fila no válida en el índice.\n");
            return;
        }
        printf("ID = %d, Username = %s, Email = %s\n", row->id, row->username, row->email);
        return;
    }

    // Búsqueda secuencial si no hay índice
    for (uint32_t i = 0; i < table->num_rows; i++)
    {
        Row *row = &table->rows[i];
        if (evaluate_condition(row, command))
        {
            printf("ID = %d, Username = %s, Email = %s\n",
                   row->id, row->username, row->email);
        }
    }
}

void load_test_data(Table *table, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error: No se pudo abrir el archivo %s\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        Row row;
        sscanf(line, "%d,%19[^,],%29s", &row.id, row.username, row.email);
        insert_row(table, row);
    }

    fclose(file);
    printf("Datos cargados desde %s\n", filename);
}
