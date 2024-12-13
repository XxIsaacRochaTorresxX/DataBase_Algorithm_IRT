#ifndef SQL_PARSER_H
#define SQL_PARSER_H

// Tipos de comandos SQL soportados
typedef enum {
    COMMAND_CREATE_DATABASE,
    COMMAND_USE,
    COMMAND_CREATE_TABLE,
    COMMAND_INSERT,
    COMMAND_SELECT,
    COMMAND_LOAD_TABLE,
    COMMAND_DROP_TABLE,
    COMMAND_SELECT_WHERE,
    COMMAND_SELECT_LIMIT,
    COMMAND_UNKNOWN
} CommandType;

// Resultado del parser
typedef enum {
    PARSE_OK,
    PARSE_SYNTAX_ERR
} ParseResult;

// Estructura para almacenar el comando parseado
typedef struct {
    CommandType type;  // Tipo de comando
    char arguments[256]; // Argumentos generales del comando
    char table_name[128]; // Nombre de la tabla (agrega este campo si lo necesitas)
    char column_name[64]; // Nombre de la columna en el WHERE
    char operator[3];     // Operador: '=', '<', '>', etc.
    char value[64];       // Valor para comparar
} ParsedCommand;

// Función para analizar comandos SQL
//ParseResult parse_sql(const char* input, ParsedCommand* command);
ParsedCommand parse_command(const char *input);

#endif // SQL_PARSER_H
