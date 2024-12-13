#include <stdio.h>
#include "storage.h" // Declaraciones para create_table, insert_row, etc.
#include "unit_tests.h"
#include "skiplist.h" // Implementación de Skip List

void run_tests()
{
    printf("=== INICIANDO PRUEBAS UNITARIAS ===\n");

    // Crear una tabla
    Table *table = create_table(10);
    printf("Tabla creada con capacidad maxima de %d filas.\n", table->max_rows);

    // Cargar datos desde un archivo
    load_test_data(table, "test_data.txt");
    printf("Numero de filas cargadas: %d\n", table->num_rows);

    // Verificar contenido cargado
    printf("\nFilas cargadas:\n");
    select_rows(table);

    // Probar consultas con WHERE
    ParsedCommand command_where = {
        .type = COMMAND_SELECT_WHERE,
        .column_name = "id",
        .operator= "=",
        .value = "1"};
    printf("\nConsulta WHERE id = 1:\n");
    select_rows_with_where(table, &command_where);

    // Liberar la tabla
    free_table(table);
    printf("\nTabla liberada correctamente.\n");
    printf("=== FIN DE PRUEBAS UNITARIAS ===\n");
}