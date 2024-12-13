#ifndef STORAGE_H
#define STORAGE_H

#include <stdint.h>
#include "sql_parser.h"
#include "skiplist.h"

#define COLUMN_USERNAME_SIZE 20
#define COLUMN_EMAIL_SIZE 30


// Definición del número máximo de filas por tabla
#define TABLE_MAX_ROWS 1000  // Puedes ajustar este valor según sea necesario

// Estructura para representar una fila
typedef struct {
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
} Row;

// Estructura para representar una tabla
/*typedef struct {
    uint32_t num_rows;      // Número de filas actualmente en la tabla
    uint32_t max_rows;      // Máximo número de filas permitido
    Row* rows;              // Arreglo dinámico de filas
} Table;*/

typedef struct {
    uint32_t num_rows;      // Número de filas actualmente en la tabla
    uint32_t max_rows;      // Máximo número de filas permitido
    Row* rows;              // Arreglo dinámico de filas
    SkipList* index;        // Índice de Skip List
} Table;


// Funciones para manejar tablas y filas
Table* create_table(uint32_t max_rows);
void free_table(Table* table);
void save_table(Table* table, const char* filename);
Table* load_table(const char* filename);
void insert_row(Table* table, Row row);
void select_rows(Table* table);
// Prototipo en storage.h o un archivo similar
void select_rows_with_where(Table *table, ParsedCommand *command);
// Añadir al final del archivo `storage.h`
void load_test_data(Table* table, const char* filename);


#endif // STORAGE_H
