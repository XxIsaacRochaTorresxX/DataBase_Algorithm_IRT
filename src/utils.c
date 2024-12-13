#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

void error_exit(const char* message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}
