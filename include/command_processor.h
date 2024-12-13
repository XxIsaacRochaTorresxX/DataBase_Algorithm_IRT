// include/command_processor.h
#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include "sql_parser.h"  // Incluye la definición de ParsedCommand

void execute_command(ParsedCommand command);
int check_database_selected();
#endif // COMMAND_PROCESSOR_H
